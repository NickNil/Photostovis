#ifndef client_h
#define client_h

#define array_length(array) (sizeof(array)/sizeof(array[0]))

extern void print_usage();
extern void panic(const char *msg);
extern void sendp(int sockfd, char* message);
extern void readp(int sockfd);
extern int original_main(int argc, char *argv[]);
int photostovis_connect_to_server(char* server, int port);

#endif
