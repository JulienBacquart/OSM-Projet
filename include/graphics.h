#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_ttf.h>

#include "data.h"

// #define WIN_WIDTH 800
#define MAX_WIN_HEIGHT 600
#define MAX_WIN_WIDTH 1200
extern int WIN_HEIGHT;
extern int WIN_WIDTH;

#define BACKGROUND_COLOR "#F2EFE9"

#define MOTORWAY_COLOR "#E990A0"
#define MOTORWAY_WIDTH 20

#define MOTORWAY_LINK_COLOR "#E990A1"
#define MOTORWAY_LINK_WIDTH 20

#define TRUNK_COLOR "#FBB29A"
#define TRUNK_WIDTH 20

#define TRUNK_LINK_COLOR "#FBB29A"
#define TRUNK_LINK_WIDTH 20 

#define PRIMARY_COLOR "#FDD7A1"
#define PRIMARY_WIDTH 15

#define PRIMARY_LINK_COLOR "#FDD7A1"
#define PRIMARY_LINK_WIDTH 15

#define SECONDARY_COLOR "#F6FABB"
#define SECONDARY_WIDTH 15

#define SECONDARY_LINK_COLOR "#F6FABB"
#define SECONDARY_LINK_WIDTH 15

#define TERTIARY_COLOR "#FEFEFE"
#define TERTIARY_WIDTH 15

#define TERTIARY_LINK_COLOR "#FEFEFE"
#define TERTIARY_LINK_WIDTH 15

#define UNCLASSIFIED_COLOR "#FEFEFE"
#define UNCLASSIFIED_WIDTH 10

#define RESIDENTIAL_COLOR "#FEFEFE"
#define RESIDENTIAL_WIDTH 10

#define SERVICE_COLOR "#FEFEFE"
#define SERVICE_WIDTH 7

#define LIVING_STREET_COLOR "#EDEDED"
#define LIVING_STREET_WIDTH 10

#define PEDESTRIAN_COLOR "#DDDDE9"
#define PEDESTRIAN_WIDTH 10

#define TRACK_COLOR "#AC8327"
#define TRACK_WIDTH 5

#define FOOTWAY_COLOR "#FA8072"
#define FOOTWAY_WIDTH 5

#define STEPS_COLOR "#FC7F6F"
#define STEPS_WIDTH 5

#define PATH_COLOR "#FA8072"
#define PATH_WIDTH 5

#define CYCLEWAY_COLOR "#3333FC"
#define CYCLEWAY_WIDTH 5

#define RIVER_COLOR "#B5D0D0"

#define BUILDING_COLOR "#BFAEAE"


int hex_to_rgba(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a);
void doPause();
int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width, char* color);
int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color);
int drawFilledPolygon(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color);
//int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b,double angle);
