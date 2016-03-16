#include "graphics.h"

int long2tilex(double lon, int z) 
{ 
	return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z))); 
}

int lat2tiley(double lat, int z)
{ 
	return (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z))); 
}

double tilex2long(int x, int z) 
{
	return x / pow(2.0, z) * 360.0 - 180;
}

double tiley2lat(int y, int z) 
{
	double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

void pause()
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

int drawRoad(SDL_Renderer *renderer,SDL_Point *points,int width,int nbPoints,int r, int g, int b, int a)
{
	int i;
	for(i = 0;i<nbPoints;i++)
	{
		if(i == nbPoints-1)
		{
			break;
		}
		thickLineRGBA(renderer,points[i].x,points[i].y,points[i+1].x,points[i+1].y,width,r,g,b,a);
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
