CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2 -lSDL2 -lSDL2_gfx -lm
EXEC = renderer

all: $(EXEC)

parsexml.o: include/parsexml.h src/parsexml.c
	gcc -c src/parsexml.c -lxml2 -I/usr/include/libxml2
	
calcul.o: include/calcul.h src/calcul.c
	gcc -c src/calcul.c
	
graphics.o: include/calcul.h include/graphics.h src/graphics.c
	gcc -c src/graphics.c

renderer: parsexml.o calcul.o graphics.o src/renderer.c
	gcc -o renderer -I/usr/include/libxml2 src/renderer.c parsexml.o calcul.o graphics.o -lSDL2 -lSDL2_gfx -lm -lxml2
	
run: renderer
	./renderer maps_test/02_paris_place_des_vosges.osm 
	
clean:
	rm -rf *.o
	
mrproper: clean
	rm -f $(EXEC)
