#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

int main(int argc, char *argv[]) {
	typedef struct {
	char nom[200];
	int occurrences;
    }Ville;

    
    //Verifie les arguments
    if (argc != 2) {
        fprintf(stderr, "PROGRAMME C : Usage: %s <nom_du_fichier>\n", argv[0]);
        return 1;
    }

    // Ouvrir le fichier en lecture
    FILE *fichier = fopen(argv[1], "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    // Compte le nb de ligne du fichier et affiche colonne 2 et 3 (VILLE)
    int Nb_villes = 0;
    char ligne[200];
    
    Ville* ville = NULL;
    

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
    
    	char *pos = strchr(ligne, '\n');
	if (pos != NULL) {
	    *pos = '\0';
	}
    
        char *token = strtok(ligne, ";");

        // Parcourir les colonnes jusqu'à la 2e et 3e (VILLE)
        for (int i = 1; i < 6; i++) {
            token = strtok(NULL, ";");
            if (token != NULL) {
            	// Afficher la 2e colonne
                if (i == 2) {
                    int villeTrouvee = 0;
                    
                    for(int j =0; j<Nb_villes; j++){
                    	if(strcmp(ville[j].nom, token) == 0){
                    		ville[j].occurrences++;
                    		villeTrouvee = 1;
                    		break;
                    	}
                    }
                    
                    if (!villeTrouvee){
                    	ville = realloc(ville, (Nb_villes+1) * sizeof(Ville));
                    	if(ville == NULL){
                    	 	printf("ERREUR SEGMENTATION FAULT !!!");
                    	 	exit(15);
                    	}
                    	
                    	strcpy(ville[Nb_villes].nom, token);
                    	ville[Nb_villes].occurrences = 1;
                    	Nb_villes++;
                    }

                }
                // Afficher la 3e colonne
                if (i == 3) {
                    int villeTrouvee = 0;
                    
                    for(int j =0; j<Nb_villes; j++){
                    	if(strcmp(ville[j].nom, token) == 0){
                    		ville[j].occurrences++;
                    		villeTrouvee = 1;
                    		break;
                    	}
                    }
                    
                    if (!villeTrouvee){
                    	ville = realloc(ville, (Nb_villes+1) * sizeof(Ville));
                    	if(ville == NULL){
                    	 	printf("ERREUR SEGMENTATION FAULT !!!");
                    	 	exit(15);
                    	}
                    	
                    	strcpy(ville[Nb_villes].nom, token);
                    	ville[Nb_villes].occurrences = 1;
                    	Nb_villes++;
                    }
                }
            } 
            else {
                break; // Si la ligne n'a pas assez de colonnes
            }
        }
    }
    
    for(int i = 0; i<Nb_villes;i++){
    	printf("%s;%d\n", ville[i].nom, ville[i].occurrences);
    }

    // Fermer le fichier
    fclose(fichier);

    free(ville);	
	
    return 0;
}
