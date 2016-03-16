CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
CPPFLAGS = -I/usr/include/libxml2
LDFLAGS = -lxml2 -lSDL2 -lSDL2_gfx -lSDL2_ttf -lm
EXEC = parsexml frame render_min

all: $(EXEC)

parsexml: parsexml.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

frame: frame.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)

render_min: render_min.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
calc_coord: calc_coord.c
	$(CC) -o $@ $(CPPFLAGS) $< $(LDFLAGS)
	
run: parsexml
# 	./test ../maps_test/01_denver_lafayette.osm
	./test ../maps_test/02_paris_place_des_vosges.osm
	
clean:
	rm -f $(OBJECTS)

mrproper: clean
	rm -f $(EXEC)
