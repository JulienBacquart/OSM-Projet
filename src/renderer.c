#include "../include/parsexml.h"
//#include "../include/graphics.h"

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
	xmlDocPtr doc;
	xmlNodePtr racine;

	if(argc > 1) {
		
		// Ouverture du document
		xmlKeepBlanksDefault(0); // Ignore les noeuds texte composant la mise en forme
		doc = xmlParseFile(argv[1]);
	if (doc == NULL) {
		fprintf(stderr, "Document XML invalide\n");
		return EXIT_FAILURE;
	}
	// Récupération de la racine
	racine = xmlDocGetRootElement(doc);
	if (racine == NULL) {
			fprintf(stderr, "Document XML vierge\n");
			xmlFreeDoc(doc);
			return EXIT_FAILURE;
	}
		// Parcours
		Map map = parcours_prefixe(racine, afficher_noeud);
		Way *w = NULL;		
		
		for(w=map.h_ways; w!=NULL; w=(Way*)(w->hh.next)){
			printf(" id = %d \n", w->id);
			printf(" nb_points = %d \n",w->nb_nds);
			// CODE DEGUEULASSE ........................................................................................
			int i=0;
			int found= 0;
			printf("nb_tags %d \n", w->nb_tags);
			for(i=0; i < w->nb_tags; i++){
				printf("%s \n", w->tags[i].key);
				if(strcmp(w->tags[i].key,"highway")==0){
					found =1;
					break;
				}
				if(strcmp(w->tags[i].key,"building")==0){
					found =2;
					break;
				}
			}
			if(found ==1){
				drawRoad(renderer,w->nds,10,w->nb_nds,255, 255, 255, 255, map.h_nodes,map.m_bds);
				SDL_RenderPresent(renderer);
			}
			else if(found==2){
				drawMur(renderer,w->nds,10,w->nb_nds,255, 255, 255, 255, map.h_nodes,map.m_bds);
				SDL_RenderPresent(renderer);
			}
			else{
				drawRoad(renderer,w->nds,1,w->nb_nds,255, 0, 0, 255, map.h_nodes,map.m_bds);
				SDL_RenderPresent(renderer);			
			}
		}
		
		print_map(map);
		// Libération de la mémoire
		xmlFreeDoc(doc);
	}
	else {
		fprintf(stderr, "Name of the map file expected as argument\n");
		exit (EXIT_FAILURE);
	 }
	

	doPause();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return EXIT_SUCCESS;
}
