#include "FileManager.h"

void photostovis_get_filenames_from_client()
{
    DIR *dirp;
    char* path = "/home/global-sw-dev/Photostovis/pictures/";
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
            //char* hash_conent = photostovis_hash_file_content_on_client(filename, path);
            char* hash_path = photostovis_hash_file_path_on_client(fullpath);
            char* newpath = fullpath;

            //Add hash_content, hash_path and newpath to the backup.txt file.Append or Depend.

            printf("\n FULLPATH: %s", newpath);
            printf("\n HASHPATH: %s", hash_path);
            //printf("\n HASHCONTENT: %s", hash_content);
        }
    }
    closedir(dirp);
}

void photostovis_hash_file_content_on_client(char* name, char* path)
{

}

char* photostovis_hash_file_path_on_client(char* path)
{
    BYTE buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, path, strlen(path));
    sha256_final(&ctx, buf);

    int i;
    int size = 32;
    char* buf_str = (char*) malloc (2*size + 1);
    char* result = (char*) malloc (2*size + 1);
    char* buf_ptr = buf_str;
    for (i = 0; i < size; i++)
    {
        //printf("\n BUFFER: %02X", buf[i]);
        sprintf(buf_ptr, "%02X", buf[i]);
        strcat(result, buf_ptr);
    }
    //printf("\n BUFFER2: %s", result);
    return result;
}
