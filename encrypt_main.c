#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include "scrambler.h"
#include "error_mgmt.h"
#include "boyer_moore.h"
#include "encrypt_main.h"


#define KEYLEN 1024
#define KEYFILE "keyfile.txt"
// Path for saving encrypted files
#define ENCRYPTDIR "/home/global-sw-dev/encrypted"
#define ENCRYPTEDLIST "encrypted_list.txt"
#define NEWFILELIST "./textfiles/filesadded.txt"

char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz23456789";

// Checks if the given file is accessible or not
int file_exists(char *filename)
{
    return access(filename, F_OK);
}

// Delete directory contents
void empty_dir(char *path)
{
    DIR *dirptr;
    struct dirent *direntry;
    char filepath[255];

    dirptr = opendir(path);

    if(dirptr != NULL)
    {
        while((direntry = readdir(dirptr)) != NULL)
        {
            if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, ".."))
            {
                strcpy(filepath, path);
                strcat(filepath, "/");
                strcat(filepath, direntry->d_name);
                if(unlink(filepath) != 0)
                {
                    printf("\nError deleting %s \n", filepath);
                }
            }
        }
    }
    closedir(dirptr);
}

// Create random name for encrypted files
void createfilename(char *filename){
    int i=0, j=0, k=0, rem, nchars;
    unsigned int r;
    nchars = strlen(charset);
    for(i=0; i<4; i++){
        r = random();
        for(j=0; j<8; j++){
            rem = r%nchars;
            filename[k++] = charset[rem];
            r/=nchars;
        }
    }
    filename[k] = '\0';
    return;
}


// Save encrypted file in specified folder
void save_file(char *dir, char *filename, char *pic_file) 
{
    strncpy(filename, dir, 255);
    strcat(filename, "/");
    strcat(filename, pic_file);
}

// Create Key and Encrypt files
void photostovis_run_encryption(char **added_files, char **encryptedfilepath, int numberoffiles)
{
    if(numberoffiles > 0)
    {
        FILE *fd = NULL, *dstFile = NULL, *encrypted_list = NULL;
        unsigned char *keySequence;
        keySequence = malloc(KEYLEN*sizeof(unsigned char));
        char *shifted_filename, *encrypted_filename, *pic_file;
        shifted_filename = malloc(255*sizeof(char));
        encrypted_filename = malloc(255*sizeof(char));
        pic_file = malloc(255*sizeof(char));
        // This contains array of shifted file paths
        char** shiftedfiles;
        shiftedfiles = malloc(numberoffiles*sizeof(char*));
        // These are arrays of file pointers
        FILE **shifted_files, **encrypted_files;
        shifted_files = malloc(numberoffiles*sizeof(FILE*));
        encrypted_files = malloc(numberoffiles*sizeof(FILE*));

        encrypted_list = fopen("encrypted_list.txt", "a");
        if(encrypted_list == NULL)
        {
            printf("\n encrypted list fopen() Error!!\n");
            exit(-1);
        }
        int pic;
        for(pic=0; pic<numberoffiles; pic++)
        {
            fd = fopen(strtok(strtok(added_files[pic], "\""), "\""), "r");
            // Creating byte shifted files in encrypted folder 
            memset(pic_file, 0, 255);
            createfilename(pic_file);
            save_file(ENCRYPTDIR, shifted_filename, pic_file); 
            shiftedfiles[pic] = strndup(shifted_filename, 255);

            dstFile = fopen(shifted_filename, "w+");

            if(NULL == fd || NULL == dstFile)
            {
                printf("\n fd dstFile fopen() Error!!\n");
                exit(-1);
            }

            shiftFileContents(fd, dstFile);
            shifted_files[pic] = fopen(shifted_filename, "rw");

            if(NULL == shifted_files[pic])
            {
                printf("\n fopen() Error!!\n");
                exit(-1);
            }
            fclose(fd);
            fclose(dstFile);
        }

        // Key Creation starts here
        computeKeyFromFiles(shifted_files, numberoffiles, keySequence, KEYLEN);

        // Writing created key to file
        if(file_exists(KEYFILE) != 0)
        {
            //printf("\n keyfile doesn't exists!!\n");
            FILE *keyfile = fopen(KEYFILE, "w+");
            if(KEYLEN != fwrite(keySequence, 1, KEYLEN, keyfile))
            {
                printf("\n keyfile fwrite() Error!!\n");
                exit(-1);
            }
        }

        for(pic=0; pic<numberoffiles; pic++){
            fclose(shifted_files[pic]);
        }

        // Encryption of files using Key starts here 
        char *srcFile, *encrypted_list_content;
        encrypted_list_content = malloc(255*sizeof(char));
        for(pic=0; pic<numberoffiles; pic++){
            srcFile = strndup(shiftedfiles[pic], 255);
            shifted_files[pic] = fopen(srcFile, "r");
            // Creating encrypted files in encrypted folder 
            memset(pic_file, 0, 255);
            createfilename(pic_file);
            save_file(ENCRYPTDIR, encrypted_filename, pic_file); 
            encryptedfilepath[pic] = strndup(encrypted_filename, 255);
            encrypted_files[pic] = fopen(encrypted_filename, "w+");
            if(NULL == encrypted_files[pic])
            {
                printf("\n encrypted files fopen() Error!!\n");
                exit(-1);
            }

            // Calling encrypt function to scramble file contents
            scrambleFileContents(shifted_files[pic], encrypted_files[pic], keySequence, KEYLEN);

            // Write encrypted file path and random generated name to encrypted_list file
            memset(encrypted_list_content, 0, 255);
            strncpy(encrypted_list_content, added_files[pic], 255);
            //strcat(encrypted_list_content, "\",");
            strcat(encrypted_list_content, ",");
            strcat(encrypted_list_content, pic_file);
            strcat(encrypted_list_content, "\n");
            if(strlen(encrypted_list_content) != fwrite((encrypted_list_content), 1, strlen(encrypted_list_content), encrypted_list))
            {
                printf("\n fwrite() Error!!\n");
                exit(-1);
            }
            fclose(shifted_files[pic]);
            free(srcFile);
            fclose(encrypted_files[pic]);
        }

        fclose(encrypted_list);
        free(encrypted_list_content);
        free(shifted_files);
        free(encrypted_files);
        free(shifted_filename);
        free(encrypted_filename);
        free(keySequence);
        for(pic=0; pic<numberoffiles; pic++){
            remove(shiftedfiles[pic]);
            free(shiftedfiles[pic]);
        }
        free(shiftedfiles);
        free(pic_file);

    }
}

