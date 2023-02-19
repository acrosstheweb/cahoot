# C-vraiment-pas-cool
Projet langage C - S1 - 2A2

## Prérequis
    sudo apt install libsdl2-dev
    sudo apt install libsdl2-image-dev
    sudo apt install libsdl2-ttf-dev

## Commande pour compiler
    gcc main.c functions.c window.c $(sdl2-config --cflags --libs) -lSDL2 -lSDL2_image -lSDL2_ttf -o main -g -pthread
ou
    gcc main.c lib/*.c vue/*.c -lSDL2 -lSDL2_image -lSDL2_ttf -o main -g -pthread
