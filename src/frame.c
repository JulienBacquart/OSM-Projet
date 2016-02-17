#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

void pause();

int main(int argc, char *argv[])
{
        SDL_Window *window;
        SDL_Renderer *renderer;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
                return 3;
        }

        window = SDL_CreateWindow("SDL_CreateTexture",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1024, 768,SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

		pause();
 
        SDL_DestroyRenderer(renderer);
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
