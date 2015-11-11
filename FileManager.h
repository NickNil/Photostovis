#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sha256.h>

void photostovis_get_filenames_from_client();
void photostovis_hash_file_content_on_client(char* name, char* path);
char* photostovis_hash_file_path_on_client(char* path);
