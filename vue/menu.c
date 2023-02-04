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

    SDL_Texture* select1Texture = textureFromMessage(window->renderer, "Creer un paquet", setColor("Black"), window->font);
    SDL_Texture* select1HoverTexture = textureFromMessage(window->renderer, "Creer un paquet", setColor("Blue"), window->font);
    SDL_Texture* select2Texture = textureFromMessage(window->renderer, "Gerer mes paquets", setColor("Black"), window->font);
    SDL_Texture* select2HoverTexture = textureFromMessage(window->renderer, "Gerer mes paquets", setColor("Blue"), window->font);
    SDL_Texture* select3Texture = textureFromMessage(window->renderer, "Heberger une partie", setColor("Black"), window->font);
    SDL_Texture* select3HoverTexture = textureFromMessage(window->renderer, "Heberger une partie", setColor("Blue"), window->font);
    SDL_Texture* select4Texture = textureFromMessage(window->renderer, "Rejoindre une partie", setColor("Black"), window->font);
    SDL_Texture* select4HoverTexture = textureFromMessage(window->renderer, "Rejoindre une partie", setColor("Blue"), window->font);

    States* select = setStates(buttonSelectTexture, buttonSelectHoverTexture);
    States* select1Text = setStates(select1Texture, select1HoverTexture);
    States* select2Text = setStates(select2Texture, select2HoverTexture);
    States* select3Text = setStates(select3Texture, select3HoverTexture);
    States* select4Text = setStates(select4Texture, select4HoverTexture);

    // Définir les dimensions
    int selectX = 425, selectY = 75, selectStartY = 175;
    int selectTextX = selectX + SELECT_WIDTH + 10;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonSelect1Rect = {
        selectX,
        selectStartY + selectY,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonSelect1TextRect = {
        selectTextX,
        buttonSelect1Rect.y - SELECT_HEIGHT/2,
        getTextWidth("Creer un paquet", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };
    SDL_Rect buttonSelect2Rect = {
        selectX,
        selectStartY + selectY * 2,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonSelect2TextRect = {
        selectTextX,
        buttonSelect2Rect.y - SELECT_HEIGHT/2,
        getTextWidth("Gerer mes paquets", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };
    SDL_Rect buttonSelect3Rect = {
        selectX,
        selectStartY + selectY * 3,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonSelect3TextRect = {
        selectTextX,
        buttonSelect3Rect.y - SELECT_HEIGHT/2,
        getTextWidth("Heberger une partie", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };
    SDL_Rect buttonSelect4Rect = {
        selectX,
        selectStartY + selectY * 4,
        SELECT_WIDTH,
        SELECT_HEIGHT
    };
    SDL_Rect buttonSelect4TextRect = {
        selectTextX,
        buttonSelect4Rect.y - SELECT_HEIGHT/2,
        getTextWidth("Rejoindre une partie", 2*SELECT_HEIGHT),
        2*SELECT_HEIGHT
    };



    Node* first = NULL;
    addTemplateToList(&first, window, 1, 0, 1, "===MENU===");
    addButtonToList(&first, buttonSelect1Rect, select, buttonSelect1TextRect, select1Text, 1, 3);
    addButtonToList(&first, buttonSelect2Rect, select, buttonSelect2TextRect, select2Text, 1, 4);
    addButtonToList(&first, buttonSelect3Rect, select, buttonSelect3TextRect, select3Text, 1, 5);
    addButtonToList(&first, buttonSelect4Rect, select, buttonSelect4TextRect, select4Text, 1, 6);
    
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