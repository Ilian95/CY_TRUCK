#!/bin/bash

#------------------------------------------------------------------------------------------------------------------------------------------------
# 						VERIFICATIONS
#------------------------------------------------------------------------------------------------------------------------------------------------

# Vérifier si l'argument -h est présent parmi les paramètres
if [[ "$*" == *-h* ]]; then
    echo -e "Aide : Le chemin du fichier de données doit être le premier argument et est obligatoire.\nUtilisation : $0 [chemin_du_fichier] [traitement 1]"
    exit 0
fi

#Verifie le nb d'arguments
if [ "$#" -eq 0 ]; then
	echo "Erreur: Veuillez spécifier le chemin du fichier de données en paramètre et au moins un traitement"
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
dossier1="temp"
contenu=$(ls -A "$dossier1" | wc -l)
if [ ! -e "$dossier1" ]; then
	echo "Le dossier $dossier1 n'existe pas"
	mkdir "$dossier1"
else
	if  [ ! "$contenu" -eq 0 ]; then
		rm -r "$dossier1"/*
	fi
fi

#Verifie la présence de images et le crée si il n'existe pas
dossier2="images"
if [ ! -e "$dossier2" ]; then
	echo "Le dossier $dossier2 n'existe pas"
	mkdir "$dossier2"
fi

#------------------------------------------------------------------------------------------------------------------------------------------------
# 						TRAITEMENT D1
#------------------------------------------------------------------------------------------------------------------------------------------------

if [[ "$*" == *-d1* ]]; then

	# Enregistrez le temps de début
	start_time=$(date +%s.%N)

	# Créer un dossier temporaire
	tempresult=$(mktemp -d)

	# Supprimer les doublons dans les colonnes 1 et 6
	awk -F ';' '!seen[$1,$6]++' "$file" > "$tempresult/unique_data.csv"

	# Extraire les noms des conducteurs et le nombre de trajets avec tri et comptage
	awk -F ';' '{if ($6 != "") count[$6]++} END {for (name in count) print count[name], name}' "$tempresult/unique_data.csv" | sort -nrt " " -k1 | head -n 10 > "$tempresult/resultat1.txt"

	# Afficher les résultats
	awk '{print $2, $3}' "$tempresult/resultat1.txt"

	# Enregistrez le temps de fin
	end_time=$(date +%s.%N)

	# Calculez la différence de temps
	execution_time=$(echo "$end_time - $start_time" | bc)

	echo "Le script a pris $execution_time secondes pour s'exécuter."

	# Supprimer le dossier temporaire
	rm -r "$tempresult"
fi

exit 0
