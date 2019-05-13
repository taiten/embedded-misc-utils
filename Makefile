CC=gcc
CFLAGS=-I

all: fbtest.c hello.c
	$(CC) -o fbtest fbtest.c 
	$(CC) -o hello hello.c

fbtest: fbtest.c
	$(CC) -o fbtest fbtest.c

hell:
	$(CC) -o hello hello.c

install:
	install	-D -m755 fbtest "$(DESTDIR)"/bin/fbtest
	install -D -m755 hello "$(DESTDIR)"/bin/hello

clean:
	rm -rf "$(DESTDIR)"/bin/fbtest
	rm -rf "$(DESKDIR)"/bin/hello
