CC=gcc
CFLAGS=-I
INSTALLDIR=usr/bin/

all: fbtest.c
	$(CC) -o fbtest fbtest.c 

fbtest: fbtest.c
	$(CC) -o fbtest fbtest.c

install:
	cp fbtest $(INSTALLDIR)

clean:
	rm -rf fbtest
