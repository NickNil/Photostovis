#include <stdio.h>
#include <stdlib.h>

/* This function will initialize seed for random number generation.
 * This function should only be called once at the beginning of the program,
 * as part of the initialization of libraries.
 * This function takes no input parameters and returns nothing.
*/
void scramblerInitialize(void);

/*
 * This function will take care of the initialization of buffers used for
 * the purpose of calculating scrambling keys and scrambling itself, since we don't
 * have guarantees that the compiler will do it for us.
 * Function takes two input parameters and does not return anything.
 * Param1:  bufferPtr, pointer to the aray of memory to be initialized
 * Param2:  bufferLen, length (in char/bytes) of the memory area to be initialized
 * Param3:  offset, offset (in bytes) from the start of the buffer before starting the zeroing
*/
void zeroCharBuffer(unsigned char *bufferPtr, unsigned int bufferLen, unsigned int offset);

/*
 * This function is responsible of the actual bitwise XORing of the contents of dstBuffer with
 * with that of srcBuffer, then storing the results in dstBuffer.
 * NOTE1: previous contents of dstBuffer will be lost after calling this function!
 * NOTE2: make sure that both srcBuffer and dstBuffer have the same length, or at lest that dstBuffer size is
 *        longer than that of srcBuffer, since only bufferLen bytes will be processed
 * Function takes three input parameters and does not return anything.
 * Param1:  srcBufferPtr, pointer to the source buffer with the first array of bytes to be used for XORing
 * Param2:  dstBufferPtr, pointer to the detination buffer with the second array of bytes to be used for XORing
 *          and the destination where the resulting sequence of bytes will be stored
 * Param3:  bufferLen, length of the buffers to be XORed
*/
void xorCharBuffers(unsigned char *srcBufferPtr, unsigned char *dstBufferPtr, unsigned int bufferLen);

/*
 * This function will scan through the whole file, in chunks of keyLen size, xoring all chunks to obtain the
 * scrambling key and store it in keySequence buffer.
 * NOTE1: filePtr must be a valid FILE pointer to an already opened file
 * NOTE2: keySequence must point to an already allocated area of memory of size (in bytes) keyLen
 * Function takes three input paramters and does not return anything.
 * Param1:  filePtr, pointer to a valid and already opened file whose contents will be used to compute the key
 * Param2:  keySequence, pointer to the buffer (already allocated before the call to this function) on which the computed key shall be stored
 * Param3:  keyLen, size of the key to be computed (in bytes) and also the size of the buffer to hold the computed key (keySequence)
*/
void computeKeyFromFile(FILE *filePtr, unsigned char *keySequence, unsigned int keyLen, int initialize);

/* This is the function that actually scrambles/descrambles the file contents by XORing its contents with the provided
 * keySequence
 * This function has four input paramters and returns void
 * Param1:  scrFilePtr, handle to an already open (read-only) file whose contents will be XORed
 * Param2:  dstFilePtr, handle to an already open (write) file on which the scrambled contents of srcFilePtr will be written
 * Param2:  keySequence, pointer to an already allocated memory that contains the key to be used for scrambling
 * Param3:  keyLen, length of the key to be to be used, which shall be the same as the lenght of buffer pointed by Param3
 * Param4:  intialize, int to indicate if the keySequence buffer should be initialized <> 0 (first file) or if it already contains key information that should be kept ==0
*/
void scrambleFileContents(FILE *srcFilePtr, FILE *destFilePtr, unsigned char *keySequence, unsigned int keyLen);

/*
 * This function will scan the set of files passed to it (number given by parameter numFiles) and calculate
 * a scrambling key based on the contents of all these files. The result will be stored in keySequence buffer
 * at the end of the procedure.
 * The function takes four parameters and returns nothing.
 * Param1:  filePtr, an array of pointers to already opened files that will be used to generate the scrambling key.
 * Param2:  numFiles, number of entries of filePtr array
 * Param3:  keySequence, char array (which must be already initialized when calling this function) that will hold the computed key at the end of the procedure
 * Param4:  keyLen, length (in bytes) of the key to be computed
*/
void computeKeyFromFiles(FILE **filePtr, int numFiles, unsigned char *keySequence, unsigned int keyLen);

/* This function will shift the contents of the input file by a random number of bytes, then stored the resulting byte sequence
 * into the output file. The size of both input and output files will be the same, and the contents of the fileswill be copied as
 * is the source file was a circular buffer (bytes at the end of the source file will be pegged at the start of the output file.
 * The fuction takes two input parameters and returns none.
 * Param1:  srcFilePtr, pointer to an already opened file with read access
 * Param2:  dstFilePtr, pointer to an already opened file with write access
*/
void shiftFileContents(FILE *srcFilePtr, FILE *dstFilePtr);

/* This function will de-shift the contents of the input file by searching within it the char sequence "startSequence"
 * and then creating a new file whose contents will be those of the input file "srcFile" but starting at the position at
 * which "startSequence" was found, then wraping around the end of the file and copying the first chunk of "srcFile"
 * (up to the position of "startSequence") at the end of the resulting file.
 * This function takes four input parameters and returns none.
 * Param1:  srcFilePtr, pointer to an already opened file with read access
 * Param2:  destFilePtr, pointer to an already opened file with write access
 * Param3:  startSequence, pointer to a buffer containing the char sequence to look for in srcFile
 * Param4:  startSequenceLength, integer containing the length of the sequence to look for
*/
void deShiftFileContents(FILE *srcFilePtr, FILE *destFilePtr, unsigned char *startSequence, unsigned long startSequenceLength);
