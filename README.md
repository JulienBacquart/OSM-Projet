# OpenStreetMap - Conduite de projet

 Ce projet réalisé dans le cadre du cours de conduite de projet à l'université Paris Diderot a pour but de mettre à disposition un logiciel de rendu graphique écrit en C (grâce à la bibliothèque SDL2 (et SDL2_gfx, SDL2_ttf en complément) et xml2) de cartes OpenStreetMap.
 
## A propos d'OpenStreetMap

OpenStreetMap (OSM) est un projet international fondé en 2004 dans le but de créer une carte libre du monde. Des données sont collectées dans le monde entier sur les routes, voies ferrées, les rivières, les forêts, les bâtiments et bien plus.
Les données cartographiques collectées sont ré-utilisables sous licence libre ODbL . Les cartes sont au format xml (.osm) et la spécification est disponible [ici](https://wiki.openstreetmap.org/wiki/Main_Page)

## Librairies requises

Pour faire fonctionner ce logiciel de rendu, cela nécessite au préalable l'installation de plusieurs librairies : 
* xml2 (`sudo apt-get install libxml2-dev`)
* SDL2 (`sudo apt-get install libsdl2-2.0-0`)
* SDL2_gfx (`sudo apt-get install libsdl2-gfx-dev`)
* SDL2_ttf (`sudo apt-get install libsdl2-ttf-dev`)
* check (`sudo apt-get install check`)

## Exécution

#### Exécution rapide
Pour une exécution rapide et un affichage direct, vous pouvez exécuter la commande `make run` à la racine du répertoire, cela aura pour effet de faire un affichage graphique de la map '02_paris_place_des_vosges.osm' (située dans le repertoire maps) si vous souhaitez conserver cette exécution rapide en changeant de map, il vous fait éditer la ligne 27 du Makefile. 

        run: renderer
        	./renderer maps_test/`nom_de_la_map`.osm 
        	
#### Exécution via l'exécutable
Vous pouvez également choisir de lancer l'exécutable. Pour cela il faut d'abord le créer en lançant la commande `make` à la racine du répèrtoire. L'exécutable est alors créé, vous pouvez l'appeler en faisant `./renderer/maps/'nom_de_map'.osm` 
# Test

Si vous avez compilé grâce au `make`, il y a un autre exécutable qui s'est créé, c'est `check_calcul`, vous avez vu plus haut qu'il faut installer la librairie check pour faire fonctionner ce logiciel, check permet de faire des tests unitaires en C, et si vous faites `./check_calcul`, vous verrez si tous les tests effectués sur les fonctions de calcul sont réussis ou non.

# A propos

La documentation complète de tout le code source est disponible dans le répertoire doc à la racine du projet. Cette documentation à été créée grâce à l'outil Doxygen et la page d'accueil est disponible sur `./doc/html/index.html`
Projet réalisé en trinôme par ZEGHLACHE Adel, BACQUART Julien, ROUILLARD Charles



