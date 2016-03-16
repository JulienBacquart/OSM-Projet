CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2 -lSDL2 -lSDL2_gfx -lm
EXEC = renderer

all: $(EXEC)

renderer: src/renderer.c  src/graphics.c src/parsexml.c
	$(CC) -o $@ $(CPPFLAGS) $^ $(LDFLAGS)

mrproper: clean
	rm -f $(EXEC)
