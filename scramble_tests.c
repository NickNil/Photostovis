#include "scramble_tests.h"
#include "scrambler.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define BUFFLEN 1024
#define BUFFOFF 256
#define FILENAME1 "tempFile1"
#define FILENAME2 "tempFile2"

int zeroCharBuffer_tests()
{
    unsigned char *testBuff;
    int buffIndex;

    printf("/*******************************************************/\n");
    printf("/****************ZEROCHARBUFFER TESTS*******************/\n");
    printf("/*******************************************************/\n");
    printf("Test number 1: zeroing a 1024 bytes long buffer\n");

    /* allocate BUFFLEN bytes of memory to testBuff */
    testBuff = malloc(BUFFLEN);

    /* have this thing abort execution if no memory is assigned */
    assert(testBuff != NULL);

    /* initialize buff to some non-zero value */
    for(buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        testBuff[buffIndex] = 0x55;
    }

    /* call zeroCharBuffer with correct lengths and offset = 0 */
    zeroCharBuffer(testBuff, BUFFLEN, 0);

    /* now check that the buffer as correctly been zeroed */
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x00)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with zeroCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("Some of the contents of testBuff were NOT correctly initialized by zeroCharBuffer... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of testBuff were correctly initialized by zeroCharBuffer... TEST PASS\n");
    }

    printf("Test number 2: zeroing a 1024 bytes long buffer starting at offset position 256\n");

    /* no need to reallocate memory, we will reuse the previously allocated memory buffer, but we need to re-initialize it */
    for(buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        testBuff[buffIndex] = 0x55;
    }

    /* call zeroCharBuffer with correct lengths and offset = 256 */
    zeroCharBuffer(testBuff, BUFFLEN, BUFFOFF);

    /* now check that the buffer starting bytes have not been modified */
    for (buffIndex = 0; buffIndex < BUFFOFF; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x55)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with zeroCharBuffer */
    if (buffIndex < BUFFOFF)
    {
        printf("Offset parameter not correctly used by zeroCharBuffer... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("Offset parameter seems to be correctly applid by zeroCharBuffer... TEST ONGOING\n");
    }

    /* now check that the buffer ending bytes have actually been zeroed */
    for (buffIndex = BUFFOFF; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x00)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with zeroCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("Some of the contents of testBuff were NOT correctly initialized by zeroCharBuffer... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of testBuff were correctly initialized by zeroCharBuffer... TEST PASS\n");
    }

    free(testBuff);
    return 0;
}

int xorCharBuffer_tests()
{
    unsigned char *testBuff_A;
    unsigned char *testBuff_B;
    int buffIndex;

    printf("/*******************************************************/\n");
    printf("/****************XORCHARBUFFER TESTS********************/\n");
    printf("/*******************************************************/\n");
    printf("Test number 1: xoring two 1024 bytes long buffers: one buffer contains only 0x00, the other contains an arbitrary value: values should be that of the non-zeroed buffer after operation\n");

    /* allocate BUFFLEN bytes of memory to testBuff_A & testBuff_B */
    testBuff_A = malloc(BUFFLEN);
    testBuff_B = malloc(BUFFLEN);

    /* have this thing abort execution if no memory is assigned */
    assert((testBuff_A != NULL) && (testBuff_B != NULL));

    /* initialize buff to some non-zero value */
    for(buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        testBuff_A[buffIndex] = 0x55;
        testBuff_B[buffIndex] = 0x00;
    }

    /* call xorCharBuffer with correct lengths and offset = 0 */
    xorCharBuffers(testBuff_A, testBuff_B, BUFFLEN);

    /* now check that the destination buffer constains the correct XOR operation result */
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff_B[buffIndex] != 0x55)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("Some of the contents of testBuff_B were NOT correctly xored with those of testBuff_A by xorCharBuffer... TEST FAIL at %d\n", buffIndex);
        free(testBuff_A);
        free(testBuff_B);
        return -1;
    }
    else
    {
        printf("All contents of testBuff_B were correctly xored with the contents if testBuff_A by xorCharBuffer... TEST PASS\n");
    }

    printf("Test number 2: xoring two 1024 bytes long buffers: one buffer contains only 0xFF, the other contains an arbitrary value: values should be inverted from that of the non-zeroed buffer after operation\n");

    /* initialize buff to some non-zero value */
    for(buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        testBuff_A[buffIndex] = 0x55;
        testBuff_B[buffIndex] = 0xFF;
    }

    /* call xorCharBuffer with correct lengths and offset = 0 */
    xorCharBuffers(testBuff_A, testBuff_B, BUFFLEN);

    /* now check that the destination buffer constains the correct XOR operation result */
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff_B[buffIndex] != 0xAA)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("Some of the contents of testBuff_B were NOT correctly xored with those of testBuff_A by xorCharBuffer... TEST FAIL at %d\n", buffIndex);
        free(testBuff_A);
        free(testBuff_B);
        return -1;
    }
    else
    {
        printf("All contents of testBuff_B were correctly xored with the contents if testBuff_A by xorCharBuffer... TEST PASS\n");
    }


    free(testBuff_A);
    free(testBuff_B);
    return 0;
}

