#include "scrambler.h"
#include "error_mgmt.h"
#include "boyer_moore.h"

/* required for memset(...) */
#include <string.h>
/* required for time(...) */
#include <time.h>

void scramblerInitialize(void)
{
    srand(time(NULL));
}

void zeroCharBuffer(unsigned char *bufferPtr, unsigned int bufferLen, unsigned int offset)
{
    memset(bufferPtr+offset, 0x00, bufferLen-offset);

    /* this variable, which must be of the same integer type as bufferLen, will be used as an index in the buffer while zeroing
     * unsigned int index;
     *
     * for(index = offset; index < bufferLen; index++)
     * {
     *     bufferPtr[index] = 0x00;
     * }
    */
}

void xorCharBuffers(unsigned char *srcBufferPtr, unsigned char *dstBufferPtr, unsigned bufferLen)
{
    /* this variable, which must be of the same integer type as bufferLen, will be used as an index in the buffers while xoring */
    unsigned int index;

    for(index = 0; index < bufferLen; index++)
    {
        dstBufferPtr[index] = srcBufferPtr[index] ^ dstBufferPtr[index];
    }

}

void computeKeyFromFile(FILE *filePtr, unsigned char *keySequence, unsigned int keyLen, int initialize)
{
    if (initialize != 0)
    {
        /* first thing to do is to pre-initialize the contents of the keySequence with the first keyLen bytes of the file */
        size_t readBytes = fread(keySequence, 1, keyLen, filePtr);

        /* now we need to check that we have actually read the desired amount of bytes */
        if (readBytes != keyLen)
        {
            /* here something happended because I read less bytes than requested... either error or file too short */
            /* let's check if we reached the end of the file */
            if (feof(filePtr) != 0)
            {
                /* since we have reached the end of the file, we just need to make sure that the keySequence only contains
                 * bytes that were actually in the file, so let's zero the remaining chunk till keyLength */
                zeroCharBuffer(keySequence, keyLen, readBytes);
                /* so unfortunately, for a file so short the key will be the actual file contents... let's return */
                return;
            }
            else
            {
                /* some error happened while reading the file, report problem, cleanup and exit program */
                printErrorReadingFile();
                fclose(filePtr);
                exit(-1);
            }
        }
    }
    /* allocate some temporary buffer of the same size as keySequence so we can read in the file chunk per chunk */
    unsigned char *tempBuffer = malloc(sizeof(unsigned char)*keyLen);
    /* check if memory was actually allocated, otherwise clean up and exit since we cannot proceed */
    if (tempBuffer == NULL)
    {
        printErrorAllocMem();
        fclose(filePtr);
        exit(-1);
    }

    /* now simply continue with the rest of the file till reaching its end */
    while (feof(filePtr) == 0)
    {
        /* read next keyLength bytes from the file into tempBuffer */
        size_t readBytes = fread(tempBuffer, 1, keyLen, filePtr);
        /* again, check that we have read the correct number of bytes */
        if (readBytes != keyLen)
        {
            /* here something happended cause I read less bytes than requested... either error or file too short */
            /* let's check if we reached the end of the file */
            if (feof(filePtr) != 0)
            {
                /* since we have reached the end of the file, we just need to make sure that the keySequence only contains
                 * bytes that were actually in the file, so let's zero the remaining chunk till keyLength */
                zeroCharBuffer(tempBuffer, keyLen, readBytes);
                /* now, XOR tempBuffer into keySequence and return cause we've done everything we could */
                xorCharBuffers(tempBuffer, keySequence, readBytes);
                /* so unfortunately, for a file so short the key will be the actual file contents... let's return */
            }
            else
            {
                /* some error happened while reading the file, report problem, cleanup and exit program */
                printErrorReadingFile();
                fclose(filePtr);
                free(tempBuffer);
                exit(-1);
            }
        }
        else
        {
            /* XOR the chunk of bytes we've just read with the keySequence contents */
            xorCharBuffers(tempBuffer, keySequence, keyLen);

        }

    }

    /* free the memory block previously allocated */
    free(tempBuffer);

    return;
}

