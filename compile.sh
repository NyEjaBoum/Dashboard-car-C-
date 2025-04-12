#!/bin/bash

# Compiler les fichiers source .cpp en fichiers objets .o
g++ -c main.cpp -o main.o
g++ -c Connexion.cpp -o Connexion.o
g++ -c Voiture.cpp -o Voiture.o
g++ -c Fenetre.cpp -o Fenetre.o
g++ -c Acceleration_Log.cpp -o Acceleration_Log.o

# Lier tous les fichiers objets pour générer l'exécutable
g++ main.o Connexion.o Voiture.o Fenetre.o Acceleration_Log.o -o m -lmysqlcppconn -lsfml-graphics -lsfml-window -lsfml-system

# Nettoyage des fichiers objets après la compilation
rm -f *.o

echo "Compilation terminée. Exécutable 'm' généré."

./m
