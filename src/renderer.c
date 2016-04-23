/**
* \file renderer.c
* \brief contient la fonction main
* \author Adel.Z Julien.B Charles.R
*/

#include "../include/parsexml.h"
#include "../include/graphics.h"
#include "../include/calcul.h"

int WIN_WIDTH;
int WIN_HEIGHT;


/**
 * \fn int main (int argc, char **argv)
 * \brief Entrée du programme.
 * \return 0 - Arrêt normal du programme.
 *
 * Lit un fichier .osm au format XML, remplit les structures en conséquence, appel la partie graphique pour lancer la frame et affiche chaque elements sur la frame. 
 * Attend la fermeture du programme
 */
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
		
		// Select the correct windows size based on the map ratio
		WIN_HEIGHT = MAX_WIN_HEIGHT;
		WIN_WIDTH = ratio_wh * MAX_WIN_HEIGHT;
		if (WIN_WIDTH > MAX_WIN_WIDTH){
			WIN_WIDTH = MAX_WIN_WIDTH;
			WIN_HEIGHT = WIN_WIDTH / ratio_wh;
		}
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
		
		// Select the background color
		Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
		Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
		Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
		Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
		html_to_rgba(BACKGROUND_COLOR, r, g, b, a);
		SDL_SetRenderDrawColor(renderer, *r, *g, *b, *a);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		
		// Draw ways
		Way *w;
		for(w=map.h_ways; w != NULL; w=w->hh.next) {			
			int i;
		
			int isHighway = 0;
			char name[40];
			strcpy(name,"");
			int priority = 0;

			// Parcours tags
			for(i = 0; i < w->nb_tags; i++){
// 				print_tag(w->tags[i]);
				
				// The way is a road
				if(strcmp(w->tags[i].key,"highway") == 0){
					isHighway = 1;					
										
					// Motorway
					if(strcmp(w->tags[i].val,"motorway") == 0){
						// Draw as a double line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, MOTORWAY_WIDTH * scale, MOTORWAY_COLOR);
						priority = 1;
						//break;
					}
					
					// Motorway link
					else if(strcmp(w->tags[i].val,"motorway_link") == 0){
						// Draw as a double line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, MOTORWAY_LINK_WIDTH * scale, MOTORWAY_LINK_COLOR);
						priority = 1;						
						//break;
					}
					
					// Trunk
					else if(strcmp(w->tags[i].val,"trunk") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, TRUNK_WIDTH * scale, TRUNK_COLOR);
						priority = 1;
						//break;
					}
					
					// Trunk link
					else if(strcmp(w->tags[i].val,"trunk_link") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, TRUNK_LINK_WIDTH * scale, TRUNK_LINK_COLOR);
						priority = 1;
						//break;
					}
					
					// Primary road
					else if(strcmp(w->tags[i].val,"primary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, PRIMARY_WIDTH * scale, PRIMARY_COLOR);
						priority = 2;
						//break;
					}
					
					// Primary link
					else if(strcmp(w->tags[i].val,"primary_link") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, PRIMARY_LINK_WIDTH * scale, PRIMARY_LINK_COLOR);
						priority = 2;							
						//break;
					}
					
					// Secondary road
					else if(strcmp(w->tags[i].val,"secondary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, SECONDARY_WIDTH * scale, SECONDARY_COLOR);
						priority = 2;
						//break;
					}
					
					// Secondary link
					else if(strcmp(w->tags[i].val,"secondary_link") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, SECONDARY_LINK_WIDTH * scale, SECONDARY_LINK_COLOR);
						priority = 2;
						priority = 2;
						//break;
					}
					
					// Tertiary road
					else if(strcmp(w->tags[i].val,"tertiary") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, TERTIARY_WIDTH * scale, TERTIARY_COLOR);
						priority = 2;
						//break;
					}
					
					// Tertiary link
					else if(strcmp(w->tags[i].val,"tertiary_link") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, TERTIARY_LINK_WIDTH * scale, TERTIARY_LINK_COLOR);
						priority = 2;						
						//break;
					}
					
					// unclassified road
					else if(strcmp(w->tags[i].val,"unclassified") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, UNCLASSIFIED_WIDTH * scale, UNCLASSIFIED_COLOR);
						priority = 3;						
						//break;
					}
					
					// residential road
					else if(strcmp(w->tags[i].val,"residential") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, RESIDENTIAL_WIDTH * scale, RESIDENTIAL_COLOR);
						priority = 3;						
						//break;
					}
					
					// service road
					else if(strcmp(w->tags[i].val,"service") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, SERVICE_WIDTH * scale, SERVICE_COLOR);
						priority = 3;						
						//break;
					}
					
					// living street road
					else if(strcmp(w->tags[i].val,"living_street") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, LIVING_STREET_WIDTH * scale, LIVING_STREET_COLOR);
						priority = 3;						
						//break;
					}
					
					// pedestrian road
					else if(strcmp(w->tags[i].val,"pedestrian") == 0){
						drawRoad(renderer, w, map.h_nodes, map.m_bds, PEDESTRIAN_WIDTH * scale, PEDESTRIAN_COLOR);
						priority = 3;						
						//break;
					}
					
					// track
					else if(strcmp(w->tags[i].val,"track") == 0){
						// Should be dotted-dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, TRACK_WIDTH * scale, TRACK_COLOR);
						priority = 3;						
						//break;
					}
										
					// footway road
					else if(strcmp(w->tags[i].val,"footway") == 0){
						// Should be dotted line
// 						drawRoad(renderer, w, map.h_nodes, map.m_bds, FOOTWAY_WIDTH * scale, FOOTWAY_COLOR);
						drawDottedLine(renderer, w, map.h_nodes, map.m_bds, FOOTWAY_COLOR, FOOTWAY_DOT_RADIUS);
						//break;
					}
					
					// steps
					else if(strcmp(w->tags[i].val,"steps") == 0){
						//Should be dashed line
						drawRoad(renderer, w, map.h_nodes, map.m_bds, STEPS_WIDTH, STEPS_COLOR);
						//break;
					}