void scrambleFileContents(FILE *srcFilePtr, FILE *destFilePtr, unsigned char *keySequence, unsigned int keyLen)
{
    /* allocate some temporary buffer of the same size as keySequence so we can read in the file chunk per chunk */
    unsigned char *tempBuffer = malloc(sizeof(unsigned char)*keyLen);
    /* check if memory was actually allocated, otherwise clean up and exit since we cannot proceed */
    if (tempBuffer == NULL)
    {
        printErrorAllocMem();
        fclose(srcFilePtr);
        fclose(destFilePtr);
        exit(-1);
    }

    /* cycle through the while file until EOF */
    while (feof(srcFilePtr) == 0)
    {
        /* read next keyLength bytes from the file into tempBuffer */
        size_t readBytes = fread(tempBuffer, 1, keyLen, srcFilePtr);

        /* again, check that we have read the correct number of bytes */
        if (readBytes != keyLen)
        {

            if (feof(srcFilePtr) != 0)
            {
                /* we reached the end of the file and the current chuck to process is shorter than the ciphering key
                 * so we will just use the starting bytes of the key sequence with this chunk */
                zeroCharBuffer(tempBuffer, keyLen, readBytes);
                /* now, XOR tempBuffer into keySequence and return cause we've done everything we could */
                xorCharBuffers(keySequence, tempBuffer, readBytes);
                /* now write this chunk to the destination file and return */
                size_t writtenBytes = fwrite(tempBuffer, 1, readBytes, destFilePtr);
                /* make sure that the information has been writen */
                if (writtenBytes < readBytes)
                {
                    /* some sort of error happened during writing of the file, abort and exit */
                    printErrorWritingFile();
                    fclose(srcFilePtr);
                    fclose(destFilePtr);
                    free(tempBuffer);
                    exit(-1);
                }
            }
            else
            {
                /* some error happened while reading the file, report problem, cleanup and exit program */
                printErrorReadingFile();
                fclose(srcFilePtr);
                fclose(destFilePtr);
                free(tempBuffer);
                exit(-1);
            }
        }
        else
        {

            /* XOR the chunk of bytes we've just read with the keySequence contents */
            xorCharBuffers(keySequence, tempBuffer, keyLen);
            /* now write this chunk to the destination file and return */
            size_t writtenBytes = fwrite(tempBuffer, 1, readBytes, destFilePtr);
            /* make sure that the information has been writen */

            if (writtenBytes < readBytes)
            {
                /* some sort of error happened during writing of the file, abort and exit */
                printErrorWritingFile();
                fclose(srcFilePtr);
                fclose(destFilePtr);
                free(tempBuffer);
                exit(-1);
            }
        }
    }

    /* free the memory block previously allocated */
    free(tempBuffer);

    return;
}

void computeKeyFromFiles(FILE **filePtr, int numFiles, unsigned char *keySequence, unsigned int keyLen)
{
    /* index file to scan through all the files in filePtr */
    int fileIndex;

    /* Calculate key chunk for the first file (thus initializing the keySequence buffer */
    computeKeyFromFile(filePtr[0], keySequence, keyLen, 1);

    /* Now, for each of the remaining files... */
    for(fileIndex = 1; fileIndex < numFiles; fileIndex++)
    {
        computeKeyFromFile(filePtr[fileIndex], keySequence, keyLen, 0);
    }

    return;
}

