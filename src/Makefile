CC = gcc
CFLAGS = -g -Wall
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2
SDFLAGS = -lSDL2
EXEC = parsexml frame

all: $(EXEC)

parsexml: parsexml.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

frame: frame.c
	$(CC) -o $@ $(CPPFLAGS) $< $(SDFLAGS)
	
test: test.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
clean:
	rm -f $(OBJECTS)

mrproper: clean
	rm -f $(EXEC)
