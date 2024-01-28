#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_VILLE 35
#define MAX_VILLES 10

// Structure pour stocker les informations sur une ville
typedef struct avl {
  int totalVisites;
  int Visites_D;
  char nom[50];
  struct avl *fd;
  struct avl *fg;
  int equilibre;
} Avl;

// Crée un nouveau nœud pour une ville avec son nom et initialise les données
Avl *creerAvl(char* n) {
  Avl *pnew = malloc(sizeof(Avl));
  if (pnew == NULL) {
    exit(1);
  }
  strcpy(pnew->nom, n);
  pnew->totalVisites = 1;
  pnew->Visites_D = 0;
  pnew->fd = NULL;
  pnew->fg = NULL;
  pnew->equilibre = 0;
  return pnew;
}

// Fonction utilitaire pour retourner le maximum entre deux entiers
int max(int a, int b) {
  if (a >= b) {
    return a;
  }
  return b;
}

// Fonction utilitaire pour retourner le minimum entre deux entiers
int min(int a, int b) {
  if (a >= b) {
    return b;
  }
  return a;
}

// Fonction utilitaire pour retourner le minimum entre trois entiers
int min2(int a, int b, int c) {
  return min(min(a, b), c);
}

// Fonction utilitaire pour retourner le maximum entre trois entiers
int max2(int a, int b, int c) {
  return max(max(a, b), c);
}

// Fonctions pour effectuer les rotations dans l'arbre AVL
Avl *rotationGauche(Avl *a) {
  int eq_pivot;
  int eq_a;
  Avl *pivot;
  //Effectue la rotation
  pivot = a->fd;
  a->fd = pivot->fg;
  pivot->fg = a;
  //Met a jour l'équlibre
  eq_a = a->equilibre;
  eq_pivot = pivot->equilibre;
  a->equilibre = eq_a - max(eq_pivot, 0) - 1;
  pivot->equilibre = min2(eq_a - 2, eq_a + eq_pivot - 2, eq_pivot - 1);
  a = pivot;
  return a;
}

Avl *rotationDroite(Avl *a) {
  int eq_pivot;
  int eq_a;
  Avl *pivot;
  pivot = a->fg;
  a->fg = pivot->fd;
  pivot->fd = a;
  eq_a = a->equilibre;
  eq_pivot = pivot->equilibre;
  a->equilibre = eq_a - min(eq_pivot, 0) + 1;
  pivot->equilibre = max2(eq_a + 2, eq_a + eq_pivot + 2, eq_pivot + 1);
  a = pivot;
  return a;
}

Avl *DoubleRG(Avl *a) {
  a->fd = rotationDroite(a->fd);
  return rotationGauche(a);
}

Avl *DoubleRD(Avl *a) {
  a->fg = rotationGauche(a->fg);
  return rotationDroite(a);
}

// Fonction pour maj l'équilibre d'un nœud dans l'arbre
Avl *equilibreAVL(Avl *a) {
  if (a->equilibre >= 2) {
    if (a->fd->equilibre >= 0) {
      return rotationGauche(a);
    } else {
      return DoubleRG(a);
    }
  } else if (a->equilibre <= -2) {
    if (a->fg->equilibre <= 0) {
      return rotationDroite(a);
    } else {
      return DoubleRD(a);
    }
  }
  return a;
}

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

// Insère dans un AVL
Avl *insererVille(Avl *a, char* nom, int *h) {
  if (a == NULL) {
    *h = 1;
    return creerAvl(nom);
  } 
  else if (strcmp(nom, a->nom) < 0) {
    a->fg = insererVille(a->fg, nom, h);
    *h = -*h;
  } 
  else if (strcmp(nom, a->nom) > 0) {
    a->fd = insererVille(a->fd, nom, h);
  } 
  else {
    *h = 0;
    a->totalVisites ++;
    return a;
  }
  if (*h != 0) {
    a->equilibre = a->equilibre + *h;
    a = equilibreAVL(a);
    if (a->equilibre == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return a;
}

// Insère dans un AVL
Avl *insererVilleD(Avl *a, char* nom, int *h) {
  if (a == NULL) {
    *h = 1;
    return creerAvl(nom);
  } 
  else if (strcmp(nom, a->nom) < 0) {
    a->fg = insererVille(a->fg, nom, h);
    *h = -*h;
  } 
  else if (strcmp(nom, a->nom) > 0) {
    a->fd = insererVille(a->fd, nom, h);
  } 
  else {
    *h = 0;
    a->totalVisites ++;
    a->Visites_D ++;
    return a;
  }
  if (*h != 0) {
    a->equilibre = a->equilibre + *h;
    a = equilibreAVL(a);
    if (a->equilibre == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return a;
}

int comparerVilles(const void *a, const void *b) {
    const Avl *villeA = *(const Avl **)a;
    const Avl *villeB = *(const Avl **)b;
    return strcmp(villeA->nom, villeB->nom);
}

int main() {
	FILE *fichier;
	Avl *a = NULL;
	int *h = malloc(sizeof(int));
	char ligne[1024];

    Avl *racine = NULL;

    fichier = fopen("data/data.csv", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        char *colonnes[5];

        int colonne = 0;
        token = strtok(ligne, ";");
        while (token != NULL && colonne < 5) {
            colonnes[colonne++] = token;
            token = strtok(NULL, ";");
        }

        if (colonne >= 4) {
            char *ville = colonnes[3];

            if (strlen(ville) < TAILLE_VILLE) {
                racine = insererVille(racine, ville, h);
            }
        }

        if (colonne >= 2 && atoi(colonnes[1]) == 1 && colonne >= 3) {
            char *villeDepart = colonnes[2];

            if (strlen(villeDepart) < TAILLE_VILLE) {
                racine = insererVilleD(racine, villeDepart, h);
            }
        }
    }

    fclose(fichier);

    Avl *topVilles[MAX_VILLES];
    int nbVillesTop = 0;

    parcoursPrefixe(racine, topVilles, &nbVillesTop);

    qsort(topVilles, nbVillesTop, sizeof(Avl *), comparerVilles);

    printf("Les 10 villes avec le plus de visites :\n");
    for (int i = 0; i < nbVillesTop && i < MAX_VILLES; i++) {
        printf("%s;%d;%d\n", topVilles[i]->nom, topVilles[i]->totalVisites, topVilles[i]->Visites_D);
    }
    
    free(h);

    return EXIT_SUCCESS;
}
