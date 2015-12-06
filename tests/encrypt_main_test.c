#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
#include "../encrypt_main.h"

#define ENCRYPTDIR "./encrypted"


// Test file_exists() function 
int test_file_exists(char *filename)
{
    int test_passed = 0;
    // Test with NULL filename
    assert(file_exists(NULL)!=0);
    test_passed++;

    // Test with valid filename
    assert(file_exists(filename)==0);
    test_passed++;

    return test_passed;
}

// Test empty_dir() function
int test_empty_dir(char *dirpath, char *filename)
{
    int test_passed = 0;
    // Call to empty_dir() function
    empty_dir(dirpath);
    // Assert if file readme.txt exists 
    assert(file_exists(filename) != 0);
    test_passed++;
    
    return test_passed;
}

// Test createfilename() function
int test_createfilename(char *filename)
{
    int test_passed = 0;
    createfilename(filename);
    assert(strlen(filename)==32);
    test_passed++;
    
    return test_passed;
}

// Test save_file() function 
int test_save_file(char *dir, char *filename, char *pic_file)
{
    int test_passed = 0;
    save_file(dir, filename, pic_file);
    assert(strlen(filename)==strlen(dir)+strlen(pic_file)+1);
    test_passed++;

    return test_passed;
}

int main()
{
    int test_passed = 0;
    // Check the existence of ENCRYPTDIR and create one 
    struct stat st = {0};
    if (stat(ENCRYPTDIR, &st) == -1) {
        mkdir(ENCRYPTDIR, 0700);
    }

    // Create readme.txt file inside ENCRYPTDIR
    char *filename = malloc(255*sizeof(char));
    strncpy(filename, ENCRYPTDIR, sizeof(ENCRYPTDIR));
    strcat(filename, "/");
    strcat(filename, "readme.txt");
    FILE *readme = fopen(filename, "w+");
    fclose(readme);
    char *randomfilename;
    randomfilename = malloc(255*sizeof(char));
    strcpy(randomfilename, "randomfile.txt");
    char *filepath = malloc(255*sizeof(char));

    // Call test functions
    test_passed += test_file_exists(filename);
    test_passed += test_empty_dir(ENCRYPTDIR, filename);
    test_passed += test_createfilename(randomfilename);
    test_passed += test_save_file(ENCRYPTDIR, filepath, filename);
    
    printf("\n %d Tests Passed.\n", test_passed);

    // Cleaning up
    free(filepath);
    free(filename);
    free(randomfilename);
    unlink(filename);
    remove(ENCRYPTDIR);
   
    return 0;
}
