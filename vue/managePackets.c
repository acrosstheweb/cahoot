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
    int page = 0;

    // Créer les textures
    SDL_Texture* nextTexture = textureFromImage(window->renderer, "img/next.png");
    SDL_Texture* nextHoverTexture = textureFromImage(window->renderer, "img/next_hover.png");
    SDL_Texture* prevTexture = textureFromImage(window->renderer, "img/prev.png");
    SDL_Texture* prevHoverTexture = textureFromImage(window->renderer, "img/prev_hover.png");
    SDL_Texture* redRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");
    SDL_Texture* blueRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    SDL_Texture* yellowRectTexture = textureFromImage(window->renderer, "img/rect_yellow.png");
    SDL_Texture* yellowRectHoverTexture = textureFromImage(window->renderer, "img/rect_yellow_hover.png");
    
    States* next = setStates(nextTexture, nextHoverTexture);
    States* prev = setStates(prevTexture, prevHoverTexture);
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);
    States* blue = setStates(blueRectTexture, blueRectHoverTexture);
    States* yellow = setStates(yellowRectTexture, yellowRectHoverTexture);

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect nextRect = {
        SCREEN_WIDTH - MARGIN - NEXT_RECT_WIDTH,
        (SCREEN_HEIGHT - NEXT_RECT_HEIGHT) / 2,
        NEXT_RECT_WIDTH,
        NEXT_RECT_HEIGHT
    };
    SDL_Rect prevRect = {
        MARGIN,
        (SCREEN_HEIGHT - PREV_RECT_HEIGHT) / 2,
        PREV_RECT_WIDTH,
        PREV_RECT_HEIGHT
    };
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
    addTemplateToList(&first, window, 1, 1, 1, "===MES PAQUETS===");
    addButtonToList(&first, prevRect, prev, empty(), NULL, 1, 11);
    addButtonToList(&first, nextRect, next, empty(), NULL, 1, 12);

    int packetNbOnPage = (page < (*packetNb) / 4) ? 4 : (*packetNb) % 4;
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
                }, text, 1, 15 + i);
            break;

        case 1:
            addButtonToList(&first, greenRect, green, (SDL_Rect){
                greenRect.x + (greenRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                greenRect.y + (greenRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 15 + i);
            break;

        case 2:
            addButtonToList(&first, blueRect, blue, (SDL_Rect){
                blueRect.x + (blueRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                blueRect.y + (blueRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 15 + i);
            break;

        case 3:
            addButtonToList(&first, yellowRect, yellow, (SDL_Rect){
                yellowRect.x + (yellowRect.w- getTextWidth(*(packetList + i), 50)) / 2,
                yellowRect.y + (yellowRect.h - 50) / 2,
                getTextWidth(*(packetList + i), 50),
                50
                }, text, 1, 15 + i);
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
                                if (current->button.isClickable <= 10){
                                    return current->button.isClickable;
                                } else if (current->button.isClickable == 11){
                                    if (page > 0){
                                        printf("page--");
                                        page--;
                                    }
                                } else if (current->button.isClickable == 12){
                                    if (page < *packetNb / 4){
                                        printf("page++");
                                        page++;
                                    }
                                }
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

        if (page == 0){
        } else if (page == (*packetNb) / 4){

        }

        if (first != NULL) {
            do {
                if (current->button.isClickable <= 10){
                    display(window->renderer, current->button);
                } else if (current->button.isClickable < 15){
                    if ((current->button.icon == prev && page > 0) || (current->button.icon == next && page < *packetNb / 4)){
                        display(window->renderer, current->button);
                    }
                } else {
                    if (current->button.isClickable - 15 >= page * 4 && current->button.isClickable - 15 < (page+1) * 4){
                        display(window->renderer, current->button);
                    }
                }
                current = current->next;
            }while (current != first);
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}