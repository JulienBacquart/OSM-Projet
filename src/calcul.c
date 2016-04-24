/**
 * \file calcul.c
 * \brief Contient des fonctions qui permettent de manipuler les calculs des latitudes, longitudes en pixels et calcul d'angle
 * \author Adel.Z Julien.B Charles.R
 */

#include "../include/calcul.h"

/**
 * \fn double degrees_to_radian(double degrees)
 * \brief Fonction qui convertit un angle en degrès vers un angle en radian
 *
 * \param degrees angle de type double a convertir.
 * \return Retourne un double correspondant a l'angle en radian
 */
double degrees_to_radian(double degrees){
	return degrees * (M_PI/180);
}

/**
 * \fn double radian_to_degrees(double radian)
 * \brief Convert an angle in radian to degrees
 *
 * \param radian the angle in radian
 * \return Return a double corresponding to the value in degrees
 */
double radian_to_degrees(double radian){
	return radian * (180/M_PI);
}

/**
 * \fn double lat_to_y(double lat)
 * \brief Fonction qui convertit une latitude GPS en une projection de mercator en mètre
 *
 * \param lat latitude de type double a convertir.
 * \return Retourne un double correspondant a une projection de mercator de la latitude
 */
double lat_to_y(double lat) { 
	return earth_radius * log(tan(M_PI/4 + degrees_to_radian(lat)/2));
}

/**
 * \fn double lon_to_x(double lon)
 * \brief Fonction qui convertit une longitude GPS en une projection de mercator en mètre
 *
 * \param lon longitude de type double a convertir.
 * \return Retourne un double correspondant a une projection de mercator de la longitude
 */
double lon_to_x(double lon) { 
	return degrees_to_radian(lon) * earth_radius;
}

/**
 * \fn lat_to_pixels(double lat, double map_minlat, double map_maxlat)
 * \brief Fonction qui convertit une latitude GPS en une valeur en pixel (entre 0 et 1)
 *
 * \param lat latitude de type double a convertir.
 * \param map_minlat la latitude minimum de la map
 * \param map_maxlat la latitude maximal de la map
 * \return Retourne un double correspondant a la latitude GPS convertit en pixel
 */
double lat_to_pixels(double lat, double map_minlat, double map_maxlat){
	return 1 - ((lat_to_y(lat) - lat_to_y(map_minlat)) / (lat_to_y(map_maxlat) - lat_to_y(map_minlat)));
}

/**
 * \fn lon_to_pixels(double lon, double map_minlon, double map_maxlon)
 * \brief Fonction qui convertit une longitude GPS en une valeur en pixel (entre 0 et 1)
 *
 * \param lon longitude de type double a convertir.
 * \param map_minlon la longitude minimum de la map
 * \param map_maxlon la longitude maximal de la map
 * \return Retourne un double correspondant a la longitude GPS convertit en pixel
 */
double lon_to_pixels(double lon, double map_minlon, double map_maxlon){
	return (lon_to_x(lon) - lon_to_x(map_minlon)) / (lon_to_x(map_maxlon) - lon_to_x(map_minlon));
}
