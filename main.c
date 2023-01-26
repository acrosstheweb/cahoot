// IDEA: dossier de gif applaudissement en random si bonne réponse
// IDEA: meme chat (animation tournante) si mauvaise réponse
#include <stdio.h>
#include <stdlib.h>
#include "includes/struct.h"
#include "includes/functionsDisplay.h"
#include "includes/menu.h"

Window* create_window();
void destroy_window(Window* window);

Window* create_window() {
    SDL_Init(SDL_INIT_VIDEO);

    Window* window = malloc(sizeof(Window));
    if(window == NULL){
        printf("Echec malloc window");
        exit(EXIT_FAILURE);
    }

    // Initialisation de la fenêtre
    window->sdl_window = SDL_CreateWindow("Cahoot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window->sdl_window == NULL) {
        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    window->renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);

    return window;

}

void destroy_window(Window *window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdl_window);
    free(window);
    SDL_Quit();
    return;
}


int main() {
    int choice;
    Window* window = create_window();

    do{
        choice = menu(window);

        switch (choice) {
            case 1:
                break;

            
            case 2:
            break;
            
            default:
                /*
                Afficher message (en ligne de commande au cas où ca crash ? )
                "Oups, je ne sais pas ce qui a bien pu se passer...\n"
                       "Vous pouvez nous envoyer un message avec la manip effectuer pour que nous puissions regarder ça de plus près ! ;)\n"
                */
                break;
        }
        
    }while (choice != 1);

    destroy_window(window);
    return 0;
}
