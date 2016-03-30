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
int drawLine(SDL_Renderer *renderer,int x1, int y1,int x2, int y2,int width,int r,int g, int b, int a);
int drawPolygon(SDL_Renderer *renderer, short *x_tab, short *y_tab, int nb_pts, int r, int g, int b, int a);
int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width ,int r, int g, int b, int alpha);
int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int r, int g, int b, int alpha);
//int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b,double angle);