void shiftFileContents(FILE *srcFilePtr, FILE *dstFilePtr)
{
    unsigned int chunkSize = 1024;

    /* obtain source file size based on the FILE pointer we have */
    unsigned long srcFileSize;
    /* move file pointer all the way to the end of the file */
    fseek(srcFilePtr, 0L, SEEK_END);     /* watch out here, we're using C and the only guaranteed way to get file size is with stat, fseek behavior can be platform dependant */
    /* then request the file position... and we have our file size */
    srcFileSize = ftell(srcFilePtr);
    /* In theory both fseek and ftell can fail (return <> 0), so would need to check... TODO */

    /* now ramdomize this result (this is not the best way to obtain a ranged random (not uniform distrib due to module operation), and random() seed should be initialized somewhere) */
    unsigned long randomOffset = random() % srcFileSize;
    /* now position the file pointer in the randomized offset */
    fseek(srcFilePtr, randomOffset, SEEK_SET);
    /* In theory both fseek and ftell can fail (return <> 0), so would need to check... TODO */

    /* now we need to copy contents of srcFile from RandomOffset to srcFileSize to the beginning of dstFile, then roll over to the beginning of srcFile and copy from 0 to randomOffset
     * to the end of dstFile */

    /* first allocate some memory to copy the files */
    unsigned char *tempBuffer = malloc(sizeof(unsigned char)*chunkSize);
    /* check if memory was actually allocated, otherwise clean up and exit since we cannot proceed */
    if (tempBuffer == NULL)
    {
        printErrorAllocMem();
        fclose(srcFilePtr);
        fclose(dstFilePtr);
        exit(-1);
    }

    /* cycle through the while file until EOF */
    while (feof(srcFilePtr) == 0)
    {
        /* read next keyLength bytes from the file into tempBuffer */
        size_t readBytes = fread(tempBuffer, 1, chunkSize, srcFilePtr);
        /* again, check that we have read the correct number of bytes */
        if (readBytes != chunkSize)
        {
            if (feof(srcFilePtr) != 0)
            {
                /* now write this chunk to the destination file and return */
                size_t writtenBytes = fwrite(tempBuffer, 1, readBytes, dstFilePtr);
                /* make sure that the information has been writen */
                if (writtenBytes < readBytes)
                {
                    /* some sort of error happened during writing of the file, abort and exit */
                    printErrorWritingFile();
                    fclose(srcFilePtr);
                    fclose(dstFilePtr);
                    free(tempBuffer);
                    exit(-1);
                }
            }
            else
            {
                /* some error happened while reading the file, report problem, cleanup and exit program */
                printErrorReadingFile();
                fclose(srcFilePtr);
                fclose(dstFilePtr);
                free(tempBuffer);
                exit(-1);
            }
        }
        else
        {
            /* now write this chunk to the destination file and return */
            size_t writtenBytes = fwrite(tempBuffer, 1, readBytes, dstFilePtr);
            /* make sure that the information has been writen */
            if (writtenBytes < readBytes)
            {
                /* some sort of error happened during writing of the file, abort and exit */
                printErrorWritingFile();
                fclose(srcFilePtr);
                fclose(dstFilePtr);
                free(tempBuffer);
                exit(-1);
            }
        }
    }

    /* now need to copy the initial part of srcFile */
    /* so first reset file pointer to the beginning of the file */
    fseek(srcFilePtr, 0L, SEEK_SET);
    unsigned long pos = 0;
    /* cycle through the while file until EOF */
    while (pos < randomOffset)
    {
        /* this should only happen in the last itertion, so no need to save original chunkSize for later */
        if ((pos+chunkSize) > randomOffset)
        {
            chunkSize = randomOffset - pos;
        }

        /* read next keyLength bytes from the file into tempBuffer */
        size_t readBytes = fread(tempBuffer, 1, chunkSize, srcFilePtr);
        /* again, check that we have read the correct number of bytes */
        if (readBytes != chunkSize)
        {
            if (feof(srcFilePtr) != 0)
            {
                /* now write this chunk to the destination file and return */
                size_t writtenBytes = fwrite(tempBuffer, 1, readBytes, dstFilePtr);
                /* make sure that the information has been writen */
                if (writtenBytes < readBytes)
                {
                    /* some sort of error happened during writing of the file, abort and exit */
                    printErrorWritingFile();
                    fclose(srcFilePtr);
                    fclose(dstFilePtr);
                    free(tempBuffer);
                    exit(-1);
                }
            }
            else
            {
                /* some error happened while reading the file, report problem, cleanup and exit program */
                printErrorReadingFile();
                fclose(srcFilePtr);
                fclose(dstFilePtr);
                free(tempBuffer);
                exit(-1);
            }
        }
        else
        {
            /* now write this chunk to the destination file and return */
            size_t writtenBytes = fwrite(tempBuffer, 1, readBytes, dstFilePtr);
            /* make sure that the information has been writen */
            if (writtenBytes < readBytes)
            {
                /* some sort of error happened during writing of the file, abort and exit */
                printErrorWritingFile();
                fclose(srcFilePtr);
                fclose(dstFilePtr);
                free(tempBuffer);
                exit(-1);
            }
        }

        pos += chunkSize;
    }

    /* free the memory block previously allocated */
    free(tempBuffer);

    return;
}

