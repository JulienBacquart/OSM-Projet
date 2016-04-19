/**
 * \file data.h
 * \brief Contient les signatures et directives de préprocesseur pour le calcul et le stockage des données xml
 * \author Adel.Z Julien.B Charles.R
 */

#ifndef _DATA_H_
#define _DATA_H_

#include <stdbool.h>

#include "uthash.h"

#define MAX_NODE_IN_WAY 2000
#define MIN_LAT −90.0000000
#define MAX_LAT 90.0000000
#define MIN_LON −180.0000000
#define MAX_LON 180.0000000

/**
 * \struct Tag
 * \brief permet de stocker les tags en fonctions de leurs clé et de leur valeur
 *
 */
typedef struct{
	char * key;
	char * val;
} Tag;

/**
 * \struct Node
 * \brief permet de stocker les Node du fichier xml et le rendre hashable
 *
 * permet de récuperer les attributs d'un type Node et de le stocker dans cette structure, l'id, le champ visible, la latitude, la longitude, le nombre de tags
 */
typedef struct{
	int id;		// key
	bool visible;
	double lat;
	double lon;
	int nb_tags;
	Tag *tags;
	UT_hash_handle hh;	// makes this structure hashable
} Node;

/**
 * \struct Way
 * \brief permet de stocker les Way du fichier xml et la rendre hashable
 *
 * permet de récuperer les attributs d'un type way et de le stocker dans cette structure, l'id, les noeuds, le nombre de noeuds, les tags et le nombre de tags
 */
typedef struct{
	int id;		// key
	bool visible;
	Node * nds;
	int nb_nds;
	Tag * tags;
	int nb_tags;
	UT_hash_handle hh;	// makes this structure hashable
} Way;

/**
 * \struct Bounds
 * \brief permet de stocker les bounds du fichier xml
 *
 * permet de récuperer les latitudes et longitudes maximal et minimal de la map définit pas un fichier xml
 */
typedef struct{
	double minlat;
	double minlon;
	double maxlat;
	double maxlon;
} Bounds;

/**
 * \struct Relation
 * \brief permet de stocker les relations du fichier xml
 */
typedef struct{
	int id;		// key
	bool visible;
	Tag * tags;
	int nb_tags;
// 	Member members;
	int nb_members;
	UT_hash_handle hh;	// makes this structure hashable
} Relation;

/**
 * \struct Map
 * \brief permet de stocker les détails de la map
 */
typedef struct{
	Node *h_nodes;		// hastable containing all the nodes
	Way *h_ways;		// hashtable containing all the ways
	Bounds *m_bds;		// contains the map boundaries
	Relation *h_relations; // hashable containing all the relations
} Map;

#endif // #ifndef _DATA_H_