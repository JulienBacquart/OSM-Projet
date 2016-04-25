/**
 * \file parsexml.c
 * \brief Contient des fonctions qui permettent de parser un fichier xml
 * \author Adel.Z Julien.B Charles.R
 */

#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>

#include "../include/parsexml.h"
#include "../include/graphics.h"

// The hashtable holding the nodes
Node *h_nodes = NULL;    /* important! initialize to NULL */
// The hashtable holding the ways
Way *h_ways = NULL;    /* important! initialize to NULL */
// The hashtable holding the relations
Relation *h_relations = NULL;    /* important! initialize to NULL */


/**
 * \fn print_bounds(Bounds bds)
 * \brief Fonction qui affiche sur la console la valeur d'un bounds
 *
 * \param bds le bounds à afficher
 * \return void
 */
void print_bounds(Bounds bds){
	printf("Bound minlat: %.7f , minlon: %.7f , maxlat: %.7f , maxlon: %.7f\n", bds.minlat, bds.minlon, bds.maxlat, bds.maxlon);
}


/**
 * \fn print_tag(Tag tag)
 * \brief Fonction qui affiche sur la console la valeur d'un tag
 *
 * \param tag le tag à afficher
 * \return void
 */
void print_tag(Tag tag){
	printf("Tag k: %s, v: %s\n", tag.key, tag.val);
}


/**
 * \fn print_node(Node node)
 * \brief Fonction qui affiche sur la console la valeur d'un node
 *
 * \param node le node à afficher
 * \return void
 */
void print_node(Node node){
	// For lat et long we need a precision to the 7 decimal place
	printf("Node id: %d visible=%d lat=%.7f lon=%.7f nb_tags=%d\n", node.id, node.visible, node.lat, node.lon, node.nb_tags);
	int i;
	for(i = 0; i< node.nb_tags; i++){
		print_tag(node.tags[i]);
	}
}

/**
 * \fn print_Member(Member member)
 * \brief Display a string representation of a member of a relation
 *
 * \param node the member to be displayed
 * \return void
 */
void print_member(Member member){
	printf("Type: %s, Ref: %d, Role: %s\n", member.type, member.ref, member.role);
}

/**
 * \fn print_way(Way way)
 * \brief Fonction qui affiche sur la console la valeur d'une way
 *
 * \param way la way à afficher
 * \return void
 */
void print_way(Way way){
	printf("Way id: %d visible=%d nb_nds=%d nb_tags=%d z-level=%d\n", way.id, way.visible, way.nb_nds, way.nb_tags, way.z_level);
	int i;
	for(i = 0; i< way.nb_tags; i++){
		print_tag(way.tags[i]);
	}
}


/**
 * \fn print_relation(Relation relation)
 * \brief Fonction qui affiche sur la console la valeur d'une relation
 *
 * \param relation la relation a afficher
 * \return void
 */
void print_relation(Relation relation){
	printf("Relation id: %d visible=%d nb_tags=%d nb_members=%d\n", relation.id, relation.visible, relation.nb_tags, relation.nb_members);
	int i;
	for(i = 0; i< relation.nb_members; i++){
		print_member(relation.members[i]);
	}
	for(i = 0; i< relation.nb_tags; i++){
		print_tag(relation.tags[i]);
	}
}


/**
 * \fn print_map(Map map)
 * \brief Fonction qui affiche sur la console la valeur d'une map
 *
 * \param map la map a afficher
 * \return void
 */
void print_map(Map map){
	printf("Map nb_nodes: %u nb_ways: %u nb_relations: %u\n", HASH_COUNT(map.h_nodes), HASH_COUNT(map.h_ways), HASH_COUNT(map.h_relations));
	print_bounds(*map.m_bds);
}


/**
 * \fn find_node(int node_id)
 * \brief Fonction qui trouve un node dans la hashtable en fonction de son id
 *
 * \param node_id l'id d'une node
 * \return Node* le noeud correspondant a l'id
 */
Node *find_node(int node_id) {
    Node *n;

    HASH_FIND_INT(h_nodes, &node_id, n);  /* n: output pointer */
    return n;
}

/**
 * \fn find_way(int way_id)
 * \brief Find a way in the hashtable based on its id.
 *
 * \param way_id id of the way
 * \return Way* the way corresponding to the id
 */
Way *find_way(int way_id) {
    Way *w;

    HASH_FIND_INT(h_ways, &way_id, w);  /* w: output pointer */
    return w;
}


/**
 * \fn add_node(Node *nd)
 * \brief Fonction qui ajoute un node dans la hashtable 'nodes'
 *
 * \param nd un pointeur de node a ajouter a la table de hashage
 * \return void
 */
void add_node(Node *nd) {
	// Maybe check uniqueness ?
	HASH_ADD_INT(h_nodes, id, nd);  /* id: name of key field */
}


