CC=gcc
CFLAGS=-I

all: fbtest.c
	$(CC) -o fbtest fbtest.c 

fbtest: fbtest.c
	$(CC) -o fbtest fbtest.c

install:
	-D -m755 fbtest "$(DISTDIR)"/bin/fbtest

clean:
	rm -rf fbtest
