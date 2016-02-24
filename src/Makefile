CC = gcc
CFLAGS = -g -Wall
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2 -lSDL2 -lSDL2_gfx
EXEC = parsexml frame

all: $(EXEC)

parsexml: parsexml.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

frame: frame.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
test: test.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
clean:
	rm -f $(OBJECTS)

mrproper: clean
	rm -f $(EXEC)
