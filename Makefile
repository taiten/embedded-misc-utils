CC=gcc
CFLAGS=-I
BUILDDIR=
INSTALLDIR=usr/bin/

all: fbtest.c
	mkdir -p $(BUILDDIR)
	$(CC) -o $(BUILDDIR)fbtest fbtest.c 

fbtest: fbtest.c
	mkdir -p $(BUILDDIR)
	$(CC) -o $(BUILDDIR)fbtest fbtest.c

install:
	cp $(BUILDDIR)* $(INSTALLDIR)

clean:
	rm -rf $(BUILDDIR)
