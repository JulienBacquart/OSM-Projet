#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

void pause();
int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b);

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
            return 3;
    }

	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

    window = SDL_CreateWindow("OpenStreetMap",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800, 800,SDL_WINDOW_SHOWN); //Création de la fenêtre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre

    SDL_SetRenderDrawColor(renderer,233,234,237,255);//La couleur du fond
	SDL_RenderClear(renderer);//Application de la couleur choisis

	writeText(renderer,"Some text",60,250,300,300,100,0,0,0);
	
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

int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b)
{
	TTF_Font *police = NULL;
	police = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-ExtraLight.ttf",fontWidth);	
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
