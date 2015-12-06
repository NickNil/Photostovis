#ifndef encrypt_main_h
#define encrypt_main_h

int file_exists(char *filename);
void empty_dir(char *path);
void createfilename(char *filename);
void save_file(char *filename, char *pic_file); 
void photostovis_encrypt_files();
void photostovis_run_encryption(char **added_files, int numberoffiles);
void photostovis_read_newfiles(char *newfilelist);

#endif
