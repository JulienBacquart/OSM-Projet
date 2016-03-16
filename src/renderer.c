#include "../include/parsexml.h"
#include "../include/graphics.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s <file>.osm\n",argv[0]);
		exit(0);
	}
	
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

	window = SDL_CreateWindow("OpenStreetMap - Renderer",40,-105,WIDTH, HEIGHT,SDL_WINDOW_SHOWN); //Création de la fenêtre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre
	
	SDL_SetRenderDrawColor(renderer,242,239,233,0);
	SDL_RenderClear(renderer);
 	SDL_RenderPresent(renderer);

	/** Ici le code
		
			
		



	*/

	doPause();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return EXIT_SUCCESS;
}
