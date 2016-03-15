#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>

#include "data.h"

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

int main() {
    xmlDocPtr doc;
    xmlNodePtr racine;
 
    // Ouverture du document
    xmlKeepBlanksDefault(0); // Ignore les noeuds texte composant la mise en forme
    doc = xmlParseFile("../maps_test/02_paris_place_des_vosges.osm");
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

    // Libération de la mémoire
    xmlFreeDoc(doc);

    return EXIT_SUCCESS;
}
