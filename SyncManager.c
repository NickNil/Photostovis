
#include "client.h"
#include "SyncManager.h"

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
void photostovis_read_backup_file(char* backupFilePath, struct BackupFileContent* fileContent)
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
void photostovis_sync_files_to_server(int socket, char* server2, unsigned int port, char* const basePath)
{
    receive_file(socket);
    printf("RECIEVED BACKUPFILE\n");
    char* path = "/home/global-sw-dev/Photostovis/received_backup.txt";
    char* file_name = path;
    FILE* fp = fopen(file_name,"r");
    uint32_t htonl_size = 0;

    if(fp == NULL)
    {
       perror("Error while opening backup file.\n");
       return;
    }

    int server_file_length = photostovis_read_number_of_lines_in_file(fp);
    rewind(fp);
    fclose(fp);

    char* client_path = "/home/global-sw-dev/Photostovis/backup.txt";
    char* client_file_name = client_path;
    FILE* client_fp = fopen(client_file_name,"r");


    if(client_fp == NULL)
    {
       perror("Error while opening backup file.\n");
       return;
    }

    int client_file_length = photostovis_read_number_of_lines_in_file(client_fp);
    //printf("\n CLIENTL: %d", client_file_length);
    rewind(client_fp);
    fclose(client_fp);

    struct BackupFileContent server[server_file_length];
    photostovis_read_backup_file(path, server);

    struct BackupFileContent client[client_file_length];
    photostovis_read_backup_file(client_path, client);


    int size = photostovis_client_server_backup_diff(client, client_file_length, server, server_file_length, NULL, 0);
    //printf("\n SIZE : %d", size);
    struct BackupFileContent result[size];
    photostovis_client_server_backup_diff(client, client_file_length, server, server_file_length, result, 1);

    htonl_size = htonl(size); //converting size to network byte order
    write(socket, &htonl_size, sizeof(uint32_t)); //sending image number
    printf("image number: %d\n", size);

    int i;
    for(i = 0; i < size; i++)
    {

        // Ensure we only are working on valid files
        if (result[i].fileHash != NULL)
        {
           printf("\nSaving file: %s", result[i].fileHash);
        }
        send_image(socket, result[i].filePath, basePath);

        //close(socket);
        sleep(1);
    }

   //send_image(socket, "/home/global-sw-dev/Photostovis/image-03.jpg");

}


