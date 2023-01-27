// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/functionsDisplay.h"


int createPacket(Window* window) {
    int colorNum = rand() % 4;

    TTF_Init();
    if (TTF_Init() != 0)
    {
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    // Créer les textures
    SDL_Texture* colorRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    
    if (colorNum == 0){
        colorRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
        colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    } else if (colorNum == 1){
        colorRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
        colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");
    } else if (colorNum == 2){
        colorRectTexture = textureFromImage(window->renderer, "img/rect_yellow.png");
        colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_yellow_hover.png");
    }
    States* color = setStates(colorRectTexture, colorRectHoverTexture);


    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect colorRect = {
        (SCREEN_WIDTH - PACKET_RECT_WIDTH) / 2,
        (SCREEN_HEIGHT - PACKET_RECT_HEIGHT) / 2,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };

    Node* first = NULL;
    addTemplateToList(&first, window->renderer, 1, 1, 1, "Comment voulez-vous appeler votre paquet?");
    addButtonToList(&first, colorRect, color, empty(), NULL, 0, 1);
    
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        Node* current = first;
        int x, y;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEMOTION:
                    x = e.motion.x;
                    y = e.motion.y;
                    checkHover(first, x, y);
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    x = e.motion.x;
                    y = e.motion.y;
                    checkHover(first, x, y);
                    if (first != NULL) {
                        do {
                            if (current->button.isHovered && current->button.isClickable){
                                return current->button.isClickable;
                            }
                            current = current->next;
                        }while (current != first);
                    }
                    break;

                case SDL_QUIT:
                    return 1;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);

        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}