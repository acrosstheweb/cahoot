// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/functionsDisplay.h"


int managePackets(Window* window) {

    TTF_Init();
    if (TTF_Init() != 0)
    {
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    // Créer les textures
    SDL_Texture* redRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");
    SDL_Texture* blueRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    SDL_Texture* yellowRectTexture = textureFromImage(window->renderer, "img/rect_yellow.png");
    SDL_Texture* yellowRectHoverTexture = textureFromImage(window->renderer, "img/rect_yellow_hover.png");
    
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);
    States* blue = setStates(blueRectTexture, blueRectHoverTexture);
    States* yellow = setStates(yellowRectTexture, yellowRectHoverTexture);

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect redRect = {
        PACKET_RECT_STARTX + MARGIN / 4,
        PACKET_RECT_STARTY + MARGIN / 2,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect greenRect = {
        PACKET_RECT_ENDX - PACKET_RECT_WIDTH - MARGIN / 4,
        PACKET_RECT_STARTY + MARGIN / 2,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect blueRect = {
        PACKET_RECT_STARTX + MARGIN / 4,
        PACKET_RECT_ENDY -PACKET_RECT_HEIGHT-MARGIN,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect yellowRect = {
        PACKET_RECT_ENDX - PACKET_RECT_WIDTH - MARGIN / 4,
        PACKET_RECT_ENDY - PACKET_RECT_HEIGHT - MARGIN,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };



    Node* first = NULL;
    addTemplateToList(&first, window->renderer, 1, 1, 1, "Mes paquets");
    addButtonToList(&first, redRect, red, empty(), NULL, 1, 4);
    addButtonToList(&first, greenRect, green, empty(), NULL, 1, 4);
    addButtonToList(&first, blueRect, blue, empty(), NULL, 1, 4);
    addButtonToList(&first, yellowRect, yellow, empty(), NULL, 1, 4);
    
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