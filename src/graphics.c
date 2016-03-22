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

int drawRoad(SDL_Renderer *renderer, Way *way, Node *h_nodes, Bounds *m_bds, int draw_width ,int r, int g, int b, int alpha){
	int i;
	double minLat = m_bds->minlat;
	double minLon= m_bds->minlon;
	double maxLat = m_bds->maxlat;
	double maxLon= m_bds->maxlon;
		
	// Should be changed
	int win_width = 800;
	int win_height = 600;
		
	for(i = 0; i < (way->nb_nds - 1); i++){
		Node *n;
    		HASH_FIND_INT(h_nodes, &way->nds[i], n);
			
		int x1 = lon_to_pixels(n->lon, minLon, maxLon) * win_width;
		int y1 = lat_to_pixels(n->lat, minLat, maxLat) * win_height;

		Node *nSuiv;
    		HASH_FIND_INT(h_nodes, &way->nds[i+1], nSuiv);

		int x2 = lon_to_pixels(nSuiv->lon, minLon, maxLon) * win_width;
		int y2 = lat_to_pixels(nSuiv->lat, minLat, maxLat) * win_height;
 
		drawLine(renderer, x1, y1, x2, y2, draw_width, r, g, b, alpha);
	}
	return 0;
}

int drawMur(SDL_Renderer *renderer,Node * nds,int width,int nbPoints,int r, int g, int b, int a, Node *h_nodes,Bounds *m_bds)
{
	int i;
	for(i = 0;i<nbPoints;i++)
	{
		if(i == nbPoints-1)
		{
			break;
		}
		double minLat = m_bds->minlat;
		double minLon= m_bds->minlon;
		double maxLat = m_bds->maxlat;
		double maxLon= m_bds->maxlon;
		int height = 800;
		int w = 800;
		double diffLon = maxLon - minLon;   // pour x
		double diffLat = maxLat - minLat;   //pour y

		printf("diffLon = %f, diffLat = %f \n",diffLon,diffLat);

		Node *n;
    		HASH_FIND_INT(h_nodes, &nds[i], n);
			
		int xDeLon = ((n->lon-minLon)*w) /diffLon;
		int yDeLat = ((n->lat-minLat)*height) /diffLat;		

		Node *nSuiv;
    		HASH_FIND_INT(h_nodes, &nds[i+1], nSuiv);		
		
		int xDeLonSuiv = ((nSuiv->lon-minLon)*w) /diffLon;
		int yDeLatSuiv = ((nSuiv->lat-minLat)*height) /diffLat;		
				
		printf ("x = %d \n",xDeLon);
		printf ("y = %d \n",yDeLat);

		int y = 700-(yDeLat);
		int x = (xDeLon);
 
		thickLineRGBA(renderer,x,y,xDeLonSuiv,700-yDeLatSuiv,1,0,0,0,a);
	}
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
