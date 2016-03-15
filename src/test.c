#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>

#include "data.h"
#include "graphics.h"

typedef void (*fct_parcours_t)(xmlNodePtr);

// The hashtable holding the nodes
Node *nodes = NULL;    /* important! initialize to NULL */
// The hashtable holding the ways
Way *ways = NULL;    /* important! initialize to NULL */

void print_bounds(Bounds bds){
	printf("Bound minlat: %.7f , minlon: %.7f , maxlat: %.7f , maxlon: %.7f\n", bds.minlat, bds.minlon, bds.maxlat, bds.maxlon);
}

void print_tag(Tag tag){
	printf("Tag k: %s, v: %s\n", tag.key, tag.val);
}

void print_node(Node node){
	// For lat et long we need a precision to the 7 decimal place
	printf("Node id: %d visible=%d lat=%.7f lon=%.7f nb_tags=%d\n", node.id, node.visible, node.lat, node.lon, node.nb_tags);
	int i;
	for(i = 0; i< node.nb_tags; i++){
		print_tag(node.tags[i]);
	}
}

void print_way(Way way){
	printf("Way id: %d visible=%d nb_nds=%d nb_tags=%d \n", way.id, way.visible, way.nb_nds, way.nb_tags);
	int i;
	for(i = 0; i< way.nb_tags; i++){
		print_tag(way.tags[i]);
	}
// 	for(i = 0; i< way.nb_nds; i++){
// 		print_node(way.nds[i]);
// 	}
}

// Find a node in the hashtable nodes from its id
Node *find_node(int node_id) {
    Node *n;

    HASH_FIND_INT(nodes, &node_id, n);  /* s: output pointer */
    return n;
}

// Add a node to the hashtable 'nodes'
void add_node(Node *nd) {
	// Maybe check uniqueness ?
	HASH_ADD_INT(nodes, id, nd);  /* id: name of key field */
// 	printf("Node %d added successfully to the table\n", nd->id);
}

// Add a way to the hashtable 'ways'
void add_way(Way *way) {
	// Maybe check uniqueness ?
	HASH_ADD_INT(ways, id, way);  /* id: name of key field */
// 	printf("Way %d added successfully to the table\n", way->id);
}

