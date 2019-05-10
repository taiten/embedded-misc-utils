CC=gcc
CFLAGS=-I

all: fbtest.c
	$(CC) -o fbtest fbtest.c 

fbtest: fbtest.c
	$(CC) -o fbtest fbtest.c

install:
	install	-D -m755 fbtest "$(DESTDIR)"/bin/fbtest

clean:
	rm -rf fbtest
