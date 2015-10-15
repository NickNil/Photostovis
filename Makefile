#C flags 
CFLAGS=-Wall -g

CC=gcc $(CFLAGS)

all: client server 

client: client.c Makefile
	$(CC) -o client client.c

server: server.c Makefile
	$(CC) -o server server.c

clean:
	rm -f client server *~ *.o

