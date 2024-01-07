#!/bin/bash

#------------------------------------------------------------------------------------------------------------------------------------------------
# 						VERIFICATIONS
#------------------------------------------------------------------------------------------------------------------------------------------------

# Vérifier si l'argument -h est présent parmi les paramètres
if [[ "$*" == *-h* ]]; then
    echo -e "Aide : Le chemin du fichier de données doit être le premier argument et est obligatoire.\nUtilisation : $0 [chemin_du_fichier] [traitement1] [traitement2] [traitement3]...\nTraitements :\n	---[D1] : option -d1, affiche les conducteurs avec le plus de trajets;\n	---[D2] : option -d2, affiche les conducteurs avec les plus grandes distances;\n	---[L] : option -l, affiche les 10 trajets les plus longs;\n	---[T] : option -t, affiche les 10 villes les plus traversées;\n	---[S] : option -s, affiche les statistiques sur les étapes (distance min, max et moyenne);"
    exit 0
fi

#Verifie le nb d'arguments
if [ "$#" -lt 2 ]; then
	echo "Erreur: Utilisation : $0 [chemin_du_fichier] [traitement1] [traitement2] [traitement3]..."
	exit 1
fi

#Verifie que l'argument 1 est le chemin vers data.csv
if [ ! -e "$1" ]; then
	echo "$1 n'existe pas"
	exit 2
elif [ ! -f "$1" ]; then
	echo "$1 n'est pas un chemin vers un fichier"
	exit 3
else
	extension="${1##*.}"
	
	if [ ! "${extension,,}" = "csv" ]; then
		echo "$1 n'est pas un chemin vers un fichier csv"
		exit 4
	fi 
	
	chemin="data/data.csv"
	
	if [ ! "$1" = "$chemin" ]; then
		echo "$1 n'est pas le chemin vers data.csv"
		exit 5
	fi
fi 

#Verifie la présence de l'executable et compile si il est absent puis verifie la compilation
executable="progc/exec"	

if [ ! -e "$executable" ]; then
	code="progc/hello.c"
	gcc -o "$executable" "$code"
	if [ ! $? -eq 0 ]; then
		echo "Erreur: Compilation échouée"
		exit 4
	else 
		echo "Compilation réussie"
	fi
fi

#Verifie la présence de temp le crée si il n'existe pas et supprime son contenu si il existe
dossier_temp="temp"
contenu=$(ls -A "$dossier_temp" | wc -l)
if [ ! -e "$dossier_temp" ]; then
	echo "Le dossier $dossier_temp n'existe pas"
	mkdir "$dossier_temp"
else
	if  [ ! "$contenu" -eq 0 ]; then
		rm -r "$dossier_temp"/*
	fi
fi

#Verifie la présence de images et le crée si il n'existe pas
dossier_images="images"
if [ ! -e "$dossier_images" ]; then
	echo "Le dossier $dossier_images n'existe pas"
	mkdir "$dossier_images"
fi

#Verifie que les traitements sont corrects
for arg in "${@:2}";do
	if [[ ! "$arg" == *-d1* ]] && [[ ! "$arg" == *-d2* ]] && [[ ! "$arg" == *-l* ]] && [[ ! "$arg" == *-t* ]] && [[ ! "$arg" == *-s* ]] ; then
		echo -e "Erreur sur les traitements:\nUtilisation : $0 [chemin_du_fichier] [traitement1] [traitement2] [traitement3]..."
		exit 5
	fi
done

#------------------------------------------------------------------------------------------------------------------------------------------------
# 						TRAITEMENT D1
#------------------------------------------------------------------------------------------------------------------------------------------------

if [[ "$*" == *-d1* ]]; then

	# Enregistrez le temps de début
	start_time=$(date +%s)

	# Vérifier si le fichier existe et est un fichier régulier
	file="$1"
	if [ ! -f "$file" ]; then
	    echo "Erreur : Le fichier $file n'existe pas."
	    exit 6
	fi

	# Supprimer les doublons dans les colonnes 1 et 6
	awk -F ';' '!seen[$1,$6]++' "$file" > "$dossier_temp/unique_data.csv"

	# Extraire les noms des conducteurs et le nombre de trajets avec tri et comptage
	awk -F ';' '{if ($6 != "") count[$6] ++} END {for (name in count) print (name)";"count[name]}' "$dossier_temp/unique_data.csv" | sort -nt ';' -k2 | tail -10 > "$dossier_temp/resultat_d1.txt"

	# Afficher les résultats
	echo "TRAITEMENT [D1] ---------------:"
	awk '{print $1, $2}' "$dossier_temp/resultat_d1.txt" | tac 
	
	#Affiche le graphique 
	gnuplot "graph_d1.gp"
	
	image_path="images/horizontal_d1.png"

	# Vérifier si xdg-open est disponible
	if command -v xdg-open &> /dev/null; then
	    xdg-open "$image_path"
	else
	    echo "xdg-open n'est pas disponible sur votre système."
	fi

	# Enregistrez le temps de fin
	end_time=$(date +%s)

	# Calculez la différence de temps
	execution_time=$(echo "$end_time - $start_time" | bc)

	echo -e "Le traitement [D1] a pris $execution_time secondes pour s'exécuter."
		
fi

#------------------------------------------------------------------------------------------------------------------------------------------------
# 						TRAITEMENT D2
#------------------------------------------------------------------------------------------------------------------------------------------------

if [[ "$*" == *-d2* ]]; then

	# Enregistrez le temps de début
	start_time=$(date +%s)
	
	# Vérifier si le fichier existe et est un fichier régulier
	file="$1"
	if [ ! -f "$file" ]; then
	    echo "Erreur : Le fichier $file n'existe pas."
	    exit 6
	fi
	
	
	LC_NUMERIC="C" awk -F ';' '{
	    conducteur = $6;   # la sixième colonne contient le conducteur
	    distance = $5;    # la cinquième colonne contient la distance
	    
	    # Vérifier si le conducteur a déjà été rencontré
	    if (count[conducteur] > 0) {
		total_distances[conducteur] += distance;
	    } else {
		count[conducteur] = 1;
		total_distances[conducteur] = distance;
	    }
	}
	END {
	    for (conducteur in total_distances) {
		printf "%s;%0.6f\n", conducteur , total_distances[conducteur];
	    }
	}' "$file" | sort -nt ';' -k2 | tail -10 > "$dossier_temp/resultat_d2.txt"
	
	# Afficher les résultats
	echo "TRAITEMENT [D2] : ---------------"
	awk '{print $1, $2}' "$dossier_temp/resultat_d2.txt" | tac 
	
	#Affiche le graphique 
	gnuplot "graph_d2.gp"
	
	image_path="images/horizontal_d2.png"

	# Vérifier si xdg-open est disponible
	if command -v xdg-open &> /dev/null; then
	    xdg-open "$image_path"
	else
	    echo "xdg-open n'est pas disponible sur votre système."
	fi

	# Enregistrez le temps de fin
	end_time=$(date +%s)

	# Calculez la différence de temps
	execution_time=$(echo "$end_time - $start_time" | bc)

	echo "Le traitement [D2] a pris $execution_time secondes pour s'exécuter."

fi

exit 0
