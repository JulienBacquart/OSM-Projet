CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2 -lSDL2 -lSDL2_gfx -lm
EXEC = renderer

all: $(EXEC)

renderer: src/renderer.c  src/graphics.c src/parsexml.c src/calcul.c
	$(CC) -o $@ $(CPPFLAGS) $^ $(LDFLAGS)
	
run: renderer
	./renderer maps_test/02_paris_place_des_vosges.osm
	
mrproper: clean
	rm -f $(EXEC)
