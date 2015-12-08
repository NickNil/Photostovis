
#include "client.h"
#include "SyncManager.h"
#include "encrypt_main.h"

#define ARRAYSIZE(x)  (sizeof(x)/sizeof(*(x)))
#define SHA256_DIGEST_LENGTH 32

/**
 * @brief Reads the number of lines in a file
 * @param fp : File pointer that needs to read
 * @return number of lines
 */
char* photostovis_backup_file_getfield(char* line, int num)
{
   char* array[3];
   int i = 0;
   char* p = strtok (line,",");
   while (p != NULL)
   {
     array[i++] = p;
     p = strtok (NULL, ",");
   }

   return array[num];
}

/**
 * @brief Reads local or server backup file
 * @param backupFilePath
 */
void photostovis_read_backup_file(char backupFilePath[], struct BackupFileContent* fileContent)
{
   char* file_name = backupFilePath;
   FILE* fp = fopen(file_name,"r");

   if(fp == NULL)
   {
      perror("Error while opening backup file.\n");
      return;
   }

   char line[1024];
   int counter = 0;

   while (fgets(line, 1024, fp))
   {
       char* tmp = strdup(line);
       struct BackupFileContent temp_line;
       temp_line.filePath = photostovis_backup_file_getfield(tmp, 0);
       temp_line.pathHash = photostovis_backup_file_getfield(tmp, 1);
       temp_line.fileHash = photostovis_backup_file_getfield(tmp, 2);
       //printf("\n FULLPATH: %s", tmp);
       fileContent[counter] = temp_line;
       counter++;
   }

   fclose(fp);
}


/**
 * @brief Checks the difference between two backup files
 * @param client
 * @param clientLength
 * @param server
 * @param serverLength
 * @param result : Files that will be uplaoded to server
 */
int photostovis_client_server_backup_diff(struct BackupFileContent* client,
                                                                unsigned int clientLength,
                                                                struct BackupFileContent* server,
                                                                unsigned int serverLength,
                                                                struct BackupFileContent* result,
                                                                int getResult)
{
    int equal;
    unsigned int i;
    unsigned int j;
    unsigned int k = 0;

    for(i = 0; i < clientLength; i++)
    {
        equal = 0;
        for(j = 0; j < serverLength; j++)
        {
            //printf("\n %s %s %d %d", client[i].filePath, server[j].filePath, !strcmp(client[i].pathHash, server[j].pathHash), !strcmp(client[i].fileHash, server[j].fileHash));
            if (!strcmp(client[i].fileHash, server[j].fileHash))
            {
                equal = 1;
            }
        }

        if (equal == 0)
        {
            if(getResult)
            {
                result[k] = client[i];
            }
            k++;
        }
    }
    return k;
}

/**
 * @brief Comma-seperator
 * @param line : String that will be split on delimiter
 * @param num : array index
 * @return
 */
unsigned int photostovis_read_number_of_lines_in_file(FILE* fp)
{
    unsigned int length = 0;
    char dummy[1024];
    while (fgets(dummy, 1024, fp))
    {
        length++;
    }

    return length;
}


/**
 * @brief Syncronizes files between client and server
 */
void photostovis_sync_files_to_server(int socket, char* server2, unsigned int port, char* const basePath, char full_exe_path[1024], int encrypt)
{
    receive_file(socket);
    printf("RECIEVED BACKUPFILE\n");

    char exe_path_received[strlen(full_exe_path)];
    strcpy(exe_path_received, full_exe_path);
    strcat(exe_path_received, "received_backup.txt");
    char* path = exe_path_received;
    char* file_name = path;

    FILE* fp = fopen(file_name,"r");
    uint32_t htonl_size = 0;

    if(fp == NULL)
    {
       perror("Error while opening backup file server.\n");
       return;
    }

    int server_file_length = photostovis_read_number_of_lines_in_file(fp);
    rewind(fp);
    fclose(fp);

    struct BackupFileContent server[server_file_length];
    photostovis_read_backup_file(exe_path_received, server);

    char exe_path_backup[strlen(full_exe_path)];
    strcpy(exe_path_backup, full_exe_path);
    strcat(exe_path_backup, "backup.txt");
    char* client_path = exe_path_backup;
    char* client_file_name = client_path;
    FILE* client_fp = fopen(client_file_name,"r");


    if(client_fp == NULL)
    {
       perror("Error while opening backup file client.\n");
       return;
    }

    int client_file_length = photostovis_read_number_of_lines_in_file(client_fp);
    //printf("\n CLIENTL: %d", client_file_length);
    rewind(client_fp);
    fclose(client_fp);

    struct BackupFileContent client[client_file_length];
    photostovis_read_backup_file(exe_path_backup, client);


    int size = photostovis_client_server_backup_diff(client, client_file_length, server, server_file_length, NULL, 0);
    //printf("\n SIZE : %d", size);
    struct BackupFileContent result[size];
    photostovis_client_server_backup_diff(client, client_file_length, server, server_file_length, result, 1);

    htonl_size = htonl(size); //converting size to network byte order
    write(socket, &htonl_size, sizeof(uint32_t)); //sending image number
    printf("image number: %d\n", size);

    int i, image_counter = 1;

    // Encrypt file before sending
    if(encrypt){
        int encrypted_filecount = 0;
        printf("\nEncrypting Files\n");
        //struct BackupFileContent encrypt_file[size];
        char **unsynced_files, **encryptedfilepath;
        unsynced_files = malloc(size*sizeof(char*));
        encryptedfilepath = malloc(size*sizeof(char*));
        for(i = 0; i < size; i++)
        {

            unsynced_files[i] = malloc(255*sizeof(char));
            encryptedfilepath[i] = malloc(255*sizeof(char));
            strcpy(unsynced_files[i], result[i].filePath);

        }
        encrypted_filecount = photostovis_encrypt_files(unsynced_files, encryptedfilepath, size);
        for(i = 0; i< size; i++)
        {
            free(unsynced_files[i]);
        }
        free(unsynced_files);
        printf("\nDone Encrypting files, Encrypted files saved in ./encrypted folder\n");
        for(i = 0; i < size; i++)
        {
            strcpy(result[i].filePath, encryptedfilepath[i]);
            //printf("\nEncrypted file path = %s\n", result[i].filePath);
        }
        for(i = 0; i< size; i++)
        {
            free(encryptedfilepath[i]);
        }
        free(encryptedfilepath);
        size = encrypted_filecount;
    }
    for(i = 0; i < size; i++)
    {
        printf("\nimage number: %d\n", image_counter);
        // Ensure we only are working on valid files
        if (result[i].fileHash != NULL)
        {
           //printf("\nSaving file: %s", result[i].fileHash);
        }
        send_image(socket, result[i].filePath, basePath);

        //close(socket);
        usleep(150000);
        image_counter++;
    }

   //send_image(socket, "/home/global-sw-dev/Photostovis/image-03.jpg");

}


