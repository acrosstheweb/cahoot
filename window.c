// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "includes/window.h"
#include "includes/functions.h"

SDL_Rect emptyRect = {0, 0, 0, 0};

/*
    printf("---- MENU ----\n");
    printf("1 - Créer un Paquet\n");
    printf("2 - Voir mes paquets\n");
    printf("3 - EXIT\n");
 */
void menu(Window* window) {

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
    
    SDL_Texture* cahootTexture = textureFromMessage(window->renderer, "Cahoot", 0);
    SDL_Texture* menuTexture = textureFromMessage(window->renderer, "===MENU===", 0);
    SDL_Texture* select1Texture = textureFromMessage(window->renderer, "Creer un paquet", 0);
    SDL_Texture* select1HoverTexture = textureFromMessage(window->renderer, "Creer un paquet", 1);
    SDL_Texture* select2Texture = textureFromMessage(window->renderer, "Gerer mes paquets", 0);
    SDL_Texture* select2HoverTexture = textureFromMessage(window->renderer, "Gerer mes paquets", 1);
    SDL_Texture* select3Texture = textureFromMessage(window->renderer, "Heberger une partie", 0);
    SDL_Texture* select3HoverTexture = textureFromMessage(window->renderer, "Heberger une partie", 1);
    SDL_Texture* select4Texture = textureFromMessage(window->renderer, "Rejoindre une partie", 0);
    SDL_Texture* select4HoverTexture = textureFromMessage(window->renderer, "Rejoindre une partie", 1);

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
    int selectTextX = selectX + buttonSelectWidth+10;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonSettingsRect = {
        SCREEN_WIDTH - buttonSettingsWidth - margin,
        margin,
        buttonSettingsWidth,
        buttonSettingsHeight
    };
    SDL_Rect logoRect = {
        (SCREEN_WIDTH - 250) / 2,
        margin / 10,
        250,
        100
    };
    SDL_Rect menuRect = {
        (SCREEN_WIDTH - 250) / 2,
        150,
        250,
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
        400,
        2*buttonSelect1Rect.h
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
        400,
        2*buttonSelect2Rect.h
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
        400,
        2*buttonSelect3Rect.h
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
        400,
        2*buttonSelect4Rect.h
    };
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - buttonLeaveWidth) / 2,
        (SCREEN_WIDTH - buttonLeaveHeight) / 2 - margin,
        buttonLeaveWidth,
        buttonLeaveHeight
    };



    Node* first = NULL;
    addButtonToList(&first, logoRect, logo, emptyRect, NULL);
    addButtonToList(&first, menuRect, menu, emptyRect, NULL);
    addButtonToList(&first, buttonSettingsRect, settings, emptyRect, NULL);
    addButtonToList(&first, buttonLeaveRect, quitApp, emptyRect, NULL);
    Node* firstOption = NULL;
    addButtonToList(&firstOption, buttonSelect1Rect, select, buttonSelect1TextRect, select1Text);
    addButtonToList(&firstOption, buttonSelect2Rect, select, buttonSelect2TextRect, select2Text);
    addButtonToList(&firstOption, buttonSelect3Rect, select, buttonSelect3TextRect, select3Text);
    addButtonToList(&firstOption, buttonSelect4Rect, select, buttonSelect4TextRect, select4Text);
    
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;

        int x, y;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEMOTION:
                    x = e.motion.x;
                    y = e.motion.y;
                    checkHover(first, x, y);
                    checkHover(firstOption, x, y);
                    break;
                case SDL_QUIT:
                    return;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);

        Node* current = first;
        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }

        current = firstOption;
        if (firstOption != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != firstOption);
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return;
}