/**
 * \fn add_way(Way *way)
 * \brief Fonction qui ajoute une way dans la table de hashage 'ways'
 *
 * \param way un pointeur de Way a ajouter a la table de hashage
 * \return void
 */
void add_way(Way *way) {
	// Maybe check uniqueness ?
	HASH_ADD_INT(h_ways, id, way);  /* id: name of key field */
}


/**
 * \fn add_relation(Relation *relation)
 * \brief Fonction qui ajoute une relation dans la table de hashage 'relations'
 *
 * \param relation un pointeur de Relation a ajouter a la table de hashage
 * \return void
 */
void add_relation(Relation *relation) {
	// Maybe check uniqueness ?
	HASH_ADD_INT(h_relations, id, relation);  /* id: name of key field */
}

/**
 * \fn z_level_sort_function(Way *a, Way *b)
 * \brief Sort two ways based on their z-level 
 *
 * Respect the following contract:
 * It must accept two pointer arguments (the items to compare), and must return an int which is less than zero, zero, or greater than zero, 
 * if the first item sorts before, equal to, or after the second item, respectively. 
 * (This is the same convention used by strcmp or qsort in the standard C library).
 * 
 * \return void
 */
int z_level_sort_function(Way *a, Way *b) {
    return ((a->z_level) - (b->z_level));
}

/**
 * \fn sort_by_z_level()
 * \brief Sort the ways based on their z-level (from smaller to bigger values) 
 *
 * \return void
 */
void sort_by_z_level() {
    HASH_SORT(h_ways, z_level_sort_function);
}

/**
 * \fn parcours_prefixe(xmlNodePtr noeud, fct_parcours_t f)
 * \brief Fonction qui parcours un arbre xml et regarde tous les noeuds
 *
 * \param noeud 
 * \param f 
 * \return Map
 */
