#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "data.h"

#define earth_radius 6378137

double degrees_to_radian(double degrees){
	return degrees * (M_PI/180);
}

// Convert a GPS latitude to a mercator projection (in meter)
double lat_to_y(double lat) { 
	return earth_radius * log(tan(M_PI/4 + degrees_to_radian(lat)/2));
}

// Convert a GPS longitude to a mercator projection (in meter)
double lon_to_x(double lon) { 
	return degrees_to_radian(lon) * earth_radius; 
}

// Convert a GPS lat to a "pixel" value (between 0 and 1)
// we then just have to multiply this value by WIN_HEIGHT to know where we need to draw the point
double lat_to_pixels(double lat, double map_minlat, double map_maxlat){
	return (lat_to_y(lat) - lat_to_y(map_minlat)) / (lat_to_y(map_maxlat) - lat_to_y(map_minlat));
}

// Convert a GPS lon to a "pixel" value (between 0 and 1)
// we then just have to multiply this value by WIN_HEIGHT to know where we need to draw the point
double lon_to_pixels(double lon, double map_minlon, double map_maxlon){
	return (lon_to_x(lon) - lon_to_x(map_minlon)) / (lon_to_x(map_maxlon) - lon_to_x(map_minlon));
}

int main(){
	
	double map_minlat = 48.8542200; 
	double map_maxlat = 48.8569130;
	
	double map_minlon = 2.3625300;
	double map_maxlon = 2.3688710;
	
	double lat = 48.8546405;
	double lon = 2.3685569;
	
	double mx = lon_to_pixels(lon, map_minlon, map_maxlon);
	printf("mx: %.7f\n", mx);
	double my = lat_to_pixels(lat, map_minlat, map_maxlat);
	printf("my: %.7f\n", my);
	
	mx = lon_to_pixels(map_minlon, map_minlon, map_maxlon);
	printf("mx: %.7f\n", mx);
	my = lat_to_pixels(map_minlat, map_minlat, map_maxlat);
	printf("my: %.7f\n", my);
	
	mx = lon_to_pixels(map_maxlon, map_minlon, map_maxlon);
	printf("mx: %.7f\n", mx);
	my = lat_to_pixels(map_maxlat, map_minlat, map_maxlat);
	printf("my: %.7f\n", my);
	
	return EXIT_SUCCESS;
}