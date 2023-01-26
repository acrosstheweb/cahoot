// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/functionsDisplay.h"


int createPacket(Window* window) {

    TTF_Init();
    if (TTF_Init() != 0)
    {
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    // Créer les textures
    SDL_Texture* buttonMenuTexture = textureFromImage(window->renderer, "img/back_to_menu.png");
    SDL_Texture* buttonMenuHoverTexture = textureFromImage(window->renderer, "img/back_to_menu_hover.png");
    SDL_Texture* buttonLeaveTexture = textureFromImage(window->renderer, "img/quit_program.png");
    SDL_Texture* buttonLeaveHoverTexture = textureFromImage(window->renderer, "img/quit_program_hover.png");
    SDL_Texture* buttonSettingsTexture = textureFromImage(window->renderer, "img/settings.png");
    SDL_Texture* buttonSettingsHoverTexture = textureFromImage(window->renderer, "img/settings_hover.png");
    
    SDL_Texture* cahootTexture = textureFromMessage(window->renderer, "Cahoot", setColor("Black"));
    SDL_Texture* titleTexture = textureFromMessage(window->renderer, "Comment voulez-vous appeler votre paquet?", setColor("Black"));
    SDL_Texture* menuTextTexture = textureFromMessage(window->renderer, "Revenir au menu principal", setColor("Black"));
    SDL_Texture* menuTextHoverTexture = textureFromMessage(window->renderer, "Revenir au menu principal", setColor("Blue"));
    

    States* logo = setStates(cahootTexture, cahootTexture);
    States* title = setStates(titleTexture, titleTexture);
    States* settings = setStates(buttonSettingsTexture, buttonSettingsHoverTexture);
    States* quitApp = setStates(buttonLeaveTexture, buttonLeaveHoverTexture);
    States* menu = setStates(buttonMenuTexture, buttonMenuHoverTexture);
    States* menuText = setStates(menuTextTexture, menuTextHoverTexture);

    // Définir les dimensions
    int margin = 24;
    int buttonSelectWidth = 25, buttonSelectHeight = 25;
    int buttonLeaveWidth = 50, buttonLeaveHeight = 50;
    int buttonSettingsWidth = 50, buttonSettingsHeight = 50;
    int buttonMenuWidth = 50, buttonMenuHeight = 25;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonMenuRect = {
        margin,
        margin + (buttonSettingsHeight - buttonMenuHeight)/2,
        buttonMenuWidth,
        buttonMenuHeight
    };
    SDL_Rect menuTextRect = {
        buttonMenuRect.x + buttonMenuRect.w + 10,
        buttonMenuRect.y,
        getTextWidth("Revenir au menu principal", 0, buttonMenuHeight),
        buttonMenuHeight
    };
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
    SDL_Rect titleRect = {
        (SCREEN_WIDTH - getTextWidth("Comment voulez-vous appeler votre paquet?", 0, 50)) / 2,
        150,
        getTextWidth("Comment voulez-vous appeler votre paquet?", 0, 50),
        50
    };
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - buttonLeaveWidth) / 2,
        (SCREEN_WIDTH - buttonLeaveHeight) / 2 - margin,
        buttonLeaveWidth,
        buttonLeaveHeight
    };



    Node* first = NULL;
    addButtonToList(&first, logoRect, logo, empty(), NULL, 0, 0);
    addButtonToList(&first, titleRect, title, empty(), NULL, 0, 0);
    addButtonToList(&first, buttonLeaveRect, quitApp, empty(), NULL, 0, 1);
    addButtonToList(&first, buttonSettingsRect, settings, empty(), NULL, 0, 2);
    addButtonToList(&first, buttonMenuRect, menu, menuTextRect, menuText, 0, 3);
    
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