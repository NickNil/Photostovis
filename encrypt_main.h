#ifndef encrypt_main_h
#define encrypt_main_h

int file_exists(char *filename);
void empty_dir(char *path);
void createfilename(char *filename);
void save_file(char *dir, char *filename, char *pic_file); 
int photostovis_encrypt_files(char **unsynced_files, char **encryptedfilepath, int numberoffiles);
void photostovis_run_encryption(char **added_files, char **encryptedfilepath, int numberoffiles);
int  photostovis_check_duplicate_entry(char **unsynced_files, char **encryptedfilepath, int numberoffiles);
void photostovis_read_newfiles(char *newfilelist);

#endif