void deShiftFileContents(FILE *srcFilePtr, FILE *destFilePtr, unsigned char *startSequence, unsigned long startSequenceLength)
{
    /* first we will put the whole file into memory */

    /* 1. find out what is the total size of the file */
    unsigned long srcFileSize = 0;
    /* move file pointer to the end of the file */
    fseek(srcFilePtr, 0L, SEEK_END);
    /* now query the location of the file pointer */
    srcFileSize = ftell(srcFilePtr);
    /* finally reset the pointer back to the beginning of the file */
    fseek(srcFilePtr, 0L, SEEK_SET);

    /* 2. now that we have the size of the file, allocate memory to hold it */
    unsigned char *fileBuffer = malloc(sizeof(unsigned char)*srcFileSize);
    /* check if memory was actually allocated, otherwise clean up and exit since we cannot proceed */
    if (fileBuffer == NULL)
    {
        printErrorAllocMem();
        fclose(srcFilePtr);
        fclose(destFilePtr);
        exit(-1);
    }

    /* 3. now copy the whole file into memory (need this in order to use Boyer-Moore algorithm */

    size_t readBytes = 0;
    while (feof(srcFilePtr) == 0)
    {
        /* read whole file into tempBuffer */
        readBytes += fread((fileBuffer+readBytes), 1, (srcFileSize + 1 - readBytes), srcFilePtr);
        /* again, check that we have read the correct number of bytes */
        if (readBytes < srcFileSize)
        {
            /* here something happended cause I read less bytes than requested... either error or file too short */
            /* let's check if we reached the end of the file or not */
            if (feof(srcFilePtr) == 0)
            {
                /* some error happened while reading the file, report problem, cleanup and exit program */
                printErrorReadingFile();
                fclose(srcFilePtr);
                fclose(destFilePtr);
                free(fileBuffer);
                exit(-1);
            }
        }
    }

    /* second we apply Boyer-Moore algorithm to find the starting sequence */
    /* when Boyer-Moore function returns, startOfFilePtr should point to the location within fileBuffer on which the startSequence is found
     * or be NULL if not present on fileBuffer */
    unsigned char *startOfFilePtr = boyer_moore(fileBuffer, srcFileSize, startSequence, startSequenceLength);
    /* check if sequence was found */
    if (startOfFilePtr == NULL)
    {
        printErrorStartSequenceNotFound();
        fclose(srcFilePtr);
        fclose(destFilePtr);
        free(fileBuffer);
        exit(-1);
    }
    /* now, find the offset between the start of the original file and the location of the graphics file start sequence */
    unsigned long sequenceOffset = startOfFilePtr - fileBuffer;

    /* third we start writing the contents of the memory buffer into the destination file, starting at the location where we found the start sequence */
    size_t writtenBytes = 0;
    while (writtenBytes < (srcFileSize - sequenceOffset))
    {
        writtenBytes += fwrite(startOfFilePtr, 1, srcFileSize - sequenceOffset, destFilePtr);
        /* this will make the loop break in the event that not all bytes have been written... would need some hardening, maybe allowing some retries before breaking */
        if (writtenBytes < (srcFileSize - sequenceOffset))
        {
            /* some sort of error happened during writing of the file, abort and exit */
            printErrorWritingFile();
            fclose(srcFilePtr);
            fclose(destFilePtr);
            free(fileBuffer);
            exit(-1);
        }
    }
    /* the we roll back to the beginning of the file and continue till we have covered the whole file */
    writtenBytes = 0;
    while (writtenBytes < sequenceOffset)
    {
        writtenBytes += fwrite(fileBuffer, 1, sequenceOffset, destFilePtr);
        /* this will make the loop break in the event that not all bytes have been written... would need some hardening, maybe allowing some retries before breaking */
        if (writtenBytes < sequenceOffset)
        {
            /* some sort of error happened during writing of the file, abort and exit */
            printErrorWritingFile();
            fclose(srcFilePtr);
            fclose(destFilePtr);
            free(fileBuffer);
            exit(-1);
        }
    }
    /* ensure all bytes are written in the file stream */
    fflush(destFilePtr);

    /* seems we are done de-shifting the file, now free temporary buffer */
    free(fileBuffer);

    return;
}


