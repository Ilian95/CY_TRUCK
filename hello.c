#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_VILLE 35
#define MAX_VILLES 10

// Structure pour stocker les informations sur une ville
typedef struct avl{
    char nom[TAILLE_VILLE];
    int totalVisites;
    struct avl* fg;
    struct avl* fd;
    int equilibre;
} AVL;

// Crée un nouveau nœud de l'AVL
AVL *creerAVL(char *nom) {
    AVL* ville = (AVL*)malloc(sizeof(AVL));
    if(ville == NULL){
    	exit(1);
    }
    strcpy(ville->nom, nom);
    ville->totalVisites = 1;
    ville->fg = NULL;
    ville->fd = NULL;
    ville->equilibre = 1;
    return ville;
}

// Retourne le maximum entre deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Retourne le maximum entre 3 entiers
int max2(int a, int b, int c) {
    return max(max(a,b), c);
}

// Retourne le minimum entre deux entiers
int min(int a, int b) {
    return (a > b) ? b : a;
}

// Retourne le minimum entre 3 entiers
int min2(int a, int b, int c) {
    return min(min(a,b), c);
}

// Effectue rotation Gauche dans l'AVL
AVL *rotationG(AVL *a) {
	int eq_pivot;
	int eq_a;
	AVL *pivot;
	pivot = a->fd;
	a->fd = pivot->fg;
	pivot->fg = a;
	eq_a = a->equilibre;
	eq_pivot = pivot->equilibre;
	a->equilibre = eq_a - max(eq_pivot, 0) - 1;
	pivot->equilibre = min2(eq_a - 2, eq_a + eq_pivot - 2, eq_pivot - 1);
	a = pivot;
	return a;
}

// Effectue rotation Droite dans l'AVL
AVL *rotationD(AVL *a) {
	int eq_pivot;
	int eq_a;
	AVL *pivot;
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

// Effectue double rotation Gauche dans l'AVL
AVL *DoubleRG(AVL *a) {
  a->fd = rotationD(a->fd);
  return rotationG(a);
}

// Effectue double rotation Droite dans l'AVL
AVL *DoubleRD(AVL *a) {
  a->fg = rotationG(a->fg);
  return rotationD(a);
}

// Fonction qui équilibre l'arbre
AVL *equilibreAVL(AVL *a) {
  if (a->equilibre >= 2) {
    if (a->fd->equilibre >= 0) {
      return rotationG(a);
    } 
    else {
      return DoubleRG(a);
    }
  } 
  else if (a->equilibre <= -2) {
    if (a->fg->equilibre <= 0) {
      return rotationD(a);
    } 
    else {
      return DoubleRD(a);
    }
  }
  return a;
}

// Fonction pour insérer une ville dans l'arbre
AVL *insererAVL(AVL *a, char *nom, int *h) {
	
	// Insère le nouvel élément au bon endroit
    if (a == NULL) {
    	*h = 1;
        return creerAVL(nom);
	}
    if (strcmp(nom, a->nom) < 0){
        a->fg = insererAVL(a->fg, nom, h);
        *h=-*h;
    }
    else if (strcmp(nom, a->nom) > 0)
        a->fd = insererAVL(a->fd, nom, h);
    else {
    	*h = 0;
        (a->totalVisites)++;
        return a;
    }

    // Mettre à jour la hauteur du nœud
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

// Parcours l'arbre pour récupérer les 10 villes avec le plus de visites
void parcoursPrefixe(AVL *racine, AVL *topVilles[], int *nbVilles) {
    if (racine != NULL) {
        parcoursPrefixe(racine->fg, topVilles, nbVilles);

        if (*nbVilles < MAX_VILLES) {
            topVilles[(*nbVilles)++] = racine;
        } 
        else {
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

int comparerVisites(const void *a, const void *b) {
    const AVL *villeA = *(const AVL **)a;
    const AVL *villeB = *(const AVL **)b;
    return (villeB->totalVisites - villeA->totalVisites);
}

int main() {
    FILE *fichier;
    char ligne[1024];

    AVL *racine = NULL;

    fichier = fopen("data/data.csv", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        char *colonnes[5];
        int *h = malloc(sizeof(int));

        int colonne = 0;
        token = strtok(ligne, ";");
        while (token != NULL && colonne < 5){
            colonnes[colonne++] = token;
            token = strtok(NULL, ";");
        }

        if (colonne >= 4) {
            char *ville = colonnes[3];

            if (strlen(ville) < TAILLE_VILLE) {
                racine = insererAVL(racine, ville, h);
            }
        }

        if (colonne >= 2 && atoi(colonnes[1]) == 1 && colonne >= 3) {
            char *villeDepart = colonnes[2];

            if (strlen(villeDepart) < TAILLE_VILLE) {
                racine = insererAVL(racine, villeDepart, h);
            }
        }
    }

    fclose(fichier);

    AVL* topVilles[MAX_VILLES];
    int nbVillesTop = 0;

    parcoursPrefixe(racine, topVilles, &nbVillesTop);

    qsort(topVilles, nbVillesTop, sizeof(AVL *), comparerVisites);

    printf("Les 10 villes avec le plus de visites :\n");
    for (int i = 0; i < nbVillesTop && i < MAX_VILLES; i++) {
        printf("%s : Total Visites - %d\n", topVilles[i]->nom, topVilles[i]->totalVisites);
    }

    return EXIT_SUCCESS;
}
