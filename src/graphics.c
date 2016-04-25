/**
 * \file graphics.c
 * \brief Contient les signatures et directives de préprocesseur pour la partie graphique(affichage)
 * \author Adel.Z Julien.B Charles.R
 */

#include "../include/graphics.h"
#include "../include/calcul.h"


/**
 * \fn html_to_rgba(char *str, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
 * \brief Fonction qui convertit une écriture de couleur html héxadécimal en 4 entiers rgba
 *
 * \param str la chaine html héxadécimal
 * \param r la couleur rouge a remplir grace a la chaine extraite
 * \param g la couleur verte a remplir grace a la chaine extraite
 * \param b la couleur bleu a remplir grace a la chaine extraite
 * \param a la transparence alpha a remplir grace a la chaine extraite
 * \return int si tout c'est bien passé
 */
int html_to_rgba(char *str, Uint8* r, Uint8* g, Uint8* b, Uint8* a){
	Uint32 color;
	
	int res = sscanf(str, "#%"SCNx32, &color);
	if (1 == res) {
		// color now contains the value from the string	
		*r = (Uint8) ((color & 0xFF0000) >> 16);// Extract the RR byte
		*g = (Uint8) ((color >> 8) & 0xFF);	// Extract the GG byte
		*b = (Uint8) ((color) & 0xFF);		// Extract the BB byte
		*a = (Uint8) (0xFF);			// Always 255 by default
	}
	
	return res;
}



/**
 * \fn doPause()
 * \brief Fonction qui permet de mettre en pause le programme et d'attendre l'évènement de fermeture de la fenêtre
 *
 * \return void
 */
void doPause()
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


/**
 * \fn drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width, char* color)
 * \brief Fonction qui permet de dessiner une route sur la frame
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way la way qui contient le nombre de noeud de la route
 * \param h_nodes la table de hashage 'nodes'
 * \param m_bds les bounds de la map
 * \param draw_width l'épaisseur de la route
 * \param color la couleur de la route au format html
 * \return int si tout c'est bien passé
 */
int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width, char* color){
	int i;
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
	html_to_rgba(color, r, g, b, a);
	
	Node *n;
	Node *nSuiv;
	
	int x1, y1;
	int x2, y2;
	
	for(i = 0; i < (way->nb_nds - 1); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;

    		HASH_FIND_INT(h_nodes, &way->nds[i+1], nSuiv);

		x2 = lon_to_pixels(nSuiv->lon, minLon, maxLon) * WIN_WIDTH;
		y2 = lat_to_pixels(nSuiv->lat, minLat, maxLat) * WIN_HEIGHT;
 
		thickLineRGBA(renderer, x1, y1, x2, y2, draw_width, *r, *g, *b, *a);
		
		// Draw two circles at each extremity to make angles more smooth
		filledCircleRGBA(renderer, x1, y1, draw_width/2, *r, *g, *b, *a);
		filledCircleRGBA(renderer, x2, y2, draw_width/2, *r, *g, *b, *a);
	}
	
	free(r);
	free(g);
	free(b);
	free(a);
	
	return 0;
}


/**
 * \fn drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color)
 * \brief Fonction qui permet de dessiner une route sur la frame
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way la way qui contient le nombre de noeud du batiment
 * \param h_nodes la table de hashage 'nodes'
 * \param m_bds les bounds de la map
 * \param color la couleur du batiment au format html
 * \return int si tout c'est bien passé
 */
int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color){
	int i;
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
	html_to_rgba(color, r, g, b, a);
	
	Node *n;
	Node *nSuiv;
	
	short *x_tab;
	short *y_tab;	
	
	x_tab = malloc(way->nb_nds * sizeof(short *));
	y_tab = malloc(way->nb_nds * sizeof(short *));
	
	for(i = 0; i < (way->nb_nds); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x_tab[i] = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y_tab[i] = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;
	}
	
	filledPolygonRGBA(renderer, x_tab, y_tab, way->nb_nds, *r, *g, *b, *a);
	
	// Draw the external border of the building
	html_to_rgba("#8f7599", r, g, b, a);
	for(i = 0; i < (way->nb_nds - 1); i++){
		aalineRGBA(renderer, x_tab[i], y_tab[i], x_tab[i+1], y_tab[i+1], *r, *g, *b, *a);
	}
	
	free(r);
	free(g);
	free(b);
	free(a);
	free(x_tab);
	free(y_tab);
	
	return 0;
}


