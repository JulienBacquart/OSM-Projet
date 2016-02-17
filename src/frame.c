#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

void pause();

int main(int argc, char *argv[])
{
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Event event;
        SDL_Rect r;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
                return 3;
        }

        window = SDL_CreateWindow("SDL_CreateTexture",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        1024, 768,
                        SDL_WINDOW_RESIZABLE);

        r.w = 100;
        r.h = 50;
		r.x = 250;
		r.y = 250;

        renderer = SDL_CreateRenderer(window, -1, 0);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
        SDL_RenderClear(renderer);
        //SDL_RenderDrawRect(renderer,&r);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
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
