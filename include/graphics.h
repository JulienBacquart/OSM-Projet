#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 800

int long2tilex(double lon, int z);
int lat2tiley(double lat, int z);
double tilex2long(int x, int z);
double tiley2lat(int y, int z);
void pause();
int drawLine(SDL_Renderer *renderer,int x1, int y1,int x2, int y2,int width,int r,int g, int b, int a);
int drawRoad(SDL_Renderer *renderer,SDL_Point *points,int width,int nbPoints,int r, int g, int b, int a);
//int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b,double angle);
