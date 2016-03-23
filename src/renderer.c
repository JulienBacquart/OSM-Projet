#include "../include/parsexml.h"
//#include "../include/graphics.h"

int main(int argc, char *argv[]) {
	
	if(argc > 1) {
		xmlDocPtr doc;
		xmlNodePtr racine;
		printf("Start parsing %s...\n", argv[1]);
		
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
		
		printf("Document parsed successfully, data extracted:\n");
		print_map(map);
		
		// Create display window
		SDL_Window *window;
		SDL_Renderer *renderer;

		if (SDL_Init(SDL_INIT_VIDEO) < 0){
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
			return -1;
		}

		window = SDL_CreateWindow("OpenStreetMap - Renderer",40,-105,WIN_WIDTH, WIN_HEIGHT,SDL_WINDOW_SHOWN); //Création de la fenêtre
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre
	
		SDL_SetRenderDrawColor(renderer,242,239,233,0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		
		// Draw ways
		Way *w;
		for(w=map.h_ways; w != NULL; w=w->hh.next) {			
			int i;
			
			// Parcours tags
			for(i = 0; i < w->nb_tags; i++){
// 				print_tag(w->tags[i]);
				
				// The way is a road
				if(strcmp(w->tags[i].key,"highway") == 0){
					
					// Primary road
					if(strcmp(w->tags[i].val,"primary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 15, 255, 161, 144, 255);
						break;
					}
					
					// Tertiary road
					if(strcmp(w->tags[i].val,"tertiary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 255, 255, 150, 255);
						break;
					}
					
					// residential road
					else if(strcmp(w->tags[i].val,"residential") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 255, 255, 255, 255);
						break;
					}
					
					// pedestrian road
					else if(strcmp(w->tags[i].val,"pedestrian") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 215, 215, 215, 255);
						break;
					}
					
					// service road
					else if(strcmp(w->tags[i].val,"service") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 195, 195, 195, 255);
						break;
					}
					
					// footway road
					else if(strcmp(w->tags[i].val,"footway") == 0){
						// Should be dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0, 195, 195, 255);
						break;
					}
					
					// cycleway road
					else if(strcmp(w->tags[i].val,"cycleway") == 0){
						// Should be dotted line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 5, 0, 0, 195, 255);
						break;
					}
					
					// unclassified road
					else if(strcmp(w->tags[i].val,"unclassified") == 0){
						// Should be dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 195, 195, 0, 255);
						break;
					}
					
					else {
						printf("highway value:%s\n", w->tags[i].val); 
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 195, 0, 0, 255);
						break;
					}
				} 
				// The way is a river
				else if(strcmp(w->tags[i].key,"waterway") == 0){
					// Different case: river, riverbank
					if(strcmp(w->tags[i].val,"river") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 15, 0, 0, 255, 255);
						break;
					}
				}
				// The way is a bulding
				else if(strcmp(w->tags[i].key,"building") == 0){
// 					printf("building:\n");
// 					print_way(*w);
					drawBuilding(renderer, w, map.h_nodes, map.m_bds, 191, 174, 174, 255);
				}
				
			}
		}
		
		doPause();
		
		// Libération de la mémoire		
		xmlFreeDoc(doc);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		
	} else {
		fprintf(stderr,"Usage: %s <file>.osm\n",argv[0]);
		exit (EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
