# C-vraiment-pas-cool
Projet langage C - S1 - 2A2

## Prérequis
sudo apt install libsdl2-dev
sudo apt install libsdl2-ttf-dev

## Commande pour compiler
    gcc main.c lib/*.c window/*.c $(sdl2-config --cflags --libs) -o main