#include "SDL2/SDL.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

    SDL_Window *window; 
    

    SDL_Init(SDL_INIT_VIDEO);              // Initialisation de SDL2

    window = SDL_CreateWindow(
        "Hello world!",                  // titre de la fenetre
        SDL_WINDOWPOS_UNDEFINED,           // position initial du x
        SDL_WINDOWPOS_UNDEFINED,           // position initial du y
        640,                               // largeur en px
        480,                               // hauteur en px
        SDL_WINDOW_OPENGL                  // flags
    );

	//verifie que la fenetre a bien ete creee
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    
    //cree le renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, 0);
    
    //Couleur = blanc
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    
    //colore toute la fenetre
    SDL_RenderClear(renderer); 
    
    //Couleur = noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    
    //Dessine le H
    SDL_RenderDrawLine(renderer, 225, 190, 225, 290);
    SDL_RenderDrawLine(renderer, 225, 240, 255, 240);
    SDL_RenderDrawLine(renderer, 255, 190, 255, 290);
    
    //Dessine le E
    SDL_RenderDrawLine(renderer, 265, 190, 295, 190);
    SDL_RenderDrawLine(renderer, 265, 240, 295, 240);
    SDL_RenderDrawLine(renderer, 265, 290, 295, 290);
    SDL_RenderDrawLine(renderer, 265, 190, 265, 290);
    
    //Dessine le premier L
    SDL_RenderDrawLine(renderer, 305, 190, 305, 290);
    SDL_RenderDrawLine(renderer, 305, 290, 335, 290);
    
    //Dessine le second L
    SDL_RenderDrawLine(renderer, 345, 190, 345, 290);
    SDL_RenderDrawLine(renderer, 345, 290, 375, 290);
    
    //Dessine le O
    SDL_RenderDrawLine(renderer, 385, 190, 415, 190);
    SDL_RenderDrawLine(renderer, 385, 190, 385, 290);
    SDL_RenderDrawLine(renderer, 385, 290, 415, 290);
    SDL_RenderDrawLine(renderer, 415, 190, 415, 290);
    
    //met à jour le renderer
	SDL_RenderPresent(renderer);
	
    SDL_Event event;
    //attend que le bouton de fermeture de la fenetre soit activé
	while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            break;
    }
    //ferme la fenetre
    SDL_DestroyWindow(window);
    //quitte SDL
    SDL_Quit();
    return 0;
}
