// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/menu.h"
#include "../includes/functionsDisplay.h"

int menu(Window* window) {

    // Créer les textures
    SDL_Texture* buttonSelectTexture = textureFromImage(window->renderer, "img/arrow_select.png");
    SDL_Texture* buttonSelectHoverTexture = textureFromImage(window->renderer, "img/arrow_select_hover.png");

    SDL_Texture* createPacketTexture = textureFromMessage(window->renderer, "Creer un paquet", setColor("Black"), window->font);
    SDL_Texture* createPacketHoverTexture = textureFromMessage(window->renderer, "Creer un paquet", setColor("Blue"), window->font);
    SDL_Texture* managePacketTexture = textureFromMessage(window->renderer, "Gerer mes paquets", setColor("Black"), window->font);
    SDL_Texture* managePacketHoverTexture = textureFromMessage(window->renderer, "Gerer mes paquets", setColor("Blue"), window->font);
    SDL_Texture* hostGameTexture = textureFromMessage(window->renderer, "Heberger une partie", setColor("Black"), window->font);
    SDL_Texture* hostGameHoverTexture = textureFromMessage(window->renderer, "Heberger une partie", setColor("Blue"), window->font);
    SDL_Texture* joinGameTexture = textureFromMessage(window->renderer, "Rejoindre une partie", setColor("Black"), window->font);
    SDL_Texture* joinGameHoverTexture = textureFromMessage(window->renderer, "Rejoindre une partie", setColor("Blue"), window->font);

    States* select = setStates(buttonSelectTexture, buttonSelectHoverTexture);
    States* createPacketText = setStates(createPacketTexture, createPacketHoverTexture);
    States* managePacketText = setStates(managePacketTexture, managePacketHoverTexture);
    States* hostGameText = setStates(hostGameTexture, hostGameHoverTexture);
    States* joinGameText = setStates(joinGameTexture, joinGameHoverTexture);

    // Définir les dimensions
    int selectX = 425, selectY = 75, selectStartY = 175;
    int selectTextX = selectX + SELECT_WIDTH + 10;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttoncreatePacketRect = {
        selectX,
        selectStartY + selectY,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttoncreatePacketTextRect = {
        selectTextX,
        buttoncreatePacketRect.y - SELECT_HEIGHT/2,
        getTextWidth("Creer un paquet", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };
    SDL_Rect buttonmanagePacketRect = {
        selectX,
        selectStartY + selectY * 2,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonmanagePacketTextRect = {
        selectTextX,
        buttonmanagePacketRect.y - SELECT_HEIGHT/2,
        getTextWidth("Gerer mes paquets", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };
    SDL_Rect buttonhostGameRect = {
        selectX,
        selectStartY + selectY * 3,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonhostGameTextRect = {
        selectTextX,
        buttonhostGameRect.y - SELECT_HEIGHT/2,
        getTextWidth("Heberger une partie", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };
    SDL_Rect buttonjoinGameRect = {
        selectX,
        selectStartY + selectY * 4,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonjoinGameTextRect = {
        selectTextX,
        buttonjoinGameRect.y - SELECT_HEIGHT/2,
        getTextWidth("Rejoindre une partie", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };



    Node* first = NULL;
    addTemplateToList(&first, window, 1, 0, 1, "===MENU===");
    addButtonToList(&first, buttoncreatePacketRect, select, buttoncreatePacketTextRect, createPacketText, 1, MENU_CREATE_PACKET);
    addButtonToList(&first, buttonmanagePacketRect, select, buttonmanagePacketTextRect, managePacketText, 1, MENU_MANAGE_PACKET);
    addButtonToList(&first, buttonhostGameRect, select, buttonhostGameTextRect, hostGameText, 1, MENU_HOST_GAME);
    addButtonToList(&first, buttonjoinGameRect, select, buttonjoinGameTextRect, joinGameText, 1, MENU_JOIN_GAME);

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
                            if (current->button.isHovered && current->button.returnValue){
                                return current->button.returnValue;
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