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

int main(int argc, char *argv[])
{
    int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *hp;
 	int opt= 0;
	int port =-1;
	char* server=NULL;
   
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
                    panic("port argument is not a number?");
			    }
				else { port = atoi(optarg);}
				break;
			case 's' : server = optarg;
				       break;
			default: print_usage(); 
				 exit(EXIT_FAILURE);
		}
	}
	
	//Print server in screen
	if(server == NULL){
	    	printf("---- SERVER not provided \n");
		print_usage();
		exit(EXIT_FAILURE);
	}
	else {
		printf("SERVER: %s\n", server);
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
	
    printf(" Closing socket at client side\n");
    close(sockfd);

	//pause();
    return 0;

}
