#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parcoursavl.h"

#define MAX_VILLES 10

// Parcours l'arbre pour récupérer les 10 villes avec le plus de visites
void parcoursPrefixe(Avl *racine, Avl** topVilles, int *nbVilles) {
    if (racine != NULL) {
        parcoursPrefixe(racine->fg, topVilles, nbVilles);

        if (*nbVilles < MAX_VILLES) {
            topVilles[(*nbVilles)++] = racine;
        } else {
            int minVisites = topVilles[0]->totalVisites;
            int minIndex = 0;

            for (int i = 1; i < MAX_VILLES; i++) {
                if (topVilles[i]->totalVisites < minVisites) {
                    minVisites = topVilles[i]->totalVisites;
                    minIndex = i;
                }
            }

            if (racine->totalVisites > minVisites) {
                topVilles[minIndex] = racine;
            }
        }

        parcoursPrefixe(racine->fd, topVilles, nbVilles);
    }
}
int comparerVilles(const void *a, const void *b) {
    const Avl *villeA = *(const Avl **)a;
    const Avl *villeB = *(const Avl **)b;
    return strcmp(villeA->nom, villeB->nom);
}