void parcours_prefixe(xmlNodePtr noeud, fct_parcours_t f) {
    
    xmlNodePtr n;
    
    // We traverse the xml tree looking at each xml node
    for (n = noeud->children; n != NULL; n = n->next) {
	// The xml node is 'bound'
	if (!strcmp(n->name, "bounds")){
		// Initialize
		Bounds *bds = (Bounds *) malloc(sizeof(Bounds));
		
		// We extract the attributes of a bound
		xmlAttr* attribute = n->properties;
		while(attribute)
		{
			xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			
			if (!strcmp(attribute->name,"minlat")){
				bds->minlat = atof(value);	
			}
			else if (!strcmp(attribute->name,"minlon")){
				bds->minlon = atof(value);
			}
			else if (!strcmp(attribute->name,"maxlat")){
				bds->maxlat = atof(value);	
			}
			else if (!strcmp(attribute->name,"maxlon")){
				bds->maxlon = atof(value);
			}
			
			// Memory managment 
		  	xmlFree(value); 
			
		 	attribute = attribute->next;
		}
		
		// Display the map boundaries
		print_bounds(*bds);
	}	
	    
	// The xml node is an OSM node
	if (!strcmp(n->name,"node")){
		
		// Initialize
		Node *nd = (Node *) malloc(sizeof(Node));
		
		// We extract the attributes of a node
		xmlAttr* attribute = n->properties;
		while(attribute)
		{
			xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			
			if(!strcmp(attribute->name,"id")){
				nd->id = atoi(value);
			}
			else if (!strcmp(attribute->name,"lat")){
				nd->lat = atof(value);	
			}
			else if (!strcmp(attribute->name,"lon")){
				nd->lon = atof(value);
			}
			else if (!strcmp(attribute->name,"visible")){
				// Check if the node is visible
				if (strcmp(value,"true") == 0) {
					nd->visible = true;
				} else {
					nd->visible = false;
				}				
			}
			
			// Memory managment 
		  	xmlFree(value); 
			
		 	attribute = attribute->next;
		}
		
		// We extract the tags of a node
		xmlNodePtr n_fils;
		nd->nb_tags = 0;
		nd->tags = (Tag *)malloc(sizeof(Tag));
		int size_tags = 1;
		for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next) {

			xmlAttr* attributes_fils = n_fils->properties;
			
			if (!strcmp(n_fils->name,"tag")){
				Tag *tg = (Tag *) malloc(sizeof(Tag));
				char *key;
				char *val;
				
				while(attributes_fils)
				{
					xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
					if (!strcmp(attributes_fils->name,"k")){
						// Key
						key = malloc(strlen(value_fils)+1);
						strcpy(key, value_fils);
					} else {
						// Value
						val = malloc(strlen(value_fils)+1);
						strcpy(val, value_fils);
					}
					
					// Memory managment 
					xmlFree(value_fils);
					
		 			attributes_fils = attributes_fils->next;
				}
				
				// If there is no space anymore in the tags array, we double it
				if (nd->nb_tags == size_tags){
					// Double the size of the tags array
					size_tags *= 2;
					nd->tags = (Tag *) realloc(nd->tags, size_tags * sizeof(Tag));
// 					printf("realloc taille: %d\n", size_tags);
					if (nd->tags == NULL){
						fprintf(stderr, "Echec realloc\n");
						exit (EXIT_FAILURE);
					}
				}
				
				// Add the new tag to the current node
				tg->key = key;
				tg->val = val;
// 				print_tag(*tg);
				nd->tags[nd->nb_tags] = *tg;
				nd->nb_tags++;
			}
		}
	
		// Add the current node to the hashtable
		add_node(nd);
		print_node(*nd);
		
	}
	
	// The xml node is an OSM way
 	else if (!strcmp(n->name,"way")){
		
		// Initialize
		Way *way = (Way *) malloc(sizeof(Way));
 		
		// We extract the attributes of a way
		xmlAttr* attribute = n->properties;
		while(attribute)
		{
			xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			
			if(!strcmp(attribute->name,"id")){
				way->id = atoi(value);
			}
			else if (!strcmp(attribute->name,"visible")){
				// Check if the node is visible
				if (strcmp(value,"true") == 0) {
					way->visible = true;
				} else {
					way->visible = false;
				}				
			}
			
			// Memory managment 
		  	xmlFree(value); 
		 	attribute = attribute->next;
		}
 		
		xmlNodePtr n_fils;
		
		// We extract the nodes and tags of a way
		// Should we increase the default value ?
		way->nb_tags = 0;
		way->tags = (Tag *) malloc(sizeof(Tag));
		int size_tags = 1;
		way->nb_nds = 0;
		way->nds = (Node *) malloc(sizeof(Node));
		int size_nds = 1;

		for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next) {

			xmlAttr* attributes_fils = n_fils->properties;
			// Process child nodes
			if (!strcmp(n_fils->name,"nd")){
				int id;
				Node *nd = (Node *) malloc(sizeof(Node));
				
				while(attributes_fils)
				{
					xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
					if (!strcmp(attributes_fils->name,"ref")){
						// Node ID
						id = atoi(value_fils);
					}
					
					// Memory managment 
					xmlFree(value_fils);
					
		 			attributes_fils = attributes_fils->next;
				}
				
				// If there is no space anymore in the nodes array, we double it
				if (way->nb_nds == size_nds){
					// Double the size of the nodes array
					size_nds *= 2;
					way->nds = (Node *) realloc(way->nds, size_nds * sizeof(Node));
// 					printf("realloc taille: %d\n", size_nds);
					if (way->nds == NULL){
						fprintf(stderr, "Echec realloc\n");
						exit (EXIT_FAILURE);
					}
				}
				
				// Add the node to the current way
				// We get a pointer to a node from the hashtable of nodes
				nd = find_node(id);
				way->nds[way->nb_nds] = *nd;
				way->nb_nds++;
				
			}
			// Process tags
			else if (!strcmp(n_fils->name,"tag")){
				Tag *tg = (Tag *) malloc(sizeof(Tag));
				char *key;
				char *val;
				
				while(attributes_fils)
				{
					xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
					if (!strcmp(attributes_fils->name,"k")){
						// Key
						key = malloc(strlen(value_fils)+1);
						strcpy(key, value_fils);
					} else {
						// Value
						val = malloc(strlen(value_fils)+1);
						strcpy(val,  value_fils);
					}
					
					// Memory managment 
					xmlFree(value_fils);
					
		 			attributes_fils = attributes_fils->next;
				}

				// If there is no space anymore in the tags array, we double it
				if (way->nb_tags == size_tags){
					// Double the size of the tags array
					size_tags *= 2;
					way->tags = (Tag *) realloc(way->tags, size_tags * sizeof(Tag));
// 					printf("realloc taille: %d\n", size_tags);
					if (way->tags == NULL){
						fprintf(stderr, "Echec realloc\n");
						exit (EXIT_FAILURE);
					}
				}
				
				// Add the new tag to the current node
				tg->key = key;
				tg->val = val;
// 				print_tag(*tg);
				way->tags[way->nb_tags] = *tg;
				way->nb_tags++;
				
			}
		}

		// Add the current way to the hashtable
		add_way(way);
		print_way(*way);
 	}
    }
    
	// Display the total number of nodes added to the hashtable 
 	unsigned int num_nodes;
	num_nodes = HASH_COUNT(nodes);
	printf("There are %u nodes in the table\n", num_nodes);
	
	// Display the total number of ways added to the hashtable 
 	unsigned int num_ways;
	num_ways = HASH_COUNT(ways);
	printf("There are %u ways in the table\n", num_ways);
}

