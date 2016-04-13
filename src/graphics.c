#include "../include/graphics.h"
#include "../include/calcul.h"

int html_to_rgba(char *str, Uint8* r, Uint8* g, Uint8* b, Uint8* a){
	Uint32 color;
	
	int res = sscanf(str, "#%"SCNx32, &color);
	if (1 == res) {
		// color now contains the value from the string
// 		printf("Color: %#08x\n", color);
		
		*r = (Uint8) ((color & 0xFF0000) >> 16);// Extract the RR byte
		*g = (Uint8) ((color >> 8) & 0xFF);	// Extract the GG byte
		*b = (Uint8) ((color) & 0xFF);		// Extract the BB byte
		*a = (Uint8) (0xFF);			// Always 255 by default
		
// 		printf("Red: %d\n", *r);
// 		printf("Green: %d\n", *g);
// 		printf("Blue: %d\n", *b);
// 		printf("Alpha: %d\n", *a); 
	}
	
	return res;
}

void catchEvents(SDL_Renderer * renderer, SDL_Texture *texture)
{
	SDL_Event event;
	int terminer = 0;
	
	SDL_Rect DestR;
	SDL_Rect r;
  	r.w = 5;
        r.h = 5;
	r.x = 500;
	r.y = 500;
	DestR.x = 500;
	DestR.y = 500;
	DestR.w = 300;
	DestR.h = 300;
	
       SDL_SetRenderTarget(renderer, NULL);
       SDL_RenderCopy(renderer, texture, &DestR, NULL);
       SDL_RenderPresent(renderer);

	while(!terminer)
    {
		SDL_WaitEvent(&event);
		
		if(event.window.event == SDL_WINDOWEVENT_CLOSE)
			terminer = 1;
		switch(event.type)
		{
			case SDL_QUIT:
				terminer = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						terminer = 1;
						break;
					case SDLK_UP: // Flèche haut
						printf("Flèche haut");
						DestR.y -= 10;
						SDL_SetRenderTarget(renderer, NULL);
						SDL_RenderCopy(renderer, texture, &DestR, NULL);
						SDL_RenderPresent(renderer);
						break;
					case SDLK_DOWN: // Flèche bas
						printf("Flèche bas");
						DestR.y += 10;
						SDL_SetRenderTarget(renderer, NULL);
						SDL_RenderCopy(renderer, texture, &DestR, NULL);
						SDL_RenderPresent(renderer);
						break;
					case SDLK_RIGHT: // Flèche droite
						printf("Flèche droite");
						DestR.x += 10;
						SDL_SetRenderTarget(renderer, NULL);
						SDL_RenderCopy(renderer, texture, &DestR, NULL);
						SDL_RenderPresent(renderer);
						break;
					case SDLK_LEFT: // Flèche gauche
						printf("Flèche gauche");
						DestR.x -= 10;
						SDL_SetRenderTarget(renderer, NULL);
						SDL_RenderCopy(renderer, texture, &DestR, NULL);
						SDL_RenderPresent(renderer);

						break;
					case SDLK_PLUS: // On zoom
						printf("On zoom");
						break;
					case SDLK_MINUS:
						printf("On dezoom");
						break;
				}

				break;
		}
		
		
		

    }
}

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

int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color) {
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

int drawFilledPolygon(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, char* color) {
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
// 		printf("Alpha: %f\n", alpha);
 
		double d = sqrt((delta_x * delta_x) + (delta_y * delta_y));
// 		printf("distance: %f\n", d);
		
		double num_dash = (d / dash_length);
// 		printf("num_dash: %f\n", num_dash);
		
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
			
// 			printf("j: %d, mx1: %f, my1: %f, mx2: %f, my2: %f, visible: %d\n", j, mx1, my1,  mx2, my2, visible);
			
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
// 		printf("Decimal: %f\n", decimal_part);
		
		if (decimal_part > 0){
			mx2 = x2;
			my2 = y2;
			
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
// 		printf("Alpha: %f\n", alpha);
 
		double d = sqrt((delta_x * delta_x) + (delta_y * delta_y));
// 		printf("distance: %f\n", d);
		
		double num_dot = (d / (dot_radius));
// 		printf("num_dot: %f\n", num_dot);
		
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
// 			
// 			printf("j: %d, mx1: %f, my1: %f, mx2: %f, my2: %f, visible: %d\n", j, mx1, my1,  mx2, my2, visible);
			
			visible = !visible;
			
		}
		
		// Calculate the offset
		double decimal_part = fmod(num_dot, 1.);
// 		printf("Decimal: %f\n", decimal_part);
		
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

/*int writeText(SDL_Renderer *renderer,char *text,int fontWidth,int x, int y,int width,int height,int r,int g,int b, double angle)
{
	TTF_Font *police = NULL;
	police = TTF_OpenFont("DejaVuSans-ExtraLight.ttf",fontWidth);	
	SDL_Color coul = {r,g,b};
	SDL_Surface* texte = TTF_RenderText_Blended(police,text,coul);

	SDL_Surface *textRot = rotozoomSurface(texte,angle,1.0,1);

	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer,textRot);

	SDL_Rect message_rect; //create a rec
	message_rect.x = x;  //controls the rect's x coordinate 
	message_rect.y = y; // controls the rect's y coordinte
	message_rect.w = width; // controls the width of the rect
	message_rect.h = height; // controls the height of the rect

	SDL_RenderCopy(renderer,message,NULL,&message_rect);

	TTF_CloseFont(police);
}*/
