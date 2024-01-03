#!/bin/bash

# Vérifier si l'argument -h est présent parmi les paramètres
if [[ "$*" == *-h* ]]; then
    echo -e "Aide : Le chemin du fichier de données doit être le premier argument et est obligatoire.\nUtilisation : $0 [chemin_du_fichier]"
    exit 0
fi

#Verifie le nb d'arguments
if [ "$#" -eq 0 ]; then
	echo "Erreur: Veuillez spécifier le chemin du fichier de données en paramètre"
	exit 1
fi

#Verifie que l'argument 1 est un fichier csv
if [ ! -e "$1" ]; then
	echo "$1 n'existe pas"
	exit 2
elif [ ! -f "$1" ]; then
	echo "$1 n'est pas un fichier"
	exit 3
else
	extension="${1##*.}"
	
	if [ ! "${extension,,}" = "csv" ]; then
		echo "$1 n'est pas un fichier csv"
		exit 4
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
if [ ! -e "$dossier1" ]; then
	echo "Le dossier $dossier1 n'existe pas"
	mkdir "$dossier1"
else 
	rm -r "$dossier1"/*
fi

#Verifie la présence de images et le crée si il n'existe pas
dossier2="images"
if [ ! -e "$dossier2" ]; then
	echo "Le dossier $dossier2 n'existe pas"
	mkdir "$dossier2"
fi

exit 0
