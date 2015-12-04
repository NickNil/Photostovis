#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
#include "scrambler.h"
#include "error_mgmt.h"
#include "boyer_moore.h"
#include "encrypt_main.h"


#define KEYLEN 1024
// Path for saving encrypted files
#define ENCRYPT "/home/global-sw-dev/Photostovis/encrypted/"

char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz23456789";

// Create random name for encrypted files
void createfilename(char *pw){
    int i=0, j=0, k=0, rem, nchars;
    unsigned int r;
    nchars = strlen(charset);
    for(i=0; i<4; i++){
        r = random();
        for(j=0; j<8; j++){
            rem = r%nchars;
            pw[k++] = charset[rem];
            r/=nchars;
       }
    }
    pw[k] = '\0';
    return;
}


// Save encrypted file in specified folder
void save_file(char *filename, char *pic_file) 
{
    strncpy(filename, ENCRYPT, sizeof(ENCRYPT));
    strcat(filename, pic_file);
}

// Create Key and Encrypt files
void photostovis_run_encryption(char **added_files, int numberoffiles)
{
    FILE *fd = NULL, *dstFile = NULL;
    unsigned char *keySequence;
    keySequence = malloc(KEYLEN*sizeof(unsigned char));
    char *shifted_filename, *encrypted_filename, *pic_file;
    shifted_filename = malloc(255*sizeof(char));
    encrypted_filename = malloc(255*sizeof(char));
    pic_file = malloc(255*sizeof(char));
    char** shiftedfiles;
    shiftedfiles = malloc(numberoffiles*sizeof(char*));
    FILE **shifted_files, **encrypted_files;
    shifted_files = malloc(numberoffiles*sizeof(FILE*));
    encrypted_files = malloc(numberoffiles*sizeof(FILE*));
    int pic;
    for(pic=0; pic<numberoffiles; pic++)
    {
        fd = fopen(strtok(strtok(added_files[pic], "\""), "\""), "r");
        // Creating byte shifted files in encrypted folder 
        memset(pic_file, 0, 255);
        createfilename(pic_file);
        save_file(shifted_filename, pic_file); 
        shiftedfiles[pic] = strndup(shifted_filename, 255);

        dstFile = fopen(shifted_filename, "w+");

        if(NULL == fd || NULL == dstFile)
        {
            printf("\n fopen() Error!!\n");
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
    // TODO check if the key file is already generated
    computeKeyFromFiles(shifted_files, numberoffiles, keySequence, KEYLEN);
    // Key created after this point

    for(pic=0; pic<numberoffiles; pic++){
        fclose(shifted_files[pic]);
    }

    // Encryption of files using Key starts here 
    char *srcFile;
    for(pic=0; pic<numberoffiles; pic++){
        srcFile = strndup(shiftedfiles[pic], 255);
        shifted_files[pic] = fopen(srcFile, "r");
        // Creating encrypted files in encrypted folder 
        memset(pic_file, 0, 255);
        createfilename(pic_file);
        save_file(encrypted_filename, pic_file); 
        encrypted_files[pic] = fopen(encrypted_filename, "w+");
        if(NULL == encrypted_files[pic])
        {
            printf("\n fopen() Error!!\n");
            exit(-1);
        }

        scrambleFileContents(shifted_files[pic], encrypted_files[pic], keySequence, KEYLEN);
        fclose(shifted_files[pic]);
        free(srcFile);
        fclose(encrypted_files[pic]);
    }

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

// Reading from the text file where new added files are listed
void photostovis_read_newfiles()
{
    // Reading new added files in filesadded.txt file
    FILE* newfd = NULL;
    // TODO Provide the file to read
    newfd = fopen("./textfiles/filesadded.txt", "r");
    if(NULL == newfd)
    {
        printf("\n fopen() Error!!\n");
        exit(-1);
    }

    int numberoffiles = 4, filecount = 0, k;
    char buff[255];
    char *token;
    char **added_files;
    added_files = malloc(numberoffiles*sizeof(char*));
    // TODO only 10 files to be chosen random for key creation
    while(NULL != fgets(buff, 255, newfd))
    {
        token = strtok(buff, ",");
        if(filecount>=numberoffiles){
            numberoffiles++;
            added_files = realloc(added_files, numberoffiles*sizeof(char*)); 
        }
        added_files[filecount] = strndup(token, 255);
        filecount++;
    }
    // Call function to create key and encrypt files in added_files array
    photostovis_run_encryption(added_files, numberoffiles);
    for(k=0; k<numberoffiles; k++){
        free(added_files[k]);
    }
    free(added_files);
    fclose(newfd);
}

// Encrypt function called from main program
void photostovis_encrypt_files()
{
    // Check to see if encrypted directory exists, if not create one
    struct stat st = {0};
    if (stat("./encrypted", &st) == -1) {
        mkdir("./encrypted", 0700);
    }

    scramblerInitialize();
    photostovis_read_newfiles();
}

