#include "FileManager.h"

void photostovis_get_filenames_from_client(char* const currentPath, const unsigned int currentPathLen)
{
    DIR *dirp;
    const char* path = currentPath;//"/home/global-sw-dev/Photostovis/pictures/";
    struct dirent *dp;
    char * filename;
    char * fullpath;
    dirp = opendir(path);
    while((dp = readdir(dirp)) != NULL)
    {
        if(strlen(dp->d_name) > 2)
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

            photostovis_write_to_backup_file(newpath, hash_path, hash_content);
        }
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

int photostovis_hash_file_path_on_client(char* path, char output[65])
{
    unsigned char buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, path, strlen(path));
    sha256_final(&ctx, buf);

    sha256_hash_string(buf, output);
    return 0;
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

void photostovis_write_to_backup_file(char* path, char hash_path[65], char hash_file[65])
{
    char* backup_path = "/home/global-sw-dev/Photostovis/backup.txt";
    FILE* file = fopen(backup_path, "a");
    fprintf(file,"%s","\"");
    fprintf(file,"%s",path);
    fprintf(file,"%s","\"");
    fprintf(file,"%s",",");
    fprintf(file,"%s",hash_path);
    fprintf(file,"%s",",");
    fprintf(file,"%s",hash_file);
    fprintf(file,"%s","\n");
    fclose(file);
}

void photostovis_clear_backup_file()
{
    char* backup_path = "/home/global-sw-dev/Photostovis/backup.txt";
    FILE* file = fopen(backup_path, "w");
    fclose(file);
}
