#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

int main(int argc, char *argv[]) {

    // ===============================================
    // 			Verifications
    // ===============================================
    
    //Verifie les arguments
    if (argc != 2) {
        fprintf(stderr, "PROGRAMME C : Usage: %s <nom_du_fichier>\n", argv[0]);
        return 1;
    }
    
    // ===============================================
    //			Création AVL
    // ===============================================
    
    //Crée la struct qui permet de faire l'AVL
    typedef struct _Arbre {
  	struct _Arbre *pDroit;
  	struct _Arbre *pGauche;
  	char* Ville;
  	int Nb_Trajets;
  	int Nb_Departs;
  	int eq;
    } Arbre;
    
    //Fonction qui crée l'arbre binaire
    Arbre *creerArbre(char* V,int T, int D) {
  	Arbre *pNouv = malloc(sizeof(Arbre));
  	if (pNouv == NULL) {
    		exit(1);
  	}
  	pNouv->pDroit = NULL;
  	pNouv->pGauche = NULL;
  	pNouv->Ville = malloc(sizeof(char) * strlen(V));
  	if(pNouv->Ville == NULL) {
  		printf("Allocation échouée");
  		exit(2);
  	}
  	strcpy(pNouv->Ville, V); 
  	pNouv->Nb_Trajets = T;
  	pNouv->Nb_Departs = D;
  	pNouv->eq = 0;
     	return pNouv;
    }
    
    // Verifie si l'arbre est vide
    int estVide(Arbre *p) { 
    	return p == NULL; 
    }
    
    // Verifie si le fils gauche est vide
    int A_FilsGauche(Arbre *p) { 
    	return !estVide(p) && (p->pGauche != NULL); 
    }

    // Verifie si le fils droit est vide
    int A_FilsDroit(Arbre *p) { 
    	return !estVide(p) && (p->pDroit != NULL); 
    }
    
    //Ajoute un fils Gauche
    void ajtFilsGauche(Arbre *p, char* V, int T, int D) {
  	assert(!estVide(p));
  	if (A_FilsGauche(p)) {
    		exit(3);
  	}
  	p->pGauche = creerArbre(V, T, D);
	if (p->pGauche == NULL) {
	  exit(4);
	}
    }

    // Ajoute un fils droit
    void ajtFilsDroit(Arbre *p, char* V, int T, int D) {
  	assert(!estVide(p));
	if (A_FilsDroit(p)) {
	  exit(5);
	}
	p->pDroit = creerArbre(V, T, D);
	if (p->pDroit == NULL) {
	  exit(6);
	}
    }
     
    // Parcours de l'arbre
    void infixe(Arbre *p) {
  	if (!estVide(p)) {
	    infixe(p->pGauche);
	    printf("%s;%d;%d\n", p->Ville, p->Nb_Trajets, p->Nb_Departs);
	    infixe(p->pDroit);
	}
    }
    
    // Permet de changer la valeur des occurences d'un noeud
    Arbre *changeNoeud(Arbre *a, char* V, int T, int D) {
  	if (a == NULL) {
    		exit(7);
  	} 
  	a->Nb_Trajets = T;
  	a->Nb_Departs = D;
  	return a;
    }
    
    // Libère l'espace allouer par l'arbre
    void LibereArbre(Arbre *a) {
  	if (a == NULL) {
    		exit(8);
  	}
  	if (A_FilsGauche(a)) {
    		LibereArbre(a->pGauche);
  	}
  	if (A_FilsDroit(a)) {
    		LibereArbre(a->pDroit);
  	}
  	a->pGauche = NULL;
  	a->pDroit = NULL;
  	free(a);
    }
    
    // Permet de construire un ABR
    Arbre *insereABR(Arbre *a, char* V, int T, int D) {
	if (a == NULL) {
	  creerArbre(V, T, D);
	}
	else if (T <= a->Nb_Trajets) {
	  if (A_FilsGauche(a)) {
	    insereABR(a->pGauche, V, T, D);
	  } 
	  else {
	    ajtFilsGauche(a, V, T, D);
	  }
	} 
	else {
	  if (A_FilsDroit(a)) {
	    insereABR(a->pDroit, V, T, D);
	  } else {
	    ajtFilsDroit(a, V, T, D);
	  }
	}
	return a;
    }

    
    // ===============================================
    // 			Programme
    // ===============================================

    // Ouvrir le fichier en lecture
    FILE *fichier = fopen(argv[1], "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    // Compte le nb de ligne du fichier et affiche colonne 2 et 3 (VILLE)
    int Nb_lignes = 0;
    char ligne[200];  

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *token = strtok(ligne, ";");

        // Parcourir les colonnes jusqu'à la 2e et 3e (VILLE)
        for (int i = 1; i < 6; i++) {
            token = strtok(NULL, ";");
            if (token != NULL) {
            	// Afficher la 2e colonne
                if (i == 2) {
                    printf("%s;", token);
                }
                // Afficher la 3e colonne
                if (i == 3) {
                    printf("%s \n", token);
                }
            } 
            else {
                break; // Si la ligne n'a pas assez de colonnes
            }
        }
        Nb_lignes++;
    }
    printf("Le fichier %s contient %d lignes.\n",argv[1], Nb_lignes);

    // Fermer le fichier
    fclose(fichier);

    return 0;
}
