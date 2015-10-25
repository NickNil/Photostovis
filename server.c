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
#include <sys/socket.h> /* socket api */
#include <netinet/in.h> /* inetaddr stucts */
#include <netdb.h> /* gethostbyname() */
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <ctype.h>

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
int readp(int sockfd) {
    int received = 0;
    char rec_message[256];
    unsigned char resbuf[2];
    uint16_t rec_packet_len;
    bzero(resbuf, 2);
    received = read(sockfd,resbuf, 2);
    if (received < 0) {
        perror("ERROR reading from socket");//panic("ERROR reading from socket");
		return -1;
    } 
    memcpy((char *) &rec_packet_len, (char *) &resbuf, sizeof(uint16_t));
    rec_packet_len = ntohs(rec_packet_len);
    uint16_t tcp_packet_len = rec_packet_len;
    //printf("Received TCP message length = %d\n", tcp_packet_len);
    
    unsigned char randombuf[256];
    bzero(randombuf, 256);
    received = read(sockfd,randombuf, tcp_packet_len - 2);
    if (received < 0) {
        perror("ERROR reading from socket");//panic("ERROR reading from socket");
		return -1;
    } 
    uint16_t rec_msg_len = tcp_packet_len -2;
    unsigned int i;
    for (i=0; i<=rec_msg_len; i++){
        rec_message[i] = randombuf[i];
    }
    //printf(" Received Message length = %d\n", rec_msg_len);
    printf(" Message Received from client = %s\n", rec_message);
	return 0;
}

int listen_for_connection(int sockfd, char argv0[], int port){
    
    struct sockaddr_in client_addr;
    // -- start accepting incoming connections
    listen(sockfd,5);
    printf("%s listening on port: %d\n", argv0, port);

    // -- accept one incoming connection
    socklen_t client_len = sizeof(client_addr);

    int newsockfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_len);
    if( newsockfd < 0 ) //{ perror("accept()"); panic("accept()"); }
    {
        perror("accept()");
    }
    else{
        printf("Connection Established\n");
    }
    return newsockfd;

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
    while(1){
        newsockfd = listen_for_connection(sockfd, argv[0], port);
        if(newsockfd >= 0){
            printf(" Waiting for Hello message from client\n");
            if(readp(newsockfd) >= 0){
                printf(" Sending ACK message to client\n");
                sendp(newsockfd, "ACK");
            }
        }

    }
    //printf("%s\n",inet_ntoa(server_addr.sin_addr));
    //printf("%s\n",inet_ntoa(client_addr.sin_addr));

    // here communication would be possible using 'newsockfd'
    //printf(" Closing socket at Server side\n");
    close(sockfd);

    //pause();
    return 0;

}
