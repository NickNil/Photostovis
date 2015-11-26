/**
  * \file SyncManager.c
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BackupFileContent
{
  const char* pathHash;
  const char* fileHash;
  const char* filePath;
};

/**
 * @brief Reads the number of lines in a file
 * @param fp : File pointer that needs to read
 * @return number of lines
 */
unsigned int photostovis_read_number_of_lines_in_file(FILE* fp);

/**
 * @brief Reads local or server backup file
 * @param backupFilePath
 */
void photostovis_read_backup_file(char* backupFilePath, struct BackupFileContent*);

/**
 * @brief Syncronizes files between client and server
 */
void photostovis_sync_files_to_server();

/**
 * @brief Checks the difference between two backup files
 * @param client
 * @param clientLength
 * @param server
 * @param serverLength
 * @param result : Files that will be uplaoded to server
 */
int photostovis_client_server_backup_diff(struct BackupFileContent* client,
                                                                unsigned int clientLength,
                                                                struct BackupFileContent* server,
                                                                unsigned int serverLength,
                                                                struct BackupFileContent* result,
                                                                int getResult);

/**
 * @brief Comma-seperator
 * @param line : String that will be split on delimiter
 * @param num : array index
 * @return
 */
const char* photostovis_backup_file_getfield(char* line, int num);