/**
 * \fn drawFilledPolygon(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color)
 * \brief Fonction qui permet de dessiner un polygone remplit
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way la way qui contient le nombre de noeud du polygone
 * \param h_nodes la table de hashage 'nodes'
 * \param m_bds les bounds de la map
 * \param color la couleur du polygone au format html
 * \return int si tout c'est bien passé
 */
int drawFilledPolygon(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color){
	int i;
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
	html_to_rgba(color, r, g, b, a);
	
	Node *n;
	Node *nSuiv;
	
	short *x_tab;
	short *y_tab;	
	
	x_tab = malloc(way->nb_nds * sizeof(short *));
	y_tab = malloc(way->nb_nds * sizeof(short *));
	
	for(i = 0; i < (way->nb_nds); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x_tab[i] = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y_tab[i] = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;
	}
	
	filledPolygonRGBA(renderer, x_tab, y_tab, way->nb_nds, *r, *g, *b, *a);
	
	free(r);
	free(g);
	free(b);
	free(a);
	free(x_tab);
	free(y_tab);
	
	return 0;
}


/**
 * \fn drawDashedLine(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color, int dash_length)
 * \brief Fonction qui permet de dessiner une ligne pointillet ( __ __ __ __ __ __ )
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way la way qui contient le nombre de noeud da la ligne
 * \param h_nodes la table de hashage 'nodes'
 * \param m_bds les bounds de la map
 * \param color la couleur de la route au format html
 * \param dash_length la taile d'un pointillet ( __ )
 * \return int si tout c'est bien passé
 */
int drawDashedLine(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color, int dash_length){
	int i;
	
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
	html_to_rgba(color, r, g, b, a);
	
	Node *n;
	Node *nSuiv;
	
	int x1, y1;
	int x2, y2;
	
	bool visible = true;
	double offset = 0;
	
	for(i = 0; i < (way->nb_nds - 1); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;

    		HASH_FIND_INT(h_nodes, &way->nds[i+1], nSuiv);

		x2 = lon_to_pixels(nSuiv->lon, minLon, maxLon) * WIN_WIDTH;
		y2 = lat_to_pixels(nSuiv->lat, minLat, maxLat) * WIN_HEIGHT;
		
		int delta_x = x2 - x1;
		int delta_y = y2 - y1;
	
		double alpha = atan2(delta_y, delta_x);
		double d = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		double num_dash = (d / dash_length);
		
		double mx1 = x1;
		double my1 = y1;
		double mx2;
		double my2;
		int j;
		
		for (j=0; j < (num_dash - 1); j++){
			if (offset != 0){
				// Draw the first partial dash		
				mx2 = mx1 + offset * dash_length * cos(alpha);
				my2 = my1 + offset * dash_length * sin(alpha);
				offset = 0;
			} else {
				// Draw a normal complete dash
				mx2 = mx1 + dash_length * cos(alpha);
				my2 = my1 + dash_length * sin(alpha);
			}
			
			if (visible){
// 				thickLineRGBA(renderer, mx1, my1, mx2, my2, draw_width, *r, *g, *b, *a);
				aalineRGBA(renderer, (int)mx1, (int)my1, (int)mx2, (int)my2, *r, *g, *b, *a);
			}
			
			mx1 = mx2;
			my1 = my2;
			
			visible = !visible;
			
		}
		
		// Draw the last partial dash
		double decimal_part = fmod(num_dash, 1.);
		
		if (decimal_part > 0){
			mx2 = mx1 + decimal_part * dash_length * cos(alpha);
			my2 = my1 + decimal_part * dash_length * sin(alpha);
			
			if (visible){
// 					thickLineRGBA(renderer, mx1, my1, mx2, my2, draw_width, *r, *g, *b, *a);
					aalineRGBA(renderer, (int)mx1, (int)my1, (int)mx2, (int)my2, *r, *g, *b, *a);
			}
			
			offset = (1 - decimal_part);
		}
	}
	
	free(r);
	free(g);
	free(b);
	free(a);
	
	return 0;
}