//// Reading from the text file where new added files are listed
//void photostovis_read_newfiles(char *newfilelist)
//{
//    // Reading new added files in filesadded.txt file
//    FILE* newfd = NULL, *oldfd = NULL;
//    int numberoffiles = 4,  filecount = 0, k=0, i=0;
//    int oldnumberoffiles = 4, encrypted_files_exists = 0, unencrypted_file = 0;
//    char buff[255];
//    char *token;
//    char **added_files, **old_encrypted_files;
//    old_encrypted_files = malloc(oldnumberoffiles*sizeof(char*));
//    // TODO Provide the file to read
//    newfd = fopen(newfilelist, "r");
//    if(NULL == newfd)
//    {
//        printf("\n fopen() Error!!\n");
//        exit(-1);
//    }
//    // Check if file containing list of already encrypted files exists 
//    if(file_exists(ENCRYPTEDLIST) == 0)
//    {
//        encrypted_files_exists = 1;
//        oldfd = fopen(ENCRYPTEDLIST, "r");
//
//        if(NULL == oldfd)
//        {
//            printf("\n fopen() Error!!\n");
//            exit(-1);
//
//        }
//        // Reading list of old encrypted files 
//        while(NULL != fgets(buff, 255, oldfd))
//        {
//            token = strtok(buff, ",");
//            if(filecount>=oldnumberoffiles){
//                oldnumberoffiles++;
//                old_encrypted_files = realloc(old_encrypted_files, oldnumberoffiles*sizeof(char*)); 
//            }
//            old_encrypted_files[filecount] = strndup(token, 255);
//            filecount++;
//        }
//        //printf("\n Encrypted list file exists = %d\n", encrypted_files_exists);
//        filecount = 0;
//    }
//    else
//    {
//        // Empty encrypted directory if encrypted_list.txt doesn't exists
//        empty_dir(ENCRYPTDIR);
//    }
//    // TODO only 10 files to be chosen random for key creation
//    // Initializing added_files array
//    added_files = malloc(numberoffiles*sizeof(char*));
//    while(NULL != fgets(buff, 255, newfd))
//    {
//        token = strtok(buff, ",");
//        if(filecount>=numberoffiles){
//            numberoffiles++;
//            added_files = realloc(added_files, numberoffiles*sizeof(char*)); 
//            //i++;
//            //added_files[i] = malloc(255*sizeof(char));
//        }
//        if(encrypted_files_exists == 1)
//        {
//            for(k=0; k<oldnumberoffiles; k++)
//            {
//                // Compare the new filepath with the already encrypted file path
//                if(strcmp(old_encrypted_files[k], token) != 0)
//                {
//                    unencrypted_file = 1;
//                }
//                else 
//                {
//                    unencrypted_file = 0;
//                    break; 
//                }
//            }
//            if(unencrypted_file == 1)
//            {
//                added_files[filecount] = strndup(token, 255);
//                filecount++;
//            }
//        }
//        else
//        {
//            added_files[filecount] = strndup(token, 255);
//            filecount++;
//        }
//
//    }
//    // Initializing unitialized space of added_files array
//    for(i=filecount;i<numberoffiles;i++)
//    {
//        added_files[i] = malloc(255*sizeof(char));
//    }
//    // Call function to create key and encrypt files in added_files array
//    photostovis_run_encryption(added_files, filecount);
//
//    // Free heap allocations
//    if(encrypted_files_exists == 1){
//        for(k=0; k<oldnumberoffiles; k++){
//            free(old_encrypted_files[k]);
//        }
//        fclose(oldfd);
//    }
//    for(k=0; k<numberoffiles; k++){
//        free(added_files[k]);
//    }
//    free(old_encrypted_files);
//    free(added_files);
//    fclose(newfd);
//}

// Reading from the encrypted list text file 
int photostovis_check_duplicate_entry(char **unsynced_files, char **encryptedfilepath, int numberoffiles)
{
    // Reading new added files in filesadded.txt file
    FILE *oldfd = NULL;
    int filecount = 0, k=0, i=0, index=0;
    int oldnumberoffiles = 4, encrypted_files_exists = 0, unencrypted_file = 0;
    char buff[255];
    char *token;
    char **added_files, **old_encrypted_files;
    old_encrypted_files = malloc(oldnumberoffiles*sizeof(char*));

    // Check if file containing list of already encrypted files exists 
    if(file_exists(ENCRYPTEDLIST) == 0)
    {
        encrypted_files_exists = 1;
        oldfd = fopen(ENCRYPTEDLIST, "r");

        if(NULL == oldfd)
        {
            printf("\n fopen() Error!!\n");
            exit(-1);

        }
        // Reading list of old encrypted files 
        while(NULL != fgets(buff, 255, oldfd))
        {
            token = strtok(buff, ",");
            if(filecount>=oldnumberoffiles){
                oldnumberoffiles++;
                old_encrypted_files = realloc(old_encrypted_files, oldnumberoffiles*sizeof(char*)); 
            }
            old_encrypted_files[filecount] = strndup(token, 255);
            filecount++;
        }
        //printf("\n Encrypted list file exists = %d\n", encrypted_files_exists);
        filecount = 0;
    }
    else
    {
        // Empty encrypted directory if encrypted_list.txt doesn't exists
        empty_dir(ENCRYPTDIR);
    }
    // TODO only 10 files to be chosen random for key creation
    // Initializing added_files array
    added_files = malloc(numberoffiles*sizeof(char*));
    for(index=0; index<numberoffiles; index++)
    {
        if(encrypted_files_exists == 1)
        {
            for(k=0; k<oldnumberoffiles; k++)
            {
                // Compare the new filepath with the already encrypted file path
                if(strcmp(old_encrypted_files[k], unsynced_files[index]) != 0)
                {
                    unencrypted_file = 1;
                }
                else 
                {
                    unencrypted_file = 0;
                    break; 
                }
            }
            if(unencrypted_file == 1)
            {
                added_files[filecount] = strndup(unsynced_files[index], 255);
                filecount++;
            }
        }
        else
        {
            added_files[filecount] = strndup(unsynced_files[index], 255);
            filecount++;
        }

        // Initializing unitialized space of added_files array
        for(i=filecount;i<numberoffiles;i++)
        {
            added_files[i] = malloc(255*sizeof(char));
        }
    }
    // Call function to create key and encrypt files in added_files array
    photostovis_run_encryption(added_files, encryptedfilepath, filecount);

    // Free heap allocations
    if(encrypted_files_exists == 1){
        for(k=0; k<oldnumberoffiles; k++){
            free(old_encrypted_files[k]);
        }
        fclose(oldfd);
    }
    for(k=0; k<numberoffiles; k++){
        free(added_files[k]);
    }
    free(old_encrypted_files);
    free(added_files);
    return filecount;
}

// Encrypt function called from main program
int photostovis_encrypt_files(char **unsynced_files, char **encryptedfilepath, int numberoffiles)
{
    // Check to see if encrypted directory exists, if not create one
    struct stat st = {0};
    if (stat(ENCRYPTDIR, &st) == -1) {
        mkdir(ENCRYPTDIR, 0700);
    }
    int filecount = 0;

    scramblerInitialize();
    //photostovis_read_newfiles(NEWFILELIST);
    filecount = photostovis_check_duplicate_entry(unsynced_files, encryptedfilepath, numberoffiles);
    //photostovis_run_encryption(unsynced_files, numberoffiles);
    return filecount;
}

