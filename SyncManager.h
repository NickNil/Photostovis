#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BackupFileContent
{
  const char* pathHash;
  const char* fileHash;
  const char* filePath;
};


unsigned int photostovis_read_number_of_lines_in_file(FILE* fp);

void photostovis_read_backup_file(char* backupFilePath, struct BackupFileContent*);

void photostovis_sync_files_to_server();

void photostovis_client_server_backup_diff(struct BackupFileContent* client,
                                                                unsigned int clientLength,
                                                                struct BackupFileContent* server,
                                                                unsigned int serverLength,
                                                                struct BackupFileContent* result);

const char* photostovis_backup_file_getfield(char* line, int num);
