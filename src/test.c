#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>

#include "data.h"

typedef void (*fct_parcours_t)(xmlNodePtr);

void print_tag(Tag tag){
	printf("Tag k: %s, v: %s\n", tag.key, tag.val);
}

void print_node(Node node){
	// For lat et long we need a precision to the 7 decimal place
	printf("Node id: %d free_tags=%d lat=%.7f lon=%.7f visible=%d\n", node.id, node.free_tags, node.lat, node.lon, node.visible);
	int i;
	for(i = 0; i< node.free_tags; i++){
		print_tag(node.tags[i]);
	}
}

void parcours_prefixe(xmlNodePtr noeud, fct_parcours_t f) {
    Node nodes[177];   //ajouter malloc ici
    Way ways[1000];
    int ways_i=0;
	
    int nodes_i=0;
    xmlNodePtr n;
    
    // We traverse the xml tree looking at each node
    for (n = noeud->children; n != NULL; n = n->next) {
	// Case node
	if (!strcmp(n->name,"node")){
		
		int node_id;
		double node_lat;
		double node_lon;
		bool node_visible;
		
		nodes[nodes_i].free_tags= 0;	// should be changed
		
		// We extract the attributes of a node
		xmlAttr* attribute = n->properties;
		while(attribute)
		{
			xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			
			if(!strcmp(attribute->name,"id")){
				node_id= atoi(value);
			}
			else if (!strcmp(attribute->name,"lat")){
				node_lat= atof(value);	
			}
			else if (!strcmp(attribute->name,"lon")){
				node_lon = atof(value);
			}
			else if (!strcmp(attribute->name,"visible")){
				// Check if the node is visible
				if (strcmp(value,"true") == 0) {
					node_visible = true;
				} else {
					node_visible = false;
				}				
			}
			
			// Memory managment 
		  	xmlFree(value); 
			
		 	attribute = attribute->next;
		}
		
		// We extract the tags of a node
		xmlNodePtr n_fils;
		for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next) {

			xmlAttr* attributes_fils = n_fils->properties;
			
			if (!strcmp(n_fils->name,"tag")){
				char * key;
				char * val;
				
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
				
				// should be changed
				nodes[nodes_i].tags[nodes[nodes_i].free_tags].key= key;
				nodes[nodes_i].tags[nodes[nodes_i].free_tags].val= val;
				nodes[nodes_i].free_tags++;
			}
		}
		
		// should be changed
		nodes[nodes_i].id= node_id;
		nodes[nodes_i].lat= node_lat;
		nodes[nodes_i].lon= node_lon;
		nodes[nodes_i].visible = node_visible;
	
		//print_node(nodes[nodes_i]);
		nodes_i++;
		
	}

 	else if (!strcmp(n->name,"way")){
 		printf("************way found**************\n");
 		// We get the attributes of the way
 		int way_id;
		bool way_visible;

		ways[ways_i].nb_tags= 0;
		ways[ways_i].nb_nds= 0;

 		//xmlNodePtr n_fils;
 		
		xmlAttr* attribute = n->properties;
		while(attribute)
		{
			xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			
			if(!strcmp(attribute->name,"id")){
				way_id= atoi(value);
			}
			else if (!strcmp(attribute->name,"visible")){
				// Check if the node is visible
				if (strcmp(value,"true") == 0) {
					way_visible = true;
				} else {
					way_visible = false;
				}				
			}
			
			// Memory managment 
		  	xmlFree(value); 
		 	attribute = attribute->next;
		}
 		ways[ways_i].id= way_id;
		ways[ways_i].visible= way_visible;
 		
		xmlNodePtr n_fils;		
		
		ways[ways_i].tags = malloc(10*sizeof(Tag));
		ways[ways_i].nds = malloc(10*sizeof(int));
		ways[ways_i].nb_tags =10;
		ways[ways_i].nb_nds=10;
		int nb_added_tags = 0;
		int nb_added_nds = 0;
		if( ways[ways_i].tags == NULL )
		{
		     fprintf(stderr,"Allocation impossible");
		     exit(EXIT_FAILURE);
		}

		for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next) {

			xmlAttr* attributes_fils = n_fils->properties;
			// Process nodes
			if (!strcmp(n_fils->name,"nd")){
				char * val;
				nb_added_nds++;				
				while(attributes_fils)
				{
					xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
					if (!strcmp(attributes_fils->name,"ref")){
						// Key
						val = malloc(strlen(value_fils)+1);
						strcpy(val, value_fils);
					}
					
					// Memory managment 
					xmlFree(value_fils);
					
		 			attributes_fils = attributes_fils->next;
				}
				if(nb_added_nds==ways[ways_i].nb_nds){
					//printf("On realloc");
					ways[ways_i].nds = realloc(ways[ways_i].nds, ways[ways_i].nb_nds*ways[ways_i].nb_nds*sizeof(int));
					ways[ways_i].nb_nds = ways[ways_i].nb_nds*ways[ways_i].nb_nds;
				}
				ways[ways_i].nds[nb_added_nds-1]= atoi(val);
							
			}
			// Process tags
			else if (!strcmp(n_fils->name,"tag")){
				char * key;
				char * val;
								
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
				nb_added_tags++;
				//printf("nb_tags: %d; added_tags: %d \n",nb_added_tags,ways[ways_i].nb_tags);
				if(nb_added_tags==ways[ways_i].nb_tags){
					//printf("On realloc");
					ways[ways_i].tags = realloc(ways[ways_i].tags, ways[ways_i].nb_tags*ways[ways_i].nb_tags*sizeof(Tag));
					ways[ways_i].nb_tags = ways[ways_i].nb_tags*ways[ways_i].nb_tags;
				}
				
				// should be changed
				ways[ways_i].tags[nb_added_tags-1].key= key;
				ways[ways_i].tags[nb_added_tags-1].val= val;
				//wayss[ways_i].nb_tags++;
				
			}
		}
		int z;
		for(z=0;z<nb_added_nds;z++){
			//print_tag(ways[ways_i].tags[z]);
			printf("nd = %d\n",ways[ways_i].nds[z]);
		}
		ways_i++;
 	}
 	
// 	else{
		
// 	}
       // f(n);

    }
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
    doc = xmlParseFile("../maps_test/01_denver_lafayette.osm");
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
