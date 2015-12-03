/*
 * Global Software Development
 *
 * Project - Photostovis
 *
 * 2015-10-02
 * Client Program to create TCP socket and send images to server
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h> /* socket api */
#include <netinet/in.h> /* inetaddr stucts */
#include <netdb.h> /* gethostbyname() */
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <ctype.h>
#include "client.h"

#define unit_testing 0

#if UNIT_TESTING
#define main original_main
#ifdef unit_testing
#undef unit_testing
#define unit_testing 1
#endif
#endif

int original_main(int argc, char *argv[]);

void print_usage() {
    printf("---- Usage: ./client [--server [address]] [--port [port]] \n");
}

void panic(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// function to send command to server
void sendp(int sockfd, char* message) {

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
        perror("Write Error"); panic("Write Error");
    }
}

// function to read from server
void readp(int sockfd) {
    int received = 0;
    char rec_message[256];
    unsigned char resbuf[2];
    uint16_t rec_packet_len;
    bzero(resbuf, 2);
    received = read(sockfd,resbuf, 2);
    if (received < 0) {
        perror("ERROR reading from socket");panic("ERROR reading from socket");
    }
    memcpy((char *) &rec_packet_len, (char *) &resbuf, sizeof(uint16_t));
    rec_packet_len = ntohs(rec_packet_len);
    uint16_t tcp_packet_len = rec_packet_len;
    //printf("Received TCP message length = %d\n", tcp_packet_len);

    unsigned char randombuf[256];
    bzero(randombuf, 256);
    received = read(sockfd,randombuf, tcp_packet_len - 2);
    if (received < 0) {
        perror("ERROR reading from socket");panic("ERROR reading from socket");
    }
    uint16_t rec_msg_len = tcp_packet_len -2;
    unsigned int i;
    for (i=0; i<=rec_msg_len; i++){
        rec_message[i] = randombuf[i];
    }
    //printf(" Received Message length = %d\n", rec_msg_len);
    printf(" Received %s message from Server\n", rec_message);
}

int receive_file(int socket)
{
    int file_size = 0, packet_size = 0, receive_size = 0, read_size = 0, write_size = 0;

    char buffer[256];
    FILE *received_file;

    while(packet_size == 0)
    {
            packet_size = read(socket, &file_size, sizeof(int));
    }

    printf("packet received\n");
    //printf("packet size = %i\n", packet_size);
    //printf("image size = %i\n", file_size);

    received_file = fopen("/home/global-sw-dev/Photostovis/received_backup.txt", "w");

    while(receive_size < file_size)
    {
        do{
            read_size = read(socket, buffer, 256); //receive file data
        }while (read_size < 0);

        write_size = fwrite(buffer, 1, read_size, received_file); //write data to file

        if(read_size != write_size){
            printf("error in read write operation\n");
        }

       // printf("packet number received: %i\n", packet_index);
        printf("packet size: %i\n", read_size);
        printf("written file size: %i\n", write_size);
        printf("file size = %i\n", file_size);

        receive_size += read_size;
        //packet_index++;
        printf("total received file size: %i\n\n", receive_size);

    }
    printf("file (hopefully) received successfully :D\n");
    fclose(received_file);

    return 0;
}

