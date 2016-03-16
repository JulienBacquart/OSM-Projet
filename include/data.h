/**
 * \file data.h
 * \brief Header qui contient les structures de nos données
 * \authors
 * \version 0.1
 *
 * Fichier qui contient toutes les structures et maccros qui servent à stocker les données parser.
 *
 */

# include <stdbool.h>

#include "uthash.h"

#define MAX_NODE_IN_WAY 2000
#define MIN_LAT −90.0000000
#define MAX_LAT 90.0000000
#define MIN_LON −180.0000000
#define MAX_LON 180.0000000

typedef struct{
	char * key;
	char * val;
} Tag;

typedef struct{
	int id;		// key
	bool visible;
	double lat;
	double lon;
	int nb_tags;
	Tag *tags;
	UT_hash_handle hh;	// makes this structure hashable
} Node;

typedef struct{
	int id;		// key
	bool visible;
	Node * nds;
	int nb_nds;
	Tag * tags;
	int nb_tags;
	UT_hash_handle hh;	// makes this structure hashable
} Way;

typedef struct{
	double minlat;
	double minlon;
	double maxlat;
	double maxlon;
} Bounds;
