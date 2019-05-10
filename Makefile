CC=gcc
CFLAGS=-I

all: fbtest.c
	$(CC) -o fbtest fbtest.c 

fbtest: fbtest.c
	$(CC) -o fbtest fbtest.c

clean:
	rm -rf fbtest
