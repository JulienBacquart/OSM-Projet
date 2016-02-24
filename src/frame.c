#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void pause();
int drawRoad(SDL_Renderer *renderer,SDL_Point *points,int width,int nbPoints,int r, int g, int b, int a);

int main(int argc, char *argv[])
{
        SDL_Window *window;
        SDL_Renderer *renderer;
		SDL_Point hexa[7]; //Permet de faire un chemin de points

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
                return 3;
        }

        window = SDL_CreateWindow("OpenStreetMap",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800, 800,SDL_WINDOW_SHOWN); //Création de la fenêtre
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre

        SDL_SetRenderDrawColor(renderer,233,234,237,255);//La couleur du fond
		SDL_RenderClear(renderer);//Application de la couleur choisis

		SDL_SetRenderDrawColor(renderer,0,0,0,255);

		SDL_SetRenderDrawColor(renderer,255,0,0,255);//On change la couleur pour le prochain rectangle

		SDL_Point test[10];//Definition d'un tableau de SDL_Point contenant chaque point a dessiner
		test[0].x = test[0].y = 100;
		test[1].x = 200;test[1].y = 150;
		test[2].x = 250;test[2].y = 200;
		test[3].x = 310;test[3].y = 260;
		test[4].x = 420;test[4].y = 700;
		test[5].x = 550;test[5].y = 500;
		test[6].x = 600;test[6].y = 550;
		test[7].x = 720;test[7].y = 670;
		test[8].x = 770;test[8].y = 720;
		test[9].x = 800;test[9].y = 750;

		//Fonction drawRoad qui prend en paramètre le renderer, le tableau de points, l'épaisseur du trait,le nombre de points a dessiner (taille du tableau), et les 4 entiers rgba
		drawRoad(renderer,test,5,10,246,249,190,255);
		SDL_RenderPresent(renderer);

		pause();
 
        SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
        SDL_Quit();
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
