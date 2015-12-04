/*
 * Global Software Development
 *
 * Project - Photostovis
 *
 * 2015-10-10
 * Server Program to create TCP socket and get images from client
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/socket.h> /* socket api */
#include <netinet/in.h> /* inetaddr stucts */
#include <netdb.h> /* gethostbyname() */
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <ctype.h>
#include <sha256.h>

void print_usage() {
    printf("---- Usage: ./server [--port [port]] \n");
}

void panic(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// function to send acknowledgement to client
int sendp(int sockfd, char* message) {

    // Calculates message length
    int data_len = strlen(message);
    //printf(" Data Length = %zu\n", strlen(message));

    // TCP Message length
    int tcp_msg_len;
    tcp_msg_len = 2 + data_len;
    //printf(" TCP Message Length = %d\n", tcp_msg_len);
    unsigned char dumpbuffer[256];
    bzero(dumpbuffer, 256);

    uint16_t packet_len_buf = htons(tcp_msg_len);
    unsigned char buffer[256];
    bzero(buffer, 256);
    memcpy((char *)&buffer[0], (char *) &packet_len_buf, sizeof(uint16_t));
    memcpy(&buffer[2], message, data_len);
    int n = write(sockfd,buffer,tcp_msg_len);
    if (n < 0) {
        perror("Write Error"); //panic("Write Error");
        return -1;
    }

    return 0;
}

// function to read from client
char *readp(int sockfd) {
    int received = 0;
    char *rec_message;
    unsigned char resbuf[2];
    uint16_t rec_packet_len;
    bzero(resbuf, 2);
    received = read(sockfd,resbuf, 2);
    if (received < 0) {
        perror("ERROR reading from socket");//panic("ERROR reading from socket");
        return "-1";
    }
    memcpy((char *) &rec_packet_len, (char *) &resbuf, sizeof(uint16_t));
    rec_packet_len = ntohs(rec_packet_len);
    uint16_t tcp_packet_len = rec_packet_len;
    //printf("Received TCP message length = %d\n", tcp_packet_len);

    rec_message = (char *) malloc(256);
    bzero(rec_message, 256);

    unsigned char randombuf[256];
    bzero(randombuf, 256);
    received = read(sockfd,randombuf, tcp_packet_len - 2);
    if (received < 0) {
        perror("ERROR reading from socket");//panic("ERROR reading from socket");
        return "-1";
    }
    uint16_t rec_msg_len = tcp_packet_len -2;

    // change that to use memcpy
    unsigned int i;
    for (i=0; i<rec_msg_len; i++){
        rec_message[i] = randombuf[i];
    }
    //rec_message[rec_msg_len] = '\0';
    //printf(" Received Message length = %d\n", rec_msg_len);
    printf(" Message Received from client = %s\n", rec_message);
    return rec_message;
}

int listen_for_connection(int sockfd, char argv0[], int port){
    int pid, sent_file = 0;
    struct sockaddr_in client_addr;
    // -- start accepting incoming connections
    listen(sockfd,5);
    printf("%s listening on port: %d\n", argv0, port);

    // -- accept one incoming connection
    socklen_t client_len = sizeof(client_addr);

    while(1)
    {
        sleep(1);
        int newsockfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_len);

        send_file(newsockfd);


        if( newsockfd < 0 ) //{ perror("accept()"); panic("accept()"); }
        {
            perror("accept()");
        }
        else
        {
            printf("Connection Established\n");
        }

        /* Create child process */
        pid = fork();

        if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
        }

        if (pid == 0) {
         /* This is the client process */
         close(sockfd);
         client_process(newsockfd);
         exit(0);
        }
        else {
         close(newsockfd);
        }
    }

    return 1;

}

void client_process(int socket)
{
    uint32_t images = 0;

    read(socket, &images, sizeof(uint32_t));
    images = ntohl(images);
    printf("number of images to be received: %d\n", images);
    int i = 0;
    if (images == 0)
    {
        printf("server is up to date\n");
        exit(0);
    }
    else
    {
        while(i < images)
        {
            printf("picture: %d\n\n", i+1);
            if(socket >= 0)
            {
                receive_image(socket);
            }

            i++;
        }
    }



    close(socket);
    exit(0);
}

