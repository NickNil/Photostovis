#include "error_mgmt.h"

#include <stdio.h>

void printUsageOnScreen()
{
    printf("This program needs the name of the file to process in order to function!");
}

void printErrorOpeningFile()
{
    printf("For some reason the file passed as a parameter to the program could not be opened!");
}

void printErrorReadingFile()
{
    printf("For some reason the file passed as a parameter to the program could not be read!");
}

void printErrorWritingFile()
{
    printf("For some reason the file passed as a parameter to the program could not be written!");
}

void printErrorAllocMem()
{
    printf("Could not allocate some memory I needed to do my stuff!");
}

void printErrorStartSequenceNotFound()
{
    printf("Could not find the graphics file start sequence in the \"prepared\" file, probably file is not of graphics type\n");
}