int computeKeyFromFile_tests()
{
    printf("/*******************************************************/\n");
    printf("/**************COMPUTEKEYFROMFILE TESTS*****************/\n");
    printf("/*******************************************************/\n");


    unsigned char *testBuff;

    /* allocate BUFFLEN bytes of memory to testBuff, which happens to be the key size */
    testBuff = malloc(BUFFLEN);
    /* have this thing abort execution if no memory is assigned */
    assert(testBuff != NULL);

    printf("Test 1: XORing the contents of a file with exactly the same size as the KEY, results should be the same as the file contents\n");

    /* open a file for writting in binary mode */
    FILE *tempFile = fopen(FILENAME1, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    unsigned char charToWrite = 0x55;

    /*now populate the file with some binary contents of size BUFFLEN */
    int buffIndex;
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        fwrite(&charToWrite, 1, 1, tempFile);
    }
    /* now flush stream and close file */
    fflush(tempFile);
    fclose(tempFile);

    /* then open file read-only and pass if to computeKeyFromFileFunction */
    tempFile = fopen(FILENAME1, "rb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    computeKeyFromFile(tempFile, testBuff, BUFFLEN, 1);

    /* now close file and verify contents of the testBuff */
    fclose(tempFile);

    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x55)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("The contents of the file were NOT correctly XORed together ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of file were correctly XOR for a file with the same size as the key... TEST PASS\n");
    }

    printf("Test 2: XORing the contents of a file with smaller size as the KEY, results should be the same as the file contents\n");

    /* open a file for writting in binary mode */
    tempFile = fopen(FILENAME1, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    charToWrite = 0x55;

    /*now populate the file with some binary contents of size BUFFLEN/2 */
    for (buffIndex = 0; buffIndex < BUFFLEN/2; buffIndex++)
    {
        fwrite(&charToWrite, 1, 1, tempFile);
    }
    /* now flush stream and close file */
    fflush(tempFile);
    fclose(tempFile);

    /* then open file read-only and pass if to computeKeyFromFileFunction */
    tempFile = fopen(FILENAME1, "rb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    computeKeyFromFile(tempFile, testBuff, BUFFLEN, 1);

    /* now close file and verify contents of the testBuff */
    fclose(tempFile);

    for (buffIndex = 0; buffIndex < BUFFLEN/2; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x55)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN/2)
    {
        printf("The contents of the file were NOT correctly XORed together ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of file were correctly XOR for a file with the same size as the key... TEST ONGOING\n");
    }

    for (buffIndex = BUFFLEN/2; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x00)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("The contents of the file were NOT correctly XORed together ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of file were correctly XOR for a file with the same size as the key... TEST PASSED\n");
    }

    printf("Test 3: XORing the contents of a file four times the size of the KEY, and with alternaing chunks of 0x55 and 0xAA\n(each of BUFFLEN size) as contents, results should be the same as the file contents\n");

    /* open a file for writting in binary mode */
    tempFile = fopen(FILENAME1, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    int loopCounter;
    charToWrite = 0x55;

    for (loopCounter = 0; loopCounter < 4; loopCounter++)
    {
        /*now populate the file with some binary contents of size BUFFLEN/2 */
        for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
        {
            fwrite(&charToWrite, 1, 1, tempFile);
        }
        charToWrite = charToWrite ^ 0xFF;
    }
    /* now flush stream and close file */
    fflush(tempFile);
    fclose(tempFile);

    /* then open file read-only and pass if to computeKeyFromFileFunction */
    tempFile = fopen(FILENAME1, "rb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    computeKeyFromFile(tempFile, testBuff, BUFFLEN, 1);

    /* now check if the computed key is all 0x00, which should be for the given file contents */
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x00)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("The contents of the file were NOT correctly XORed together ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of file were correctly XOR for a file with the same size as the key... TEST PASSED\n");
    }

    printf("Test 4: XORing the contents of a file four times and a half the size of the KEY, and with alternaing chunks of 0x55 and 0xAA (each of BUFFLEN size) as contents, results should be the same as the file contents\n");

    /* open a file for writting in binary mode */
    tempFile = fopen(FILENAME1, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    charToWrite = 0x55;

    for (loopCounter = 0; loopCounter < 4; loopCounter++)
    {
        /*now populate the file with some binary contents of size BUFFLEN/2 */
        for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
        {
            fwrite(&charToWrite, 1, 1, tempFile);
        }
        charToWrite = charToWrite ^ 0xFF;
    }
    /* now write the missing half */
    for (buffIndex = 0; buffIndex < BUFFLEN/2; buffIndex++)
    {
        fwrite(&charToWrite, 1, 1, tempFile);
    }
    /* now flush stream and close file */
    fflush(tempFile);
    fclose(tempFile);

    /* then open file read-only and pass if to computeKeyFromFileFunction */
    tempFile = fopen(FILENAME1, "rb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile != NULL);

    computeKeyFromFile(tempFile, testBuff, BUFFLEN, 1);

    /* now check if the first half of the key is 0x55 */
    for (buffIndex = 0; buffIndex < BUFFLEN/2; buffIndex ++)
    {
        if (testBuff[buffIndex] != 0x55)
        {
            break;
        }
    }
    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN/2)
    {
        printf("The contents of the file were NOT correctly XORed together (checking first half of key) ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }

    /* now check if last half of the computed key is all 0x00, which should be for the given file contents */
    for (buffIndex = BUFFLEN/2; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x00)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("The contents of the file were NOT correctly XORed together (second half of key) ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("All contents of file were correctly XOR for a file with the same size as the key... TEST PASSED\n");
    }

    /* free memory allocated for temporary buffer */
    free(testBuff);

    return 0;
}

int computeKeyFromFiles_tests()
{
    printf("/*******************************************************/\n");
    printf("/*************COMPUTEKEYFROMFILES TESTS*****************/\n");
    printf("/*******************************************************/\n");


    unsigned char *testBuff;

    /* allocate BUFFLEN bytes of memory to testBuff, which happens to be the key size */
    testBuff = malloc(BUFFLEN);
    /* have this thing abort execution if no memory is assigned */
    assert(testBuff != NULL);

    printf("Test 1: XORing the contents of two files with exactly the same contents and size as the KEY\n");

    /* open first file for writting in binary mode */
    FILE *tempFile1 = fopen(FILENAME1, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile1 != NULL);

    unsigned char charToWrite = 0x55;

    /*now populate the file with some binary contents of size BUFFLEN */
    int buffIndex;
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        fwrite(&charToWrite, 1, 1, tempFile1);
    }
    /* now flush stream and close file */
    fflush(tempFile1);
    fclose(tempFile1);

    /* open second file for writting in binary mode */
    FILE *tempFile2 = fopen(FILENAME2, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile2 != NULL);

    charToWrite = 0x55;

    /*now populate the file with some binary contents of size BUFFLEN */
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        fwrite(&charToWrite, 1, 1, tempFile2);
    }
    /* now flush stream and close file */
    fflush(tempFile2);
    fclose(tempFile2);


    /* then open files read-only and pass if to computeKeyFromFileFunction */
    tempFile1 = fopen(FILENAME1, "rb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile1 != NULL);
    tempFile2 = fopen(FILENAME2, "rb");
    assert(tempFile2 != NULL);

    FILE *filePtrs[2];
    filePtrs[0] = tempFile1;
    filePtrs[1] = tempFile2;

    computeKeyFromFiles(filePtrs, 2, testBuff, BUFFLEN);

    /* now close files and verify contents of the testBuff */
    fclose(tempFile1);
    fclose(tempFile2);

    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        if (testBuff[buffIndex] != 0x00)
            break;
    }

    /* now check if we stoped execution of the for loop before checking the complete buffer (if so then there is an issue with xorCharBuffer */
    if (buffIndex < BUFFLEN)
    {
        printf("The key was not correctly calculated ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        return -1;
    }
    else
    {
        printf("The key was correctly calculated... TEST PASS\n");
    }

    /* free memory allocated for temporary buffer */
    free(testBuff);

    return 0;
}

int scrambleFileContents_tests()
{
    printf("/*******************************************************/\n");
    printf("/*************SCRAMBLEFILECONTENTS TESTS****************/\n");
    printf("/*******************************************************/\n");

    unsigned char *testBuff;

    /* allocate BUFFLEN bytes of memory to testBuff, which happens to be the key size */
    testBuff = malloc(BUFFLEN);
    /* have this thing abort execution if no memory is assigned */
    assert(testBuff != NULL);

    printf("Test 1: scramble the contents of a file (four times the size of the key) with a given key\n");

    /* open first file for writting in binary mode */
    FILE *tempFile1 = fopen(FILENAME1, "wb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile1 != NULL);

    unsigned char charToWrite = 0x55;

    /*now populate the file with some binary contents of size BUFFLEN */
    int buffIndex;
    for (buffIndex = 0; buffIndex < BUFFLEN*4; buffIndex++)
    {
        fwrite(&charToWrite, 1, 1, tempFile1);
    }
    /* now flush stream and close file */
    fflush(tempFile1);
    fclose(tempFile1);

    /* initialiaze the key to some value */
    for (buffIndex = 0; buffIndex < BUFFLEN; buffIndex++)
    {
        testBuff[buffIndex] = 0xAA;
    }

    /* now open the initial file for reading */
    tempFile1 = fopen(FILENAME1, "rb");
    /*have this thing abort execution if no file was opened */
    assert(tempFile1 != NULL);

    /* and a second file for writing on the results of the scrambling */
    FILE *tempFile2 = fopen(FILENAME2, "rb");
    assert(tempFile2 != NULL);

    /* call the scrambling function */
    scrambleFileContents(tempFile1, tempFile2, testBuff, BUFFLEN);

    /* flush destination file */
    fflush(tempFile2);
    /* close both source and destination files */
    fclose(tempFile1);
    fclose(tempFile2);

    /* now open destination file for reading */
    tempFile2 = fopen(FILENAME2, "rb");
    assert(tempFile2 != NULL);

    unsigned char readByte;
    /* now check that the contents of the file are correctly scrambled */
    while (feof(tempFile2) == 0)
    {
        fread(&readByte, 1, 1, tempFile2);
        if (readByte != 0xFF)
        {
            break;
        }
    }

    if (feof(tempFile2) == 0)
    {
        printf("The file was not correctly scrambed ... TEST FAIL at %d\n", buffIndex);
        free(testBuff);
        fclose(tempFile2);
        return -1;

    }
    else
    {
        printf("The file was correctly scrambled ... TEST PASS\n");
    }
    /* close file */
    fclose(tempFile2);
    /* free memory */
    free(testBuff);

    return 0;
}
