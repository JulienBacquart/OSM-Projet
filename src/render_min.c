#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 800

void pause();
int drawLine(SDL_Renderer *renderer,int x1, int y1,int x2, int y2,int width,int r,int g, int b, int a);
int drawRoad(SDL_Renderer *renderer,SDL_Point *points,int width,int nbPoints,int r, int g, int b, int a);
int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b);

int main(int argc, char **argv)
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow("OpenStreetMap - Renderer minimal",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH, HEIGHT,SDL_WINDOW_SHOWN); //Création de la fenêtre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre

	SDL_SetRenderDrawColor(renderer,242,239,233,0);//La couleur du fond
	SDL_RenderClear(renderer);//Application de la couleur choisis

	//Dessin des lignes pour le renderer minimal
	drawLine(renderer,80,0,80,HEIGHT,10,255,255,179,255);
	drawLine(renderer,500,0,500,HEIGHT,10,255,255,255,255);
	drawLine(renderer,750,0,750,HEIGHT,10,255,255,255,255);
	drawLine(renderer,0,150,WIDTH,150,10,255,255,255,255);
	drawLine(renderer,0,400,WIDTH,400,10,255,255,255,255);
	drawLine(renderer,0,750,WIDTH,750,10,255,255,255,255);
	drawLine(renderer,290,150,290,HEIGHT,10,255,255,255,255);

	writeText(renderer,"Some text",100,200,20,200,100,0,0,0);

	SDL_RenderPresent(renderer);

	pause();

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void pause()
{
	SDL_Event evenements;
    int terminer = 0;

	while(!terminer)
    {
		SDL_WaitEvent(&evenements);
		
		if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
			terminer = 1;
    }
}

int drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int width, int r, int g, int b, int a)
{
	thickLineRGBA(renderer,x1,y1,x2,y2,width,r,g,b,a);
	return 0;
}

int drawRoad(SDL_Renderer *renderer,SDL_Point *points,int width,int nbPoints,int r, int g, int b, int a)
{
	int i;
	for(i = 0;i<nbPoints;i++)
	{
		if(i == nbPoints-1)
		{
			break;
		}
		thickLineRGBA(renderer,points[i].x,points[i].y,points[i+1].x,points[i+1].y,width,r,g,b,a);
	}
	return 0;
}

int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b)
{
	TTF_Font *police = NULL;
	police = TTF_OpenFont("DejaVuSans-ExtraLight.ttf",fontWidth);	
	SDL_Color coul = {r,g,b};
	SDL_Surface* texte = TTF_RenderText_Blended(police,text,coul);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer,texte);

	SDL_Rect message_rect; //create a rec
	message_rect.x = x;  //controls the rect's x coordinate 
	message_rect.y = y; // controls the rect's y coordinte
	message_rect.w = width; // controls the width of the rect
	message_rect.h = height; // controls the height of the rect

	SDL_RenderCopy(renderer,message,NULL,&message_rect);

	TTF_CloseFont(police);
}
