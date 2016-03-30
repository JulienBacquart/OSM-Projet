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

void doPause();
int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width, Uint32 color);
int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, Uint32 color);
//int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b,double angle);