void send_image(int socket, char* imagepath, char* const basePath)
{
    FILE *picture;
    time_t current_time;
        char* c_time_string;
    int size, packet_size, read_size, packet_index = 1;
    uint32_t htonl_size = 0;
    char send_buffer[10240];
    char *image_name;
    char *image_dir;
    packet_index = 1;

    bzero(send_buffer, 10240);

    picture = fopen(imagepath, "r");

    if(picture == NULL)
    {
        printf("error opening image\n");
    }

    printf("Sending image: %s\n", image_name);

    image_dir = (char *) malloc(strlen(imagepath) - strlen(basePath) + 1);

    strcpy(image_dir, &imagepath[strlen(basePath)]);

    printf("inner folders: %s\n", image_dir);

    sendp(socket, image_dir); //sending image path

    free(image_dir);

    fseek(picture, 0, SEEK_END);
    size = ftell(picture);			//getting image size
    fseek(picture, 0, SEEK_SET);
    printf("picture size  = %i\n", size);
    htonl_size = htonl(size); //converting size to network byte order

    write(socket, &htonl_size, sizeof(uint32_t)); //sending image size
    int sent = 0;

    current_time = time(NULL);

    // Convert to local time format.
    c_time_string = ctime(&current_time);

    printf("start time is %s\n\n", c_time_string);

    //sending image as a byte array
    while(!feof(picture))
    {
        read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);

        do{
            packet_size = write(socket, send_buffer, read_size);
        }while(packet_size < 0);


        //printf("packet size sent: %i\n", read_size);

        sent += read_size;



        packet_index++;
        bzero(send_buffer, sizeof(send_buffer));
    }
    printf("packet number: %i\n", packet_index);
    printf("picture size  = %i\n", size);
    printf("sent: %i\n", sent);
    current_time = time(NULL);

    // Convert to local time format.
    c_time_string = ctime(&current_time);

    printf("sent picture time is %s\n\n", c_time_string);

    fclose(picture);
}

int photostovis_connect_to_server(char* srv, int prt)
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *hp;
    int opt= 0;
    int port = prt;
    char* server= srv;

    //int i;
    //printf("%d\n", array_length(argv));
    //for(i=0; i<array_length(argv); i++)
    //{
    //    printf("%s\n", argv[i]);
    //}

/*
    //
    // Option Parsing using getopt
    //
    static struct option long_options[] = {
        {"port",       required_argument, 0,  'p' },
        {"server",     required_argument, 0,  's' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;
    while ((opt = getopt_long_only(argc, argv,"",
                    long_options, &long_index )) != -1) {
        switch (opt) {
            case 'p' :
                if(!isdigit(optarg[0])){
                    if(unit_testing==1) return -1;
                    //panic("port argument is not a number?");
                    printf("Port argument is not a number\n");
                    exit(EXIT_FAILURE);
                }
                else { port = atoi(optarg);}
                break;
            case 's' : server = optarg;
                       break;
            default: print_usage();
                     if(unit_testing==1) return 1;
                     exit(EXIT_FAILURE);
        }
    }

    //Print server in screen
    if(server==NULL && port==-1){
        printf(" --Arguments not provided\n");
        if(unit_testing==1) return 1;
    }
    else if(server == NULL){
        printf("---- SERVER not provided \n");
        print_usage();
        if(unit_testing==1) return 1;
        exit(EXIT_FAILURE);
    }
    else {
        printf("SERVER: %s\n", server);
    }

    //Print port number in screen
    if (port == -1) {
        printf("---- SERVER PORT not provided\n");
        print_usage();
        if(unit_testing==1) return 1;
        exit(EXIT_FAILURE);
    }
    else {
        printf("SERVER PORT: %d\n", port);
        if(unit_testing==1) return 0;
    }

*/
    //
    // Create TCP/IP Socket
    //
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 ) {
        perror("socket()");
    }
    // -- init destination address struct
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if ((hp = gethostbyname(server)) == NULL) {
        perror("gethostbyname()");
    }
    memcpy((char *) &server_addr.sin_addr, (char *) hp->h_addr, hp->h_length);

    // make the connection to the server
    if( connect(sockfd, (struct sockaddr *) &server_addr,
                sizeof(server_addr) ) < 0 ) {
        perror("connect()");panic("Connection Error");
    }
/*
    printf("Enter HELLO message to server\n");
    printf(">> ");
    char hello_message[256];
    fgets(hello_message, 256, stdin);
    if ((strlen(hello_message)>0) && (hello_message[strlen(hello_message) - 1] == '\n')) {
        hello_message[strlen(hello_message) - 1] = '\0';
    }
    //printf("%s\n", hello_message);
    sendp(sockfd, hello_message);
    readp(sockfd);
*/
    //readp(sockfd);

    //receive_file(sockfd);

    //sockt = sockfd;
    //send_image(sockfd, "/home/global-sw-dev/Photostovis/image-03.jpg");
    //printf(" Closing socket at client side\n");
    //close(sockfd);

    //pause();
    return sockfd;

}

