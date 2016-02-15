/**
 * \file data.h
 * \brief Header qui contient les structures de nos données
 * \author Adel.Z
 * \version 0.1
 *
 * Fichier qui contient toutes les structures et maccros qui servent à stocker les données parser.
 *
 */


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
	int lat;
	int lon;
	int nb_tags;
	Tag * tags;
}Node;
