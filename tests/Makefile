#C flags 
CFLAGS=-Wall -g -DUNIT_TESTING 

CC=gcc $(CFLAGS)

all: client_test 

#client: client.c Makefile
#	$(CC) -o client client.c

client_test: ../client.c client_test.c Makefile
	$(CC) client_test.c -c -o client_test.o
	$(CC) ../client.c client_test.o -o client_test

#server: server.c Makefile
#	$(CC) -o server server.c

clean:
	rm -f client_test *~ *.o

