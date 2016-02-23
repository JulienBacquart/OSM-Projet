#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define NB_PIXELS 28

void pause();
int drawRoad(int xdeb, int ydeb, int xfin, int yfin, int ep, SDL_Renderer *);

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

		hexa[0].x = 200;hexa[0].y = 100;
		hexa[1].x = 350;hexa[1].y = 250;
		hexa[2].x = 350;hexa[2].y = 450;
		hexa[3].x = 200;hexa[3].y = 600;
		hexa[4].x = 50;hexa[4].y = 450;
		hexa[5].x = 50;hexa[5].y = 250;
		hexa[6] = hexa[0];

		SDL_RenderDrawLines(renderer,hexa,7);

		SDL_Rect rect = {700,700,50,50};//Perau et de créer un rectangle vide au points 700,700 et de dimension 50,50 
		SDL_RenderDrawRect(renderer,&rect);

		SDL_SetRenderDrawColor(renderer,255,0,0,255);//On change la couleur pour le prochain rectangle

		SDL_Rect rectFill = {600,700,50,50};
		SDL_RenderFillRect(renderer,&rectFill);

		//Dessin d'une route vertical en rouge
		drawRoad(400,0,400,800,100,renderer);

		//Dessin d'une route horizontal en bleu
		SDL_SetRenderDrawColor(renderer,0,0,255,255);
		drawRoad(0,300,800,300,100,renderer);

		//Dessin d'une route en diagonale en noir
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		drawRoad(0,300,300,0,200,renderer);
		

		SDL_RenderPresent(renderer);

		pause();
 
        SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}

//Rajouter le début et la fin en args
int drawRoad(int xdeb, int ydeb, int xfin, int yfin, int ep, SDL_Renderer *renderer)
{
	int i,j;
	int moy = 0;
	SDL_RenderDrawLine(renderer,xdeb,ydeb,xfin,yfin);
	if(xdeb < ydeb && xfin > yfin && ydeb != yfin)//Route en diagonale
	{
		SDL_RenderDrawLine(renderer,xdeb,ydeb+ep,xfin+ep,yfin);
		//SDL_RenderDrawLine(renderer,130,300,150,280);
		/*for(i = ydeb+30,j = ydeb+90;i<yfin,j<yfin;i+=110,j+=110)
		{
			SDL_RenderDrawLine(renderer,moy,i,moy,j);
		}*/
	}
	else if(ydeb == yfin)//route horizontal
	{
		moy = (int)(ydeb*2+ep)/2;
		SDL_RenderDrawLine(renderer,xdeb,ydeb+ep,xfin,yfin+ep);
		for(i = xdeb+30,j = xdeb+90;i<xfin,j<xfin;i+=110,j+=110)
		{
			SDL_RenderDrawLine(renderer,i,moy,j,moy);
		}
	}
	else
	{
		moy = (int)(xdeb*2+ep)/2;
		SDL_RenderDrawLine(renderer,xdeb+ep,ydeb,xfin+ep,yfin);
		for(i = ydeb+30,j = ydeb+90;i<yfin,j<yfin;i+=110,j+=110)
		{
			SDL_RenderDrawLine(renderer,moy,i,moy,j);
		}
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