void afficher_noeud(xmlNodePtr noeud) {
    if (noeud->type == XML_ELEMENT_NODE) {
        xmlChar *chemin = xmlGetNodePath(noeud);
        if (noeud->children != NULL && noeud->children->type == XML_TEXT_NODE) {
            xmlChar *contenu = xmlNodeGetContent(noeud);
            printf("%s -> %s\n", chemin, contenu);
            xmlFree(contenu);
        } else {
            printf("%s\n", chemin);
        }
        xmlFree(chemin);
    }
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

int drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int width, int r, int g, int b, int a)
{
	thickLineRGBA(renderer,x1,y1,x2,y2,width,r,g,b,a);
	return 0;
}

int drawRoad(SDL_Renderer *renderer,SDL_Point *points,int width,int nbPoints,int r, int g, int b, int a)
{
	int i;
	for(i = 0;i<nbPoints;i++)
	{
		if(i == nbPoints-1)
		{
			break;
		}
		thickLineRGBA(renderer,points[i].x,points[i].y,points[i+1].x,points[i+1].y,width,r,g,b,a);
	}
	return 0;
}

int render_min()
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return -1;
    }
	window = SDL_CreateWindow("OpenStreetMap - Renderer minimal",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH, HEIGHT,SDL_WINDOW_SHOWN); //Création de la fenêtre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Création du renderer associé a la fenêtre

	SDL_SetRenderDrawColor(renderer,242,239,233,0);//La couleur du fond
	SDL_RenderClear(renderer);//Application de la couleur choisis

	SDL_RenderPresent(renderer);

	//boucle for avec drawRoad() sur chaque way
	



	pause();

    //TTF_Quit();
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
	
}

int main(int argc, char *argv[]) {
    
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s <file>.osm\n",argv[0]);
		exit(0);
	}
	xmlDocPtr doc;
    xmlNodePtr racine;
 
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
    parcours_prefixe(racine, afficher_noeud);

	render_min();

    // Libération de la mémoire
    xmlFreeDoc(doc);

    return EXIT_SUCCESS;
}
