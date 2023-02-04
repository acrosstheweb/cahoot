// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/functionsDisplay.h"
#include "../includes/functionsPacket.h"


int managePackets(Window* window) {
    char** packetList = NULL;
    int* packetNb = malloc(sizeof(int));
    packetList = listPackets(packetNb);

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
    addTemplateToList(&first, window, 1, 1, 1, "Mes paquets");
    // addButtonToList(&first, redRect, red, empty(), NULL, 1, 4);
    // addButtonToList(&first, greenRect, green, empty(), NULL, 1, 4);
    // addButtonToList(&first, blueRect, blue, empty(), NULL, 1, 4);
    // addButtonToList(&first, yellowRect, yellow, empty(), NULL, 1, 4);

    for (int i = 0; i < *packetNb; i++){
        SDL_Texture* textTexture = textureFromMessage(window->renderer, *(packetList + i), setColor("Black"), window->font);
        States* text = setStates(textTexture, textTexture);
        switch (i % 4){
        case 0:
            addButtonToList(&first, redRect, red, (SDL_Rect){
                redRect.x + (redRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                redRect.y + (redRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 4);
            break;

        case 1:
            addButtonToList(&first, greenRect, green, (SDL_Rect){
                greenRect.x + (greenRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                greenRect.y + (greenRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 4);
            break;

        case 2:
            addButtonToList(&first, blueRect, blue, (SDL_Rect){
                blueRect.x + (blueRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                blueRect.y + (blueRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 4);
            break;

        case 3:
            addButtonToList(&first, yellowRect, yellow, (SDL_Rect){
                yellowRect.x + (yellowRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                yellowRect.y + (yellowRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 4);
            break;
        
        default:
            break;
        }
    }
    
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