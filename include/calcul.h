/**
 * \file calcul.h
 * \brief Contient les signatures et directives de préprocesseur pour les calculs
 * \author Adel.Z Julien.B Charles.R
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define earth_radius 6378137

double degrees_to_radian(double degrees);
double lat_to_y(double lat);
double lon_to_x(double lon);
double lat_to_pixels(double lat, double map_minlat, double map_maxlat);
double lon_to_pixels(double lon, double map_minlon, double map_maxlon);
