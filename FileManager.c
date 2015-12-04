#include "FileManager.h"
#include "assert.h"

void photostovis_get_filenames_from_client(char* const currentPath, const unsigned int currentPathLen, char full_exe_path[])
{
    DIR *dirp;
    char* path = currentPath;
    assert(path[currentPathLen-1]=='/');
    struct dirent *dp;
    char * filename;
    char * fullpath;
    dirp = opendir(path);
    while((dp = readdir(dirp)) != NULL)
    {
        if(strlen(dp->d_name) > 2)
        {
            if(dp->d_type==DT_DIR)
            {
                int nameLen = strlen(dp->d_name);
                memcpy(path+currentPathLen, dp->d_name, nameLen);
                path[currentPathLen+nameLen] = '/';
                path[currentPathLen+nameLen+1] = '\0';

                photostovis_get_filenames_from_client(path, currentPathLen+nameLen+1, full_exe_path);

            }
            else
            {
                filename = dp->d_name;
                fullpath = (char *) malloc(1 + strlen(path)+ strlen(filename) );
                strcpy(fullpath, path);
                strcat(fullpath, filename);
                char hash_content[65];
                photostovis_hash_file_content_on_client(fullpath, hash_content);
                char hash_path[65];
                photostovis_hash_file_path_on_client(fullpath, hash_path);
                char* newpath = fullpath;

                photostovis_write_to_backup_file(newpath, hash_path, hash_content, full_exe_path);
            }
        }
        path[currentPathLen]='\0';
    }

    closedir(dirp);
}

int photostovis_hash_file_content_on_client(char* fullpath, char output[65])
{
    FILE* file = fopen(fullpath, "rb");
    if(!file) return -1;

    unsigned char hash[SHA256_BLOCK_SIZE];
    SHA256_CTX sha256;
    sha256_init(&sha256);
    const int bufSize = 32768;
    char* buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        sha256_update(&sha256, buffer, bytesRead);
    }
    sha256_final(&sha256, hash);

    sha256_hash_string(hash, output);
    fclose(file);
    free(buffer);
    return 0;
}

void photostovis_hash_file_path_on_client(char* path, char output[65])
{
    unsigned char buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, path, strlen(path));
    sha256_final(&ctx, buf);

    sha256_hash_string(buf, output);
}

void sha256_hash_string (char hash[SHA256_BLOCK_SIZE], char outputBuffer[65])
{
    int i = 0;

    for(i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", (unsigned char)hash[i]);
    }

    outputBuffer[64] = 0;
}

void photostovis_write_to_backup_file(char* path, char hash_path[65], char hash_file[65], char full_exe_path[])
{
    char backup_path[strlen(full_exe_path)];
    strcpy(backup_path, full_exe_path);

    strcat(backup_path, "backup.txt");

    FILE* file = fopen(backup_path, "a");
    fprintf(file,"%s",path);
    fprintf(file,"%s",",");
    fprintf(file,"%s",hash_path);
    fprintf(file,"%s",",");
    fprintf(file,"%s",hash_file);
    fprintf(file,"%s","\n");
    fclose(file);
}

void photostovis_clear_backup_file(char full_exe_path[])
{
    char backup_path[strlen(full_exe_path)];
    strcpy(backup_path, full_exe_path);

    strcat(backup_path, "backup.txt");

    FILE* file = fopen(backup_path, "w");
    fclose(file);
}
