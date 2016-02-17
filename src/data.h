/**
 * \file data.h
 * \brief Header qui contient les structures de nos données
 * \author Adel.Z
 * \version 0.1
 *
 * Fichier qui contient toutes les structures et maccros qui servent à stocker les données parser.
 *
 */

#define MAX_NODE_IN_WAY 2000
#define MIN_LAT −90.0000000
#define MAX_LAT 90.0000000
#define MIN_LON −180.0000000
#define MAX_LON 180.0000000

typedef struct{
	char * key;
	char * val;
}Tag;

typedef struct{
	int id;
	int visible; //boolean
	int * nds;
	int nb_nds;
	Tag * tags;
	int nb_tags;
	
}Way;

typedef struct{
	int id;
	int free_tags;
	double lat;
	double lon;
	
	Tag tags[10];
}Node;

typedef struct{
	double minlat;
	double minlon;
	double maxlat;
	double maxlon;
}Bounds;
