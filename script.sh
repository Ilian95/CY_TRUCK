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
	start_time=$(date +%s.%N)

	# Vérifier si le fichier existe et est un fichier régulier
	file="$1"
	if [ ! -f "$file" ]; then
	    echo "Erreur : Le fichier $file n'existe pas."
	    exit 6
	fi

	# Supprimer les doublons dans les colonnes 1 et 6
	awk -F ';' '!seen[$1,$6]++' "$file" > "$dossier_temp/unique_data.csv"

	# Extraire les noms des conducteurs et le nombre de trajets avec tri et comptage
	awk -F ';' '{if ($6 != "") count[$6] ++} END {for (name in count) print (name) ";" count[name]}' "$dossier_temp/unique_data.csv" | sort -nrt ";" -k2 | head -n 10 > "$dossier_temp/resultat_d1.txt"

	# Afficher les résultats
	awk '{print $1, $2, $3}' "$dossier_temp/resultat_d1.txt"

	# Enregistrez le temps de fin
	end_time=$(date +%s.%N)

	# Calculez la différence de temps
	execution_time=$(echo "$end_time - $start_time" | bc)

	echo "Le script a pris $execution_time secondes pour s'exécuter."
	
	gnuplot -e " 
	set terminal pngcairo size 800,600; 
	set output 'images/histogramme_d1.png'; 
	set datafile separator ';';
	set title 'Option -d1 : Nb Routes = f(Driver)'; 
	set xlabel 'NB ROUTES'; 
	set ylabel 'DRIVERS NAMES';
	set yrange [0:*];
	set style fill solid; 
	unset key; 
	plot 'temp/resultat_d1.txt' using (2):ytic(1) with boxes lc rgb 'skyblue'; 
	set output; 
	"
		
fi

exit 0


#------------------------------------------------------------------------------------------------------------------------------------------------
# 						TRAITEMENT L
#------------------------------------------------------------------------------------------------------------------------------------------------


#Temps de début
start_time=$(date +%s)

fichier="$1"

# Traitement [L]
if [[ "$*" == *-l* ] ]; then
        # Extraire et trier les colonnes nécessaires pour le traitement [L]
        sort -t ';' -k5nr "$fichier" | head -10 | sort -t ';' -k1 | cut -d ';' -f1,5 > temp_donnes.txt
        # Affiche les données récuperer
        cat temp_donnes.txt
        # Créer un graphique avec Gnuplot
        gnuplot -e "
                set terminal pngcairo size 800,600;
                set output 'images/histogramme_l.png';
                set datafile separator ';';
                set title 'Option -l';
                set xlabel 'Route ID';
                set ylabel 'Distance';
                set yrange [0:*];
                set style fill solid;
                unset key;
                plot 'temp_donnes.txt' using (2):ytic(1) with boxes lc rgb 'skyblue'; 
                set output;
                "

fi

# Calculer et afficher la durée d'exécution
end_time=$(date +%s)
temps=$((end_time - start_time))
echo "Durée d'exécution: $temps secondes"

echo "Traitement [L] terminé."

exit 0
