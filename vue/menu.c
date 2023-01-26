// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/menu.h"
#include "../includes/functionsDisplay.h"

int menu(Window* window) {

    TTF_Init();
    if (TTF_Init() != 0)
    {
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    // Créer les textures
    SDL_Texture* buttonSelectTexture = textureFromImage(window->renderer, "img/arrow_select.png");
    SDL_Texture* buttonSelectHoverTexture = textureFromImage(window->renderer, "img/arrow_select_hover.png");
    SDL_Texture* buttonLeaveTexture = textureFromImage(window->renderer, "img/quit_program.png");
    SDL_Texture* buttonLeaveHoverTexture = textureFromImage(window->renderer, "img/quit_program_hover.png");
    SDL_Texture* buttonSettingsTexture = textureFromImage(window->renderer, "img/settings.png");
    SDL_Texture* buttonSettingsHoverTexture = textureFromImage(window->renderer, "img/settings_hover.png");
    
    SDL_Texture* cahootTexture = textureFromMessage(window->renderer, "Cahoot", setColor("Black"));
    SDL_Texture* menuTexture = textureFromMessage(window->renderer, "===MENU===", setColor("Black"));
    SDL_Texture* select1Texture = textureFromMessage(window->renderer, "Creer un paquet", setColor("Black"));
    SDL_Texture* select1HoverTexture = textureFromMessage(window->renderer, "Creer un paquet", setColor("Blue"));
    SDL_Texture* select2Texture = textureFromMessage(window->renderer, "Gerer mes paquets", setColor("Black"));
    SDL_Texture* select2HoverTexture = textureFromMessage(window->renderer, "Gerer mes paquets", setColor("Blue"));
    SDL_Texture* select3Texture = textureFromMessage(window->renderer, "Heberger une partie", setColor("Black"));
    SDL_Texture* select3HoverTexture = textureFromMessage(window->renderer, "Heberger une partie", setColor("Blue"));
    SDL_Texture* select4Texture = textureFromMessage(window->renderer, "Rejoindre une partie", setColor("Black"));
    SDL_Texture* select4HoverTexture = textureFromMessage(window->renderer, "Rejoindre une partie", setColor("Blue"));

    States* logo = setStates(cahootTexture, cahootTexture);
    States* menu = setStates(menuTexture, menuTexture);
    States* settings = setStates(buttonSettingsTexture, buttonSettingsHoverTexture);
    States* quitApp = setStates(buttonLeaveTexture, buttonLeaveHoverTexture);
    States* select = setStates(buttonSelectTexture, buttonSelectHoverTexture);
    States* select1Text = setStates(select1Texture, select1HoverTexture);
    States* select2Text = setStates(select2Texture, select2HoverTexture);
    States* select3Text = setStates(select3Texture, select3HoverTexture);
    States* select4Text = setStates(select4Texture, select4HoverTexture);

    // Définir les dimensions
    int margin = 24;
    int buttonSelectWidth = 25, buttonSelectHeight = 25;
    int buttonLeaveWidth = 50, buttonLeaveHeight = 50;
    int buttonSettingsWidth = 50, buttonSettingsHeight = 50;
    int selectX = 425, selectY = 75, selectStartY = 175;
    int selectTextX = selectX + buttonSelectWidth + 10;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonSettingsRect = {
        SCREEN_WIDTH - buttonSettingsWidth - margin,
        margin,
        buttonSettingsWidth,
        buttonSettingsHeight
    };
    SDL_Rect logoRect = {
        (SCREEN_WIDTH - getTextWidth("Cahoot", 0, 100)) / 2,
        margin / 10,
        getTextWidth("Cahoot", 0, 100),
        100
    };
    SDL_Rect menuRect = {
        (SCREEN_WIDTH - getTextWidth("===MENU===", 0, 50)) / 2,
        150,
        getTextWidth("===MENU===", 0, 50),
        50
    };
    SDL_Rect buttonSelect1Rect = {
        selectX,
        selectStartY + selectY,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect1TextRect = {
        selectTextX,
        buttonSelect1Rect.y - buttonSelectHeight/2,
        getTextWidth("Creer un paquet", 0, 2*buttonSelectHeight),
        2*buttonSelectHeight
    };
    SDL_Rect buttonSelect2Rect = {
        selectX,
        selectStartY + selectY * 2,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect2TextRect = {
        selectTextX,
        buttonSelect2Rect.y - buttonSelectHeight/2,
        getTextWidth("Gerer mes paquets", 0, 2*buttonSelectHeight),
        2*buttonSelectHeight
    };
    SDL_Rect buttonSelect3Rect = {
        selectX,
        selectStartY + selectY * 3,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect3TextRect = {
        selectTextX,
        buttonSelect3Rect.y - buttonSelectHeight/2,
        getTextWidth("Heberger une partie", 0, 2*buttonSelectHeight),
        2*buttonSelectHeight
    };
    SDL_Rect buttonSelect4Rect = {
        selectX,
        selectStartY + selectY * 4,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect4TextRect = {
        selectTextX,
        buttonSelect4Rect.y - buttonSelectHeight/2,
        getTextWidth("Rejoindre une partie", 0, 2*buttonSelectHeight),
        2*buttonSelectHeight
    };
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - buttonLeaveWidth) / 2,
        (SCREEN_WIDTH - buttonLeaveHeight) / 2 - margin,
        buttonLeaveWidth,
        buttonLeaveHeight
    };



    Node* first = NULL;
    addButtonToList(&first, logoRect, logo, empty(), NULL, 0, 0);
    addButtonToList(&first, menuRect, menu, empty(), NULL, 0, 0);
    addButtonToList(&first, buttonLeaveRect, quitApp, empty(), NULL, 0, 1);
    addButtonToList(&first, buttonSettingsRect, settings, empty(), NULL, 0, 2);
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