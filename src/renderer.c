#include "../include/parsexml.h"
#include "../include/graphics.h"
#include "../include/calcul.h"

int WIN_WIDTH;

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
		
		// Find map ratio to know the ratio we should use for the window
		double delta_x_meter = lon_to_x(map.m_bds->maxlon) - lon_to_x(map.m_bds->minlon);
		double delta_y_meter = lat_to_y(map.m_bds->maxlat) - lat_to_y(map.m_bds->minlat);
		double ratio_wh = delta_x_meter / delta_y_meter;
		printf("Ratio W/H: %f\n", ratio_wh);
		WIN_WIDTH = ratio_wh * WIN_HEIGHT;
		printf("Size window W x H: %d x %d\n", WIN_WIDTH, WIN_HEIGHT);
		
		// Calculate the 'scale' of the map in pixel/meter
		// For example if we have a road with a width of 5 meter, it size on the screen will be:
		// 5m x scale = nb of pixels
		double scale = WIN_HEIGHT / delta_y_meter;
		printf("Scale: 1 px = %f meter\n", 1/scale);
		
		// Create display window
		SDL_Window *window;
		SDL_Renderer *renderer;

		if (SDL_Init(SDL_INIT_VIDEO) < 0){
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
			return -1;
		}

		window = SDL_CreateWindow("OpenStreetMap - Renderer",40,-105, WIN_WIDTH, WIN_HEIGHT,SDL_WINDOW_SHOWN); //Création de la fenêtre
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre
	
		SDL_SetRenderDrawColor(renderer, 242, 239, 233, 255);
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
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 15, 0xFF9A98EC);
						break;
					}
					
					// Secondary road
					if(strcmp(w->tags[i].val,"secondary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFFA5D7FE);
						break;
					}
					
					// Tertiary road
					if(strcmp(w->tags[i].val,"tertiary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFFB3FFFF);
						break;
					}
					
					// residential road
					else if(strcmp(w->tags[i].val,"residential") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFFFFFFFF);
						break;
					}
					
					// pedestrian road
					else if(strcmp(w->tags[i].val,"pedestrian") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFFFFFFFF);
						break;
					}
					
					// service road
					else if(strcmp(w->tags[i].val,"service") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 5, 0xFFFFFFFF);
						break;
					}
					
					// footway road
					else if(strcmp(w->tags[i].val,"footway") == 0){
						// Should be dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 5, 0xFF0000FF);
						break;
					}
					
					// cycleway road
					else if(strcmp(w->tags[i].val,"cycleway") == 0){
						// Should be dotted line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 1, 0xFFFF0000);
						break;
					}
					
					// unclassified road
					else if(strcmp(w->tags[i].val,"unclassified") == 0){
						// Should be dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFFFFFFFF);
						break;
					}
					
					// steps
					else if(strcmp(w->tags[i].val,"steps") == 0){
						// Should be dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFF7280FA);
						break;
					}
					
					else {
						printf("highway value:%s\n", w->tags[i].val);
// 						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFF00FF00);
						break;
					}
				} 
				// The way is a river
				else if(strcmp(w->tags[i].key,"waterway") == 0){
					// Different case: river, riverbank
					if(strcmp(w->tags[i].val,"river") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, 15, 0xFFD0D0B5);
						break;
					}
					if(strcmp(w->tags[i].val,"riverbank") == 0){
						drawFilledPolygon(renderer, w, map.h_nodes, map.m_bds, 0xFFD0D0B5);
						break;
					}
				}
				// The way is a bulding
				else if(strcmp(w->tags[i].key,"building") == 0){
// 					printf("building:\n");
// 					print_way(*w);
					drawBuilding(renderer, w, map.h_nodes, map.m_bds, 0xFFAEAEBF);
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