/**
 * \fn drawDottedLine(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color, int dot_radius)
 * \brief Fonction qui permet de dessiner une ligne pointillet avec des points ( . . . . . . . )
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way la way qui contient le nombre de noeud de la ligne
 * \param h_nodes la table de hashage 'nodes'
 * \param m_bds les bounds de la map
 * \param color la couleur de la route au format html
 * \param dot_radius le rayon d'un point ( . )
 * \return int si tout c'est bien passé
 */
int drawDottedLine(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color, int dot_radius){
	int i;
	
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
	html_to_rgba(color, r, g, b, a);
	
	Node *n;
	Node *nSuiv;
	
	int x1, y1;
	int x2, y2;
	
	bool visible = true;
	double offset = 0;
	
	for(i = 0; i < (way->nb_nds - 1); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;

    		HASH_FIND_INT(h_nodes, &way->nds[i+1], nSuiv);

		x2 = lon_to_pixels(nSuiv->lon, minLon, maxLon) * WIN_WIDTH;
		y2 = lat_to_pixels(nSuiv->lat, minLat, maxLat) * WIN_HEIGHT;
		
		int delta_x = x2 - x1;
		int delta_y = y2 - y1;
	
		double alpha = atan2(delta_y, delta_x);
		double d = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		double num_dot = (d / (dot_radius));
		
		double mx1 = x1;
		double my1 = y1;
		
		int j;
		for (j=0; j < (num_dot/2); j++){
			if (visible){
				aacircleRGBA(renderer, mx1, my1, dot_radius, *r, *g, *b, *a);
// 				filledCircleRGBA(renderer, mx1, my1, dot_radius, *r, *g, *b, *a);
			}
			
			if (offset != 0){
				// Leave an offset from the beginning
				mx1 = mx1 + offset * 2 * dot_radius * cos(alpha);
				my1 = my1 + offset * 2 * dot_radius * sin(alpha);
				offset = 0;
			} else {
				// Draw a normal dot
				mx1 = mx1 + 2 * dot_radius * cos(alpha);
				my1 = my1 + 2 * dot_radius * sin(alpha);
			}
			
			visible = !visible;
			
		}
		
		// Calculate the offset
		double decimal_part = fmod(num_dot, 1.);
		
		if (decimal_part > 0){			
			offset = (1 - decimal_part);
		}
	}
	
	free(r);
	free(g);
	free(b);
	free(a);
	
	return 0;
}

// SDL_Point* get_centroid(Way *way, Node *h_nodes, Bounds *m_bds){
// 	SDL_Point* centroid = (struct SDL_Point*) malloc(sizeof(struct SDL_Point));;
// 	
// 	centroid->x =(int) malloc(sizeof(int));
// 	centroid->y =(int) malloc(sizeof(int));
// 	int sommeX = 0;
// 	int sommeY = 0;	
// 	double minLat = m_bds->minlat;
// 	double minLon= m_bds->minlon;
// 	double maxLat = m_bds->maxlat;
// 	double maxLon= m_bds->maxlon;
// 	Node *n = NULL;
// 	int x1,y1,i;	
// 	
// 	for(i = 0; i < (way->nb_nds); i++){
// 		
// 		HASH_FIND_INT(h_nodes, &way->nds[i], n);
// 			
// 		x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
// 		y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;
// 
// 		sommeX += x1;
// 		sommeY += y1;
// 		
// 	}	
// 	centroid->x = sommeX/(way->nb_nds);
// 	centroid->y = sommeY/(way->nb_nds);
// 	
// 	//printf("Centroid : x = %d ;  y = %d \n",centroid->x,centroid->y);
// 
// 	return centroid;
// }

