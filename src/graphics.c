#include "../include/graphics.h"

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

int drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int width, int r, int g, int b, int a)
{
	thickLineRGBA(renderer,x1,y1,x2,y2,width,r,g,b,a);
	return 0;
}

int drawPolygon(SDL_Renderer *renderer, short *x_tab, short *y_tab, int nb_pts, int r, int g, int b, int a)
{
	filledPolygonRGBA(renderer, x_tab, y_tab, nb_pts, r, g, b, a);
	return 0;
}

int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width ,int r, int g, int b, int alpha){
	int i;
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Node *n;
	Node *nSuiv;
	
	for(i = 0; i < (way->nb_nds - 1); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		int x1 = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		int y1 = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;

		
    		HASH_FIND_INT(h_nodes, &way->nds[i+1], nSuiv);

		int x2 = lon_to_pixels(nSuiv->lon, minLon, maxLon) * WIN_WIDTH;
		int y2 = lat_to_pixels(nSuiv->lat, minLat, maxLat) * WIN_HEIGHT;
 
		drawLine(renderer, x1, y1, x2, y2, draw_width, r, g, b, alpha);
		SDL_RenderPresent(renderer);
	}
	return 0;
}

int drawBuilding(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int r, int g, int b, int alpha) {
	int i;
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
	
	Node *n;
	short *x_tab;
	short *y_tab;
	
	x_tab = malloc(way->nb_nds * sizeof(short *));
	y_tab = malloc(way->nb_nds * sizeof(short *));
	
	for(i = 0; i < (way->nb_nds); i++){
		
		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		x_tab[i] = lon_to_pixels(n->lon, minLon, maxLon) * WIN_WIDTH;
		y_tab[i] = lat_to_pixels(n->lat, minLat, maxLat) * WIN_HEIGHT;
		
	}
	
	drawPolygon(renderer, x_tab, y_tab, way->nb_nds, r, g, b, alpha);
	
	SDL_RenderPresent(renderer);
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
