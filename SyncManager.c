#include "SyncManager.h"

const char* photostovis_backup_file_getfield(char* line, int num)
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


struct BackupFileContent* photostovis_read_backup_file(char* backupFilePath)
{
   char* file_name = backupFilePath;
   FILE* fp = fopen(file_name,"r");

   if(fp == NULL)
   {
      perror("Error while opening backup file.\n");
      return NULL;
   }

   int file_length = photostovis_read_number_of_lines_in_file(fp);
   rewind(fp);

   struct BackupFileContent file_content[file_length];

   char line[1024];
   int counter = 0;
   while (fgets(line, 1024, fp))
   {
       char* tmp = strdup(line);

       struct BackupFileContent temp_line;

       temp_line.pathHash = photostovis_backup_file_getfield(tmp, 0);
       temp_line.fileHash = photostovis_backup_file_getfield(tmp, 1);
       temp_line.lineNumber = atoi(photostovis_backup_file_getfield(tmp, 2));
       printf("\n%s, LINE: %d - %d", backupFilePath, temp_line.lineNumber, file_length);

       file_content[counter] = temp_line;
       free(tmp);
       counter++;
   }
   fclose(fp);

   return file_content;

   /*
   struct BackupFileContent test_1;
   test_1.fileHash = "29fabd8487c46119cc0a58540f4de6e6ef630326d6c1ffe47492e8633cc6f863";
   test_1.pathHash = "29fabd8487c46119cc0a58540f4de6e6ef630326d6c1ffe47492e8633cc6f863";
   test_1.lineNumber = 0;

   struct BackupFileContent server[1];
   server[0] = test_1;

   photostovis_client_server_backup_diff(file_content, file_length, server, 1);
   */
}




struct BackupFileContent* photostovis_client_server_backup_diff(struct BackupFileContent* client,
                                                                unsigned int clientLength,
                                                                struct BackupFileContent* server,
                                                                unsigned int serverLength)
{

    int equal;

    unsigned int i;
    unsigned int j;
    unsigned int k = 0;

    struct BackupFileContent result[clientLength];

     printf("\nc: %d", client[0].lineNumber);
     printf("\nc: %d", client[1].lineNumber);
     printf("\ns: %d", server[0].lineNumber);
    for(i = 0; i < clientLength; i++)
    {
        equal = 0;
        for(j = 0; j < serverLength; j++)
        {
            if (client[i].pathHash == server[j].pathHash &&
                client[i].fileHash == server[j].fileHash)
            {
                equal = 1;
                //printf("\n%d - MATCH: %s", i, client[i].fileHash);

            }
        }

        if (equal == 0)
        {
            result[k] = client[i];
            printf("\n%s", client[i].fileHash);
            k++;
        }
    }

    return result;
}


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


void photostovis_sync_files_to_server()
{
    char* path = "/home/global-sw-dev/Photostovis/server-backup.txt";
    char* file_name = path;
    FILE* fp = fopen(file_name,"r");

    if(fp == NULL)
    {
       perror("Error while opening backup file.\n");
       return NULL;
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
       return NULL;
    }

    int client_file_length = photostovis_read_number_of_lines_in_file(client_fp);
    rewind(client_fp);
    fclose(client_fp);

    struct BackupFileContent* Server = photostovis_read_backup_file(path);
    int i;
    for(i = 0; i < server_file_length; i++)
    {
       printf("\n SERVER: %d", Server[i].lineNumber);
    }

    struct BackupFileContent* Client = photostovis_read_backup_file(client_path);

    for(i = 0; i < client_file_length; i++)
    {
       printf("\n CLIENT: %d", Client[i].lineNumber);
    }
/*
    struct BackupFileContent* result = photostovis_client_server_backup_diff(Client, client_file_length,
                                                                             Server, server_file_length);

*/

}