// 					
					
					// cycleway road
					else if(strcmp(w->tags[i].val,"cycleway") == 0){
						// Should be dotted line
						drawDashedLine(renderer, w, map.h_nodes, map.m_bds, CYCLEWAY_COLOR, CYCLEWAY_DASH_LENGTH);
						//break;
					}
					
					// path
					else if(strcmp(w->tags[i].val,"path") == 0){
						// Should be dotted line
						drawDashedLine(renderer, w, map.h_nodes, map.m_bds, PATH_COLOR, PATH_DASH_LENGTH);
						//break;
					}
					
					else {
						printf("highway value:%s\n", w->tags[i].val);
// 						drawRoad(renderer, w, map.h_nodes, map.m_bds, 10, 0xFF00FF00);
						//break;
					}
				} 
				// The way is a river
				else if(strcmp(w->tags[i].key,"waterway") == 0){
					// Different case: river, riverbank
					if(strcmp(w->tags[i].val,"river") == 0){
// 						drawRoad(renderer, w, map.h_nodes, map.m_bds, 15, 0xFFD0D0B5);
						break;
					}
					if(strcmp(w->tags[i].val,"riverbank") == 0){
						drawFilledPolygon(renderer, w, map.h_nodes, map.m_bds, RIVER_COLOR);
						break;
					}
				}
				// The way is a building
				else if(strcmp(w->tags[i].key,"building") == 0){
					drawBuilding(renderer, w, map.h_nodes, map.m_bds, BUILDING_COLOR);
				}
				
				// Green spaces
				else if(strcmp(w->tags[i].key,"landuse") == 0){
					if(strcmp(w->tags[i].val,"grass") == 0){
						
						drawBuilding(renderer, w, map.h_nodes, map.m_bds, GRASS_COLOR);
						break;
					}
				}

				// Coastline
				else if(strcmp(w->tags[i].key,"natural") == 0){
					if(strcmp(w->tags[i].val,"coastline") == 0){
						
						//drawFilledPolygon(renderer, w, map.h_nodes, map.m_bds, GRASS_COLOR);
						drawBuilding(renderer, w, map.h_nodes, map.m_bds, "ffffff");
						break;
					}
				}
				
				// Amenity
				else if(strcmp(w->tags[i].key,"amenity") == 0){
					if(strcmp(w->tags[i].val,"fountain") == 0){
						drawBuilding(renderer, w, map.h_nodes, map.m_bds, RIVER_COLOR);
						break;
					}
				}

				else if(strcmp(w->tags[i].key,"name") == 0){
					int c=0;
					for(c; c<40; c++){
						name[c] = w->tags[i].val[c];
					}
				}

			}
			
			// display highways names
			if(isHighway == 1 && strcmp(name,"")!=0){
				double ratio = 1/scale;
				int display = 0;				
				if(ratio <1){
					if(priority<4){
						display = 1;
					}
				}
				else if(ratio <2){
					if(priority<3){
						display = 1;
					}
				}
				else{
					if(priority<2){
						display = 1;
					}
				}
				if(display == 1){
					name[37] = '.';	
					name[38] = '.';
					name[39] = '.';
					name[40] = '\0';
					printf("Name: %s - ",name);
					double test = getTextRotation(w, map.h_nodes, map.m_bds);			
					//printf("alpha =  %f \n", test);
					SDL_Point* point = get_middle_of_way(w, map.h_nodes, map.m_bds);
					//printf("main :  x=%d, y=%d \n",point->x,point->y);
								
					writeText(renderer,name,9,point->x,point->y,120,11,0,0,0, test, point);

				}	
				
			}
			else if(isHighway == 0 && strcmp(name,"")!=0){
				name[37] = '.';				
				name[38] = '.';				
				name[39] = '.';
				name[40] = '\0';
				SDL_Point* point = get_centroid(w, map.h_nodes, map.m_bds);					
				writeText(renderer,name,8,point->x,point->y,120,11,0,0,0, 0., point);
				
			}
		}
		
		// Draw relations
		Relation *rel;
		for(rel=map.h_relations; rel != NULL; rel=rel->hh.next) {
			int i;
			int j;
			
			// Parcours tags
			for(i = 0; i < rel->nb_tags; i++){
// 				print_tag(rel->tags[i]);
				
				// The relation is a multipolygon
				if(strcmp(rel->tags[i].key,"type") == 0 && strcmp(rel->tags[i].val,"multipolygon") == 0){
					
					// Parcours members
					for(j = 0; j < rel->nb_members; j++){
						// We try to find a member defined as 'inner' to draw the inner yard
						if(strcmp(rel->members[j].role,"inner") == 0){
// 							print_member(rel->members[j]);
							
							// We find the way with its id
							w = find_way(rel->members[j].ref);
							if (w != NULL){
// 								print_way(*find_way(rel->members[j].ref));
								
								// Draw the inner yard
								drawBuilding(renderer, w, map.h_nodes, map.m_bds, BACKGROUND_COLOR);
							} else {
								// In case the reference is to an unknown way (seems pretty frequent) 
								fprintf(stderr, "Error: Unknown reference to way: %d\n", rel->members[j].ref);
							}
						}
					}
				}
			}
		}
				
		// Render
		SDL_RenderPresent(renderer);

		//Test du texte
		//writeText(renderer,"YOLO",50,350,250,120,120,0,0,0, 45.);
		
		doPause();
		
		// Libération de la mémoire		
		xmlFreeDoc(doc);
		
		free(r);
		free(g);
		free(b);
		free(a);
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		
	} else {
		fprintf(stderr,"Usage: %s <file>.osm\n",argv[0]);
		exit (EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
