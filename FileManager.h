#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sha256.h>

void photostovis_get_filenames_from_client();
int photostovis_hash_file_content_on_client(char* fullpath, char output[32]);
int photostovis_hash_file_path_on_client(char* path, char output[32]);
void sha256_hash_string (char hash[SHA256_BLOCK_SIZE], char outputBuffer[32]);
