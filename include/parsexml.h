/**
 * \file parsexml.h
 * \brief Contient les signatures et directives de préprocesseur pour la parsage de document au format xml
 * \author Adel.Z Julien.B Charles.R
 */

#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "data.h"

typedef void (*fct_parcours_t)(xmlNodePtr);

void print_bounds(Bounds bds);
void print_tag(Tag tag);
void print_node(Node node);
void print_member(Member member);
void print_way(Way way);
Node *find_node(int node_id);
Way *find_way(int way_id);
void add_node(Node *nd);
void add_way(Way *way);
Map parcours_prefixe(xmlNodePtr noeud, fct_parcours_t f);
void afficher_noeud(xmlNodePtr noeud);