Map parcours_prefixe(xmlNodePtr noeud, fct_parcours_t f){
	// Initialize
    xmlNodePtr n;
    Map *map = (Map *) malloc(sizeof(Map));
    if (map == NULL){
	    fprintf(stderr, "Echec alloc\n");
	    exit (EXIT_FAILURE);
    }
    
    // We traverse the xml tree looking at each xml node
    for (n = noeud->children; n != NULL; n = n->next)
	{
		// The xml node is 'bound'
		if (!strcmp(n->name, "bounds"))
		{
			// Initialize
			Bounds *bds = (Bounds *) malloc(sizeof(Bounds));
			if (bds == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
		
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
		
			map->m_bds = bds;
			// Display the map boundaries
			// print_bounds(*bds);
		}	
	    
		// The xml node is an OSM node
		if (!strcmp(n->name,"node"))
		{
			// Initialize
			Node *nd = (Node *) malloc(sizeof(Node));
			if (nd == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
		
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
			if (nd->tags == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
			int size_tags = 1;
			for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next)
			{
				xmlAttr* attributes_fils = n_fils->properties;
				if (!strcmp(n_fils->name,"tag"))
				{
					Tag *tg = (Tag *) malloc(sizeof(Tag));
					if (tg == NULL){
						fprintf(stderr, "Echec alloc\n");
						exit (EXIT_FAILURE);
					}
					char *key;
					char *val;
				
					while(attributes_fils)
					{
						xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
						if (!strcmp(attributes_fils->name,"k")){
							// Key
							key = malloc(strlen(value_fils)+1);
							if (key == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(key, value_fils);
						} else {
							// Value
							val = malloc(strlen(value_fils)+1);
							if (val == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(val, value_fils);
						}
					
						// Memory managment 
						xmlFree(value_fils);
					
			 			attributes_fils = attributes_fils->next;
					}
				
					// If there is no space anymore in the tags array, we double it
					if (nd->nb_tags == size_tags)
					{
						// Double the size of the tags array
						size_tags *= 2;
						nd->tags = (Tag *) realloc(nd->tags, size_tags * sizeof(Tag));
						//printf("realloc taille: %d\n", size_tags);
						if (nd->tags == NULL){
							fprintf(stderr, "Echec realloc\n");
							exit (EXIT_FAILURE);
						}
					}
				
					// Add the new tag to the current node
					tg->key = key;
					tg->val = val;
					//print_tag(*tg);
					nd->tags[nd->nb_tags] = *tg;
					nd->nb_tags++;
				}
			}
	
			// Add the current node to the hashtable
			add_node(nd);
			//print_node(*nd);
		}
		// The xml node is an OSM way
	 	else if (!strcmp(n->name,"way"))
		{
			// Initialize
			Way *way = (Way *) malloc(sizeof(Way));
			if (way == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
	 		
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
			if (way->tags == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
			int size_tags = 1;
			way->nb_nds = 0;
			way->nds = (Node *) malloc(sizeof(Node));
			if (way->nds == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
			int size_nds = 1;
			way->z_level = 0;
			
			for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next)
			{
				xmlAttr* attributes_fils = n_fils->properties;
				// Process child nodes
				if (!strcmp(n_fils->name,"nd"))
				{
					int id;
					Node *nd = (Node *) malloc(sizeof(Node));
					if (nd == NULL){
						fprintf(stderr, "Echec alloc\n");
						exit (EXIT_FAILURE);
					}
				
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
					if (way->nb_nds == size_nds)
					{
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
				else if (!strcmp(n_fils->name,"tag"))
				{
					Tag *tg = (Tag *) malloc(sizeof(Tag));
					char *key;
					char *val;
				
					while(attributes_fils)
					{
						xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
						if (!strcmp(attributes_fils->name,"k")){
							// Key
							key = malloc(strlen(value_fils)+1);
							if (key == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							
							strcpy(key, value_fils);
							
							// Assign z-value
							// Buildings
							if (!strcmp(key,"building")){
								way->z_level = BUILDING_Z_LEVEL;
							}
						} else {
							// Value
							val = malloc(strlen(value_fils)+1);
							if (val == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(val, value_fils);
							
							// Assign z-value
							// Roads
							if (!strcmp(val,"motorway")){
								way->z_level = MOTORWAY_Z_LEVEL;
							} else if (!strcmp(val,"motorway_link")){
								way->z_level = MOTORWAY_LINK_Z_LEVEL;
							} else if (!strcmp(val,"primary")){
								way->z_level = PRIMARY_Z_LEVEL;
							} else if (!strcmp(val,"primary_link")){
								way->z_level = PRIMARY_LINK_Z_LEVEL;
							} else if (!strcmp(val,"secondary")){
								way->z_level = SECONDARY_Z_LEVEL;
							} else if (!strcmp(val,"secondary_link")){
								way->z_level = SECONDARY_LINK_Z_LEVEL;
							} else if (!strcmp(val,"tertiary")){
								way->z_level = TERTIARY_Z_LEVEL;
							} else if (!strcmp(val,"primary_link")){
								way->z_level = TERTIARY_LINK_Z_LEVEL;
							} else if (!strcmp(val,"unclassified")){
								way->z_level = UNCLASSIFIED_Z_LEVEL;
							} else if (!strcmp(val,"residential")){
								way->z_level = RESIDENTIAL_Z_LEVEL;
							} else if (!strcmp(val,"service")){
								way->z_level = SERVICE_Z_LEVEL;
							} else if (!strcmp(val,"living_street")){
								way->z_level = LIVING_STREET_Z_LEVEL;
							} else if (!strcmp(val,"pedestrian")){
								way->z_level = PEDESTRIAN_Z_LEVEL;
							} else if (!strcmp(val,"track")){
								way->z_level = TRACK_Z_LEVEL;
							} else if (!strcmp(val,"footway")){
								way->z_level = FOOTWAY_Z_LEVEL;
							} else if (!strcmp(val,"steps")){
								way->z_level = STEPS_Z_LEVEL;
							} else if (!strcmp(val,"path")){
								way->z_level = PATH_Z_LEVEL;
							} else if (!strcmp(val,"cycleway")){
								way->z_level = CYCLEWAY_Z_LEVEL;
							} 
							// Other
							else if (!strcmp(val,"riverbank")){
								way->z_level = RIVER_Z_LEVEL;
							} else if (!strcmp(val,"grass")){
								way->z_level = GRASS_Z_LEVEL;
							} else if (!strcmp(val,"coastline")){
								way->z_level = COASTLINE_Z_LEVEL;
							}
							
							
						}
						
					
						// Memory managment 
						xmlFree(value_fils);
					
			 			attributes_fils = attributes_fils->next;
					}

					// If there is no space anymore in the tags array, we double it
					if (way->nb_tags == size_tags)
					{
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
	// 		print_way(*way);
 		}
 		
 		// The xml node is an OSM relation
	 	else if (!strcmp(n->name,"relation"))
		{
			// Initialize
			Relation *relation = (Relation *) malloc(sizeof(Relation));
			if (relation == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
	 		
			// We extract the attributes of a relation
			xmlAttr* attribute = n->properties;
			while(attribute)
			{
				xmlChar* value = xmlNodeListGetString(n->doc, attribute->children, 1);
			
				if(!strcmp(attribute->name,"id")){
					relation->id = atoi(value);
				}
				else if (!strcmp(attribute->name,"visible")){
					// Check if the relation is visible
					if (strcmp(value,"true") == 0) {
						relation->visible = true;
					} else {
						relation->visible = false;
					}				
				}
			
				// Memory managment 
			  	xmlFree(value); 
			 	attribute = attribute->next;
			}
	 		
			xmlNodePtr n_fils;
		
			// We extract the members and tags of a relation
			// Should we increase the default value ?
			relation->nb_tags = 0;
			relation->tags = (Tag *) malloc(sizeof(Tag));
			if (relation->tags == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
			int size_tags = 1;
			relation->nb_members = 0;
			relation->members = (Member *) malloc(sizeof(Member));
			if (relation->members == NULL){
				fprintf(stderr, "Echec alloc\n");
				exit (EXIT_FAILURE);
			}
			int size_members = 1;

			for (n_fils = n->children; n_fils != NULL; n_fils = n_fils->next)
			{
				xmlAttr* attributes_fils = n_fils->properties;
				// Process child member
				if (!strcmp(n_fils->name,"member"))
				{
					Member *mb = (Member *) malloc(sizeof(Member));
					if (mb == NULL){
							fprintf(stderr, "Echec alloc\n");
							exit (EXIT_FAILURE);
					}
				
					while(attributes_fils)
					{
						xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
						if (!strcmp(attributes_fils->name,"ref")){
							// ref
							mb->ref = atoi(value_fils);
						}
						else if (!strcmp(attributes_fils->name,"type")){
							// type
							mb->type = malloc(strlen(value_fils)+1);
							if (mb->type == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(mb->type, value_fils);
						}
						else if (!strcmp(attributes_fils->name,"role")){
							// role
							mb->role = malloc(strlen(value_fils)+1);
							if (mb->role == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(mb->role, value_fils);
						}
					
						// Memory managment 
						xmlFree(value_fils);
			 			attributes_fils = attributes_fils->next;
					}
				
					// If there is no space anymore in the members array, we double it
					if (relation->nb_members == size_members)
					{
						// Double the size of the members array
						size_members *= 2;
						relation->members = (Member *) realloc(relation->members, size_members * sizeof(Member));
// 						printf("realloc taille: %d\n", size_members);
						if (relation->members == NULL){
							fprintf(stderr, "Echec realloc\n");
							exit (EXIT_FAILURE);
						}
					}
				
					// Add the member to the current relation
					relation->members[relation->nb_members] = *mb;
					relation->nb_members++;
				
				}
				// Process tags
				else if (!strcmp(n_fils->name,"tag"))
				{
					Tag *tg = (Tag *) malloc(sizeof(Tag));
					if (tg == NULL){
							fprintf(stderr, "Echec alloc\n");
							exit (EXIT_FAILURE);
					}
					char *key;
					char *val;
				
					while(attributes_fils)
					{
						xmlChar* value_fils = xmlNodeListGetString(n_fils->doc, attributes_fils->children, 1);
					
						if (!strcmp(attributes_fils->name,"k")){
							// Key
							key = malloc(strlen(value_fils)+1);
							if (key == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(key, value_fils);
						} else {
							// Value
							val = malloc(strlen(value_fils)+1);
							if (val == NULL){
								fprintf(stderr, "Echec alloc\n");
								exit (EXIT_FAILURE);
							}
							strcpy(val,  value_fils);
						}
					
						// Memory managment 
						xmlFree(value_fils);
					
			 			attributes_fils = attributes_fils->next;
					}

					// If there is no space anymore in the tags array, we double it
					if (relation->nb_tags == size_tags)
					{
						// Double the size of the tags array
						size_tags *= 2;
						relation->tags = (Tag *) realloc(relation->tags, size_tags * sizeof(Tag));
	// 					printf("realloc taille: %d\n", size_tags);
						if (relation->tags == NULL){
							fprintf(stderr, "Echec realloc\n");
							exit (EXIT_FAILURE);
						}
					}
				
					// Add the new tag to the current node
					tg->key = key;
					tg->val = val;
	// 				print_tag(*tg);
					relation->tags[relation->nb_tags] = *tg;
					relation->nb_tags++;
				
				}
			}

			// Add the current relation to the hashtable
			add_relation(relation);
// 			print_relation(*relation);
 		}
  
		map->h_nodes = h_nodes;
		// Sort by z-level
		sort_by_z_level();
		map->h_ways = h_ways;
		map->h_relations = h_relations;
		//print_map(*map);
	}
	return *map;
}


/**
 * \fn afficher_noeud(xmlNodePtr noeud)
 * \brief Fonction qui affiche un noeud
 *
 * \param noeud le noeud a afficher
 * \return void
 */
void afficher_noeud(xmlNodePtr noeud)
{
    if (noeud->type == XML_ELEMENT_NODE)
	{
        xmlChar *chemin = xmlGetNodePath(noeud);
        if (noeud->children != NULL && noeud->children->type == XML_TEXT_NODE)
		{
            xmlChar *contenu = xmlNodeGetContent(noeud);
            printf("%s -> %s\n", chemin, contenu);
            xmlFree(contenu);
        } 
		else
		{
            printf("%s\n", chemin);
        }
        xmlFree(chemin);
    }
}