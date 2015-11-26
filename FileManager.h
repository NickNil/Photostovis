/**
  * \file FileManager.c
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sha256.h>
/**
 * @brief This function gets the filenames from the photo-folder on the client.
 * If multiple folders are located inside the photo-folder, the filenames are gathered from these as well.
 * @param currentPath : The current path for the photo-files.
 * @param currentPathLen : The current length of the path.
 */
void photostovis_get_filenames_from_client(char* const currentPath, const unsigned int currentPathLen);

/**
 * @brief Hashes the file content of the current file.
 * @param fullpath : The full path to the image-file.
 * @param output : A reference for file content hash.
 * @return : 0 if success, -1 if error.
 */
int photostovis_hash_file_content_on_client(char* fullpath, char output[65]);

/**
 * @brief Hashes the file path of the current file.
 * @param path : The path to the file.
 * @param output : A reference for file path hash.

 */
void photostovis_hash_file_path_on_client(char* path, char output[65]);

/**
 * @brief Converts HEX values to string.
 * @param hash : The hash in HEX values
 * @param outputBuffer : A reference to the hash as char.
 */
void sha256_hash_string (char hash[SHA256_BLOCK_SIZE], char outputBuffer[65]);

/**
 * @brief Writes the fullpath, filepath hash and filecontent hash to the backup.txt file on client.
 * Appends to this file.
 * @param path : The full path to the file.
 * @param hash_path : The hashed path to the file.
 * @param hash_file : The hashed file content.
 */
void photostovis_write_to_backup_file(char* path, char hash_path[65], char hash_file[65]);

/**
 * @brief Deletes the current content of the backup.txt file on client.
 */
void photostovis_clear_backup_file();