// SDL_Point* get_middle_of_way(Way *way, Node *h_nodes, Bounds *m_bds){
// 	SDL_Point* centre = (struct SDL_Point*) malloc(sizeof(struct SDL_Point));;
// 	double minLat = m_bds->minlat;
// 	double minLon= m_bds->minlon;
// 	double maxLat = m_bds->maxlat;
// 	double maxLon= m_bds->maxlon;
// 
// 	centre->x =(int) malloc(sizeof(int));
// 	centre->y =(int) malloc(sizeof(int));
// 	int middle = (way->nb_nds-1)/2;
// 	
// 	Node *n = NULL;
// 	HASH_FIND_INT(h_nodes, &way->nds[middle], n);
// 	
// 	centre->x = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
// 	centre->y = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;
// 
// 	
// 	return centre;
// }

// double getTextRotation(Way *way, Node *h_nodes, Bounds *m_bds){
// 	double degree;
// 	int middle = (way->nb_nds-1)/2;
// 	int x1,x2,y1,y2;
// 	float pi = 3.14159265;
// 	double minLat = m_bds->minlat;
// 	double minLon= m_bds->minlon;
// 	double maxLat = m_bds->maxlat;
// 	double maxLon= m_bds->maxlon;
// 	double val = 180.0 / pi;
// 
// 	Node *n = NULL;
// 	Node *n_next = NULL;
// 	HASH_FIND_INT(h_nodes, &way->nds[middle], n);
// 
// //	Il faut calculer la distance entre les deux points
// 	
// 	x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
// 	y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;
// 
// 	HASH_FIND_INT(h_nodes, &way->nds[middle+1], n_next);
// 
// 	x2 = lon_to_pixels(n_next->lon, minLon, maxLon) * WIN_WIDTH;
// 	y2 = lat_to_pixels(n_next->lat, minLat, maxLat) * WIN_HEIGHT;
// 	printf("y2-y1 = %d \n",y2-y1);
// 	double alpha = acos((x2-x1)/(sqrt(pow((x2-x1),2) + pow((y2-y1),2))));
// 	if((y2-y1)<0){
// 		return (-alpha*val);
// 	}
// 
// 	return (alpha*val);
// }

void trim_string(const char *input, char *output, int max_length){
	int i;
	int j = 0;
	strcpy(output, input);
	for (i = 0; input[i] != '\0'; i++) {
		if ((j >= max_length) && (isspace((unsigned char) input[i]))) {
			output[i] = '\n';
			j = 0;
		} else {
			j++;
		}
	}
}

/**
 * \fn  writeBuildingName(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char *text, TTF_Font *police, char* color)
 * \brief Write the name of a building
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way the way containing the nodes of the building
 * \param h_nodes the hashtable containing all the nodes
 * \param m_bds the boundaries of the map, usued to calculate on screen coordinate 
 * \param text the name of the building to write
 * \param police the font used
 * \param color the color in html format (#RRGGBB)
 * \return int si tout c'est bien passé
 */
int writeBuildingName(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char *text, TTF_Font *police, char* color){
	int i;
	
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	int sommeX = 0;
	int sommeY = 0;	
	
	Node *n = NULL;
	
	int x1, y1;
	int x_center, y_center;
	
	for(i = 0; i < (way->nb_nds); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;

		sommeX += x1;
		sommeY += y1;
		
	}
	
	x_center = sommeX/(way->nb_nds);
	y_center = sommeY/(way->nb_nds);
	
	//printf("Center : x = %d ;  y = %d \n", x_center, y_center);
	
// 	char *result = malloc(strlen(text)+1);
// 	trim_string(text, result, 15);
	
	// Make sure the text to display is not too long
	char *result;
	if (strlen(text) > 30) {
		result = (char *) malloc(sizeof(char[30]));
		strncpy(result, text, 30);
		result[26] = '.';				
		result[27] = '.';				
		result[28] = '.';
		result[29] = '\0';
	} else {
		result = (char *) malloc(strlen(text)+1);
		strcpy(result, text);
	}
	
	writeText(renderer, result, police, x_center, y_center, color, 0);
// 	writeText(renderer, text, police, x_center, y_center, color, 0);

	return 0;
}

