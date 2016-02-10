CC = gcc
CFLAGS = -g -Wall
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2
EXEC = parsexml

all: $(EXEC)

parsexml: parsexml.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
