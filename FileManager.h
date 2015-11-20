#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sha256.h>

void photostovis_get_filenames_from_client(char* const currentPath, const unsigned int currentPathLen);
int photostovis_hash_file_content_on_client(char* fullpath, char output[65]);
int photostovis_hash_file_path_on_client(char* path, char output[65]);
void sha256_hash_string (char hash[SHA256_BLOCK_SIZE], char outputBuffer[65]);
void photostovis_write_to_backup_file(char* path, char hash_path[65], char hash_file[65]);
void photostovis_clear_backup_file();
