# OpenStreetMap - Conduite de projet
---
 Ce projet réalisé dans le cadre du cour de conduite de projet à l'université Paris Diderot a pour but de mettre a disposition un logiciel de rendu graphique écrit en C (grace a la bibliothèque SDL2 (et SDL2_gfx, SDL2_ttf en complément) et xml2) de cartes OpenStreetMap.
 
## A propos d'OpenStreetMap
---
OpenStreetMap (OSM) est un projet international fondé en 2004 dans le but de créer une carte libre du monde. Des données sont collectés dans le monde entier sur les routes, voies ferrées, les rivières, les forêts, les bâtiments et bien plus.
Le données cartographiques collectées sont ré-utilisables sous licence libre ODbL . Les cartes sont au format xml (.osm) et la spécification est disponible [ici](https://wiki.openstreetmap.org/wiki/Main_Page)

## Librairies requise
---
Pour faire fonctionner ce logi iel de rendu, cela nécéssite au préalable l'installation de plusieurs librairies : 
* xml2 (`sudo apt-get install libxml2-dev`)
* SDL2 (`sudo apt-get install libsdl2-2.0-0`)
* SDL2_gfx (`sudo apt-get install libsdl2-gfx-dev`)
* SDL2_ttf (`sudo apt-get install libsdl2-ttf-dev`)
* check (`sudo apt-get install check`)

## Exécution
---
#### Exécution rapide
Pour une éxécution rapide et un affichage direct, vous pouvez éxécuter la commande `make run` à la racine du repertoire, cela aura pour effet de faire un affichage graphique de la map '02_paris_place_des_vosges.osm' (situés dans le repertoire maps) si vous souhaitez conserver cette éxéution rapide en changeant de map, il vous fait éditer la ligne 27 du Makefile. 

        run: renderer
        	./renderer maps_test/`nom_de_la_map`.osm 
        	
#### Exéctution via l'éxécutable
Vous pouvez également choisir de lancer l'éxécutable. Pour cela il faut d'abord le créer en lancant la commande `make` a la racine du répèrtoire. L'éxécutable est alors créer, vous pouvez l'appeler en faisant `./renderer/maps/'nom_de_map'.osm` 
# Test
---
Si vous avez compilé grace au `make`, il y a un autre éxécutable qui s'est crée, c'est `check_calcul`, vous avez vu plus haut qu'il faut installer la librairie check pour faire fonctionner ce logiciel, check permet de faire des test unitaires en C, et si vous faites `./check_calcul`, vous verrez si tous les tests effectués sur les fonctions de calclul sont réussis ou non.

# A propos
---
La documentation complète de tout le code source est disponible dans le répèrtoire doc a la racine du projet. Cette documentation à été créer grace a l'outil Doxygen et la page d'acceuil est diponible sur `./doc/html/index.html`
Projet réalisé en trinome par ZEGHLACHE Adel, BACQUART Julien, ROUILLARD Charles



