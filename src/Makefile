CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2 -lSDL2 -lSDL2_gfx -lSDL2_ttf
EXEC = parsexml frame render_min

all: $(EXEC)

parsexml: parsexml.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

frame: frame.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
test: test.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

render_min: render_min.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
clean:
	rm -f $(OBJECTS)

mrproper: clean
	rm -f $(EXEC)
