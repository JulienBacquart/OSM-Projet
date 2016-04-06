#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_ttf.h>

#include "data.h"

// #define WIN_WIDTH 800
#define WIN_HEIGHT 600
extern int WIN_WIDTH;

#define MOTORWAY_COLOR "#E990A0"
#define MOTORWAY_LINK_COLOR "#E990A1"
#define TRUNK_COLOR "#FBB29A"
#define TRUNK_LINK_COLOR "#FBB29A"
#define PRIMARY_COLOR "#FDD7A1"
#define PRIMARY_LINK_COLOR "#FDD7A1"
#define SECONDARY_COLOR "#F6FABB"
#define SECONDARY_LINK_COLOR "#F6FABB"
#define TERTIARY_COLOR "#FEFEFE"
#define TERTIARY_LINK_COLOR "#FEFEFE"

#define UNCLASSIFIED_COLOR "#FEFEFE"
#define RESIDENTIAL_COLOR "#FEFEFE"
#define SERVICE_COLOR "#FEFEFE"

#define LIVING_STREET_COLOR "#EDEDED"
#define PEDESTRIAN_COLOR "#DDDDE9"
#define TRACK_COLOR "#AC8327"
#define FOOTWAY_COLOR "#FA8072"
#define STEPS_COLOR "#FC7F6F"
#define PATH_COLOR "#FA8072"
#define CYCLEWAY_COLOR "#3333FC"

#define RIVER_COLOR "#B5D0D0"

#define BUILDING_COLOR "#BFAEAE"


int hex_to_rgba(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a);
void doPause();
int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width, char* color);
int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color);
int drawFilledPolygon(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color);
//int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b,double angle);