/**
 * \fn  writeRoadName(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char *text, TTF_Font *police, char* color)
 * \brief Write the name of a road
 *
 * \param renderer le renderer sur lequel dessiner
 * \param way the way containing the nodes of the road
 * \param h_nodes the hashtable containing the nodes
 * \param m_bds the boundaries of the map, usued to calculate on screen coordinate 
 * \param text the name of the road to write
 * \param police the font used
 * \param color the color in html format (#RRGGBB)
 * \return int si tout c'est bien passé
 */
int writeRoadName(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char *text, TTF_Font *police, char* color){
	
	double minLat = m_bds->minlat;
	double minLon = m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon = m_bds->maxlon;
	
	int indMiddle = (way->nb_nds-1)/2;
	
// 	printf("Max: %d, Middle: %d\n", way->nb_nds - 1, indMiddle);
// 	printf("Max: %d, Next: %d\n", way->nb_nds - 1, indMiddle + 1);
	
	Node *n = NULL;
	Node *n_next = NULL;
	
	int x1, y1;
	int x2, y2;
	
	HASH_FIND_INT(h_nodes, &way->nds[indMiddle], n);
	
	x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
	y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;

	HASH_FIND_INT(h_nodes, &way->nds[indMiddle + 1], n_next);

	x2 = lon_to_pixels(n_next->lon, minLon, maxLon) * WIN_WIDTH;
	y2 = lat_to_pixels(n_next->lat, minLat, maxLat) * WIN_HEIGHT;
	
	int delta_x = x2 - x1;
	int delta_y = y2 - y1;

	double alpha = atan2(delta_y, delta_x);
	// Make sure the result is between -PI/2 and PI/2
	if (alpha > M_PI/2){
		alpha = alpha - M_PI;
	} else if (alpha < -M_PI/2){
		alpha = alpha + M_PI;
	}
	
// 	printf("Alpha: %f\n", alpha);
// 	printf("Angle: %f°\n", radian_to_degrees(alpha));

	writeText(renderer, text, police, x1, y1, color, radian_to_degrees(alpha));	
	
	return 0;
}

/**
 * \fn writeText(SDL_Renderer *renderer, char *text, TTF_Font *police, int x, int y, char* color, double angle)
 * \brief Write text on the screen centered around the coordinate (x,y)
 *
 * \param renderer le renderer sur lequel dessiner
 * \param text le texte a afficher
 * \param police la police d'écriture
 * \param x la coordonnées x pour placer le texte
 * \param y la coordonnées y pour placer le texte
 * \param r, g, b, a la couleur du texte
 * \param angle la rotation du texte (in degrees)
 * \return int si tout c'est bien passé
 */
int writeText(SDL_Renderer *renderer, char *text, TTF_Font *police, int x, int y, char* color, double angle)
{
	// Color from html
	Uint8 *r = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *g = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *b = (Uint8 *)malloc(sizeof(Uint8));
	Uint8 *a = (Uint8 *)malloc(sizeof(Uint8));
	html_to_rgba(color, r, g, b, a);
	SDL_Color coul = {*r,*g,*b};
	
	// Create surface
	SDL_Surface* texte = TTF_RenderUTF8_Blended(police,text,coul);
// 	SDL_Surface* texte = TTF_RenderUTF8_Blended_Wrapped(police,text,coul, 200);
	if (texte == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	// Create texture
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer,texte);
	if (message == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	
	// Calculate the size of the displayed text
	int w,h;
	if(TTF_SizeUTF8(police,text,&w,&h)) {
		fprintf(stderr, "SDL_ttf Error: %s", TTF_GetError());
		exit(1);
	} else {
		//printf("width=%d height=%d\n", w, h);
	}
	
	SDL_Rect message_rect; //create a rectangle
	
	// Center the rectangle
	message_rect.x = x-(w/2); 
	message_rect.y = y-(h/2);
	message_rect.w = w;
	message_rect.h = h;
	
	SDL_RenderCopyEx(renderer, message, NULL, &message_rect, angle, NULL, SDL_FLIP_NONE);
	
	free(r);
	free(g);
	free(b);
	free(a);
	SDL_FreeSurface(texte);
	SDL_DestroyTexture(message);
	return 0;
}


