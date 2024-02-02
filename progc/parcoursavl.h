#ifndef PARCOURSAVL_H
#define PARCOURSAVL_H

#include "autre.h"
#include "arbre.h"
struct avl_e;
struct avl;

void parcoursPrefixe(Avl *racine, Avl** topVilles, int *nbVilles);
 
int comparerVilles(const void *a, const void *b);


#endif 