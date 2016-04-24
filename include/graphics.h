/**
 * \file graphics.h
 * \brief Contient les signatures et directives de préprocesseur pour la partie graphique(affichage)
 * \author Adel.Z Julien.B Charles.R
 */

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

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
#define MOTORWAY_Z_LEVEL 10

#define MOTORWAY_LINK_COLOR "#E990A1"
#define MOTORWAY_LINK_WIDTH 20
#define MOTORWAY_LINK_Z_LEVEL 10

#define TRUNK_COLOR "#FBB29A"
#define TRUNK_WIDTH 20
#define TRUNK_Z_LEVEL 9

#define TRUNK_LINK_COLOR "#FBB29A"
#define TRUNK_LINK_WIDTH 20
#define TRUNK_LINK_Z_LEVEL 9

#define PRIMARY_COLOR "#FDD7A1"
#define PRIMARY_WIDTH 15
#define PRIMARY_Z_LEVEL 8

#define PRIMARY_LINK_COLOR "#FDD7A1"
#define PRIMARY_LINK_WIDTH 15
#define PRIMARY_LINK_Z_LEVEL 8

#define SECONDARY_COLOR "#F6FABB"
#define SECONDARY_WIDTH 15
#define SECONDARY_Z_LEVEL 7

#define SECONDARY_LINK_COLOR "#F6FABB"
#define SECONDARY_LINK_WIDTH 15
#define SECONDARY_LINK_Z_LEVEL 7

#define TERTIARY_COLOR "#FEFEFE"
#define TERTIARY_WIDTH 15
#define TERTIARY_Z_LEVEL 6

#define TERTIARY_LINK_COLOR "#FEFEFE"
#define TERTIARY_LINK_WIDTH 15
#define TERTIARY_LINK_Z_LEVEL 6

#define UNCLASSIFIED_COLOR "#FEFEFE"
#define UNCLASSIFIED_WIDTH 10
#define UNCLASSIFIED_Z_LEVEL 5

#define RESIDENTIAL_COLOR "#FEFEFE"
#define RESIDENTIAL_WIDTH 10
#define RESIDENTIAL_Z_LEVEL 5

#define SERVICE_COLOR "#FEFEFE"
#define SERVICE_WIDTH 7
#define SERVICE_Z_LEVEL 5

#define LIVING_STREET_COLOR "#EDEDED"
#define LIVING_STREET_WIDTH 10
#define LIVING_STREET_Z_LEVEL 5

#define PEDESTRIAN_COLOR "#DDDDE9"
#define PEDESTRIAN_WIDTH 10
#define PEDESTRIAN_Z_LEVEL 20

#define TRACK_COLOR "#AC8327"
#define TRACK_WIDTH 5
#define TRACK_Z_LEVEL 20

#define FOOTWAY_COLOR "#FA8072"
#define FOOTWAY_DOT_RADIUS 2
#define FOOTWAY_Z_LEVEL 20

#define STEPS_COLOR "#FC7F6F"
#define STEPS_WIDTH 5
#define STEPS_Z_LEVEL 20

#define PATH_COLOR "#FA8072"
#define PATH_DASH_LENGTH 5
#define PATH_Z_LEVEL 20

#define CYCLEWAY_COLOR "#3333FC"
#define CYCLEWAY_DASH_LENGTH 5
#define CYCLEWAY_Z_LEVEL 20

#define RIVER_COLOR "#B5D0D0"
#define RIVER_Z_LEVEL -19

#define GRASS_COLOR "#8ef484"
#define GRASS_Z_LEVEL 15

#define BUILDING_COLOR "#BFAEAE"
#define BUILDING_Z_LEVEL 11

#define COASTLINE_Z_LEVEL -20


int hex_to_rgba(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a);
void doPause();
int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width, char* color);
int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color);
int drawFilledPolygon(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color);
int drawDashedLine(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color, int dash_length);
int drawDashedLine(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color, int dot_radius);
int writeText(SDL_Renderer *renderer,char *text, TTF_Font *police,int x, int y, char* color, double angle);
SDL_Point* get_centroid(Way *way, Node *h_nodes, Bounds *m_bds);
double getTextRotation(Way *way, Node *h_nodes, Bounds *m_bds);
SDL_Point* get_middle_of_way(Way *way, Node *h_nodes, Bounds *m_bds);
// int writeText(SDL_Renderer *renderer, const char *s, Sint16 x, Sint16 y, Uint32 rotation, char* color);

#endif // #ifndef _GRAPHICS_H_