//function for sending a file
int send_file(int socket)
{
    FILE *text_file;
    int file_size, packet_size = 0, read_size = 0;
    char send_buffer[256];
    struct stat file_stat;

    text_file  = fopen("/home/global-sw-dev/Photostovis/server-backup.txt", "r");

    if(text_file == NULL)
    {
        printf("error opening file\n");
        return -1;
    }

    //finding file size
    if (stat("/home/global-sw-dev/Photostovis/server-backup.txt", &file_stat) < 0)
    {
        printf("error with file stat: %s", strerror(errno));
        return -1;
    }
    file_size = (int)file_stat.st_size;

    printf("file size: %d bytes\n", file_size);

    //sending file size
    if(write(socket, (void *)&file_size, sizeof(int)) < 0)
    {
        printf("error sending file size: %s", strerror(errno));
        return -1;
    }

    //sending file
    while(!feof(text_file))
    {
        read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, text_file);

        do{
            packet_size = write(socket, send_buffer, read_size);
        }while(packet_size < 0);

        //printf("packet number: %i\n", packet_index);
          //printf("packet size sent: %i\n", read_size);
           //printf("file size  = %i\n", file_size);

        bzero(send_buffer, sizeof(send_buffer));
    }

    printf("file hopefully successfully sent");



    return 0;
}

//function to receive an image, only use if certain an image is to be sent
void receive_image(int socket)
{
    int packet_size = 0, receive_size = 0;
    uint32_t image_size = 0;
    int read_size, write_size;
    int packet_index = 1;

    char image_name[256];
    char image_path[256];
    char *rec_image_path;
    char* base_path = "/home/global-sw-dev/Photostovis/backup-pictures/";
    char new_path[1000];
    char new_path_cpy[1000];
    char pict_array[10241];
    FILE *picture;

    time_t current_time;
    char* c_time_string;

    bzero(pict_array, 10241);
    bzero(image_name, 256);
    bzero(image_path, 256);
    bzero(new_path, 1000);
    bzero(new_path_cpy, 1000);

    rec_image_path = readp(socket);
    printf("\nimage path: %s\n", rec_image_path);

    strcpy(image_path, rec_image_path);
    free(rec_image_path);
    printf("copied image path: %s\n", image_path);

    strcpy(image_name, basename(image_path));

    packet_size = 0;


    packet_size = read(socket, &image_size, sizeof(uint32_t)); //reading image size

    image_size = ntohl(image_size); //converting to host byte order

    strcpy(new_path, base_path);
    strcat(new_path, image_path);
    strcpy(new_path_cpy, new_path);
    make_folders(new_path_cpy);

    printf("\nnew path: %s\n", new_path);

    printf("\nSAVING TO PATH: %s", new_path);



    picture = fopen(new_path, "w");
    if (picture == NULL)
    {
        printf("ERROR LOADING IMAGE");
        return;
    }



    struct timeval timeout = {10,0};

    fd_set fds;
    int buffer_fd;

    printf("image name: %s\n", image_name);


    while(receive_size < image_size) //loop until entire image is received
    {
        FD_ZERO(&fds);
        FD_SET(socket, &fds);

        buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);

        if(buffer_fd < 0)
        {
            printf("error: bad file descriptor\n");
        }

        if(buffer_fd == 0)
        {
            printf("error: buffer read timed out\n");
        }

        if(buffer_fd > 0)
        {
            do
            {
                read_size = read(socket, pict_array, 10241);
            }
            while (read_size < 0);

            write_size = fwrite(pict_array, 1, read_size, picture); //write picture to file

            if(read_size != write_size)
            {
                printf("error in read write operation\n");
            }


            //printf("packet size: %i\n", read_size);




            receive_size += read_size;
            packet_index++;


        }


    }
    printf("packet number received: %i\n", packet_index);
    printf("image size = %i\n", image_size);
    printf("total received image size: %i\n\n", receive_size);


    fclose(picture);

    char hash_content[65];
    hash_received_file_content_on_server(new_path, hash_content);
    char hash_path[65];
    hash_received_file_path_on_server(new_path, hash_path);

    write_to_backup_file_on_server(new_path, hash_path, hash_content);

    printf("image (hopefully) successfully received :D\n\n");
}

void make_folders(char *path)
{
    struct  stat st = {0};
    char dir[80];
    char dir_cpy[80];

    bzero(dir, 80);
    bzero(dir_cpy, 80);
    //printf("\npath in make_folders: %s \n", path);

    strcpy(dir, dirname(path));

    printf("dir: %s\n ", dir);

    if (stat(dir, &st) == -1) //directory does not exist
    {
        strcpy(dir_cpy, dir);
        make_folders(dir_cpy);
        mkdir(dir, 0777);
        printf("made folder %s\n", dir);
    }

    return;

}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in server_addr;
    int opt= 0;
    int port =-1;

    // Option Parsing using getopt
    static struct option long_options[] = {
        {"port",       required_argument, 0,  'p' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;
    while ((opt = getopt_long_only(argc, argv,"",
                    long_options, &long_index )) != -1) {
        switch (opt) {
            case 'p' :
                if(!isdigit(optarg[0])){
                    panic("port argument is not a number?");
                }
                else {
                    port = atoi(optarg);}
                break;
            default: print_usage();
                     exit(EXIT_FAILURE);
        }
    }


    //Print port number in screen
    if (port == -1) {
        printf("---- SERVER PORT not provided\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    else {
        printf("SERVER PORT: %d\n", port);
    }

    // -- initialize a TCP/IP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 ) {
        perror("socket()"); panic("socket()");
    }

    // -- listening socket struct
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if( bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0 ) {
        perror("bind()"); panic("bind()");
    }

    int newsockfd;
    int file_sent = 0;
    int test = 0;

    newsockfd = listen_for_connection(sockfd, argv[0], port);
    /*while(test < 10)
    {
        newsockfd = listen_for_connection(sockfd, argv[0], port);
        printf("\ntestrun: %d\n\n", test);
        if(newsockfd >= 0)
        {
            if (file_sent == 0)
            {
                send_file(newsockfd);
                file_sent++;
            }

            receive_image(newsockfd);
            close(newsockfd);
        }
        test++;
    }*/


    // here communication would be possible using 'newsockfd'
    printf(" Closing socket at Server side\n");
    close(sockfd);

    //pause();
    return 0;

}

void hash_received_file_content_on_server(char* fullpath, char output[65])
{
    FILE* file = fopen(fullpath, "rb");
    if(!file) return -1;

    unsigned char hash[SHA256_BLOCK_SIZE];
    SHA256_CTX sha256;
    sha256_init(&sha256);
    const int bufSize = 32768;
    char* buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        sha256_update(&sha256, buffer, bytesRead);
    }
    sha256_final(&sha256, hash);

    sha256_hash_string(hash, output);
    fclose(file);
    free(buffer);
    return 0;
}

void hash_received_file_path_on_server(char* path, char output[65])
{
    unsigned char buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, path, strlen(path));
    sha256_final(&ctx, buf);

    sha256_hash_string(buf, output);
}

void sha256_hash_string (char hash[SHA256_BLOCK_SIZE], char outputBuffer[65])
{
    int i = 0;

    for(i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", (unsigned char)hash[i]);
    }

    outputBuffer[64] = 0;
}

void write_to_backup_file_on_server(char* path, char hash_path[65], char hash_file[65])
{
    char* backup_path = "/home/global-sw-dev/Photostovis/server-backup.txt";
    FILE* file = fopen(backup_path, "a");
    fprintf(file,"%s",path);
    fprintf(file,"%s",",");
    fprintf(file,"%s",hash_path);
    fprintf(file,"%s",",");
    fprintf(file,"%s",hash_file);
    fprintf(file,"%s","\n");
    fclose(file);
}
