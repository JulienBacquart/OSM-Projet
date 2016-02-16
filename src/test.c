#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>

#include "data.h"

typedef void (*fct_parcours_t)(xmlNodePtr);

void print_tag(Tag tag){
	printf("TAAAAAAAAAAAAAG %s = %s\n", tag.key,tag.val);
}

void print_node(Node node){
	int i;
	printf("id=%d \n free_tags=%d \n lat=%f\n lon=%f \n", node.id, node.free_tags, node.lat, node.lon);
	for(i = 0; i< node.free_tags; i++){
		print_tag(node.tags[i]);
	}
}

void parcours_prefixe(xmlNodePtr noeud, fct_parcours_t f) {
    Node nodes[177];   //ajouter malloc ici
    int nodes_i=0;
    xmlNodePtr n;
    
    for (n = noeud->children; n != NULL; n = n->next) {
	if (!strcmp(n->name,"node")){
		printf("************node found**************\n");
		// RECUPERER LES ATTRIBUTS DU NOEUD
		xmlAttr* attribute = n->properties;
		int node_id;
		xmlNodePtr n_fils;
		float node_lat;
		float node_lon;
		nodes[nodes_i].free_tags= 0;

		while(attribute)
		{
			xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			if(!strcmp(attribute->name,"id")){
				node_id= atoi(value);
			}
			else if(!strcmp(attribute->name,"lat")){
				node_lat= atof(value);	
			}
			else if(!strcmp(attribute->name,"lon")){
				node_lon = atof(value);
			}
			
		  	//printf(" %s", attribute->name);
		  	//printf("=%s ;", value);
			
		  	xmlFree(value); 
		 	attribute = attribute->next;
		}
		//printf("id = %d; lon =%f; lat=%f \n",node_id,node_lon,node_lat);
		
		//CHERCHER LES TAGS
		for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next) {
			xmlAttr* attributes_fils = n_fils->properties;
			if (!strcmp(n_fils->name,"tag")){
				char * key;
				char * val;
				//printf("************************************************************On traite le tag\n");
				while(attributes_fils)
				{
					xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					if(!strcmp(attributes_fils->name,"k")){
						key = malloc(strlen(value_fils)+1);
						strcpy(key,  value_fils);
						//printf("%s =",key);
					}
					else{
						val = malloc(strlen(value_fils)+1);
						strcpy(val,  value_fils);
						//printf("%s\n",val);
					}
					xmlFree(value_fils);
		 			attributes_fils = attributes_fils->next;
				}
				nodes[nodes_i].tags[nodes[nodes_i].free_tags].key= key;
				nodes[nodes_i].tags[nodes[nodes_i].free_tags].val= val;
				nodes[nodes_i].free_tags++;
				
			}
		}	
		nodes[nodes_i].id= node_id;
		nodes[nodes_i].lat= node_lat;
		nodes[nodes_i].lon= node_lon;
	
		print_node(nodes[nodes_i]);
		nodes_i++;
		
	}

	else if (!strcmp(n->name,"way")){
		printf("on traite la way\n");
	}
	
	else{
		
	}
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
