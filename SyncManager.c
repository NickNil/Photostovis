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


void photostovis_read_local_backup_file()
{
   char* file_name = "/home/global-sw-dev/Photostovis/backup.txt";
   FILE* fp = fopen(file_name,"r");

   if(fp == NULL)
   {
      perror("Error while opening local backup file.\n");
      return;
   }


   int client_length = photostovis_read_number_of_lines_in_file(fp);

   rewind(fp);

   struct BackupFileContent client[client_length];

   char line[1024];
   int counter = 0;
   while (fgets(line, 1024, fp))
   {
       char* tmp = strdup(line);

       struct BackupFileContent temp_line;

       temp_line.pathHash = photostovis_backup_file_getfield(tmp, 0);
       temp_line.fileHash = photostovis_backup_file_getfield(tmp, 1);
       temp_line.lineNumber = atoi(photostovis_backup_file_getfield(tmp, 2));
       printf("HASH %s\n", temp_line.pathHash);
       printf("FPATH %s\n", temp_line.fileHash);
       printf("NO %d\n", temp_line.lineNumber);

       client[counter] = temp_line;
       free(tmp);
       counter++;
   }

   struct BackupFileContent test_1;
   test_1.fileHash = "29fabd8487c46119cc0a58540f4de6e6ef630326d6c1ffe47492e8633cc6f863";
   test_1.pathHash = "29fabd8487c46119cc0a58540f4de6e6ef630326d6c1ffe47492e8633cc6f863";
   test_1.lineNumber = 0;

   struct BackupFileContent server[1];
   server[0] = test_1;

   photostovis_client_server_backup_diff(client, client_length, server, 1);
}




void photostovis_client_server_backup_diff(struct BackupFileContent client[], unsigned int clientLength,
                                           struct BackupFileContent server[], unsigned int serverLength)
{

    int equal;

    unsigned int i;
    unsigned int j;
    unsigned int k = 0;

    struct BackupFileContent result[clientLength];

    for(i = 0; i < clientLength; i++)
    {
        equal = 0;
        for(j = 0; j < serverLength; j++)
        {
            if (client[i].pathHash == server[j].pathHash &&
                client[i].fileHash == server[j].fileHash)
            {
                equal = 1;
            }
        }

        if (equal == 0)
        {
            result[k] = client[i];
            //printf("%s\n", result[k].fileHash);

            k++;
        }
    }

    for(i = 0; i < k; i++)
    {
        printf("%s\n", result[i].fileHash);
    }
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
