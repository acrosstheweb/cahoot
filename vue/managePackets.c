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
    SDL_Texture* buttonMenuTexture = textureFromImage(window->renderer, "img/back_to_menu.png");
    SDL_Texture* buttonMenuHoverTexture = textureFromImage(window->renderer, "img/back_to_menu_hover.png");
    SDL_Texture* buttonLeaveTexture = textureFromImage(window->renderer, "img/quit_program.png");
    SDL_Texture* buttonLeaveHoverTexture = textureFromImage(window->renderer, "img/quit_program_hover.png");
    SDL_Texture* buttonSettingsTexture = textureFromImage(window->renderer, "img/settings.png");
    SDL_Texture* buttonSettingsHoverTexture = textureFromImage(window->renderer, "img/settings_hover.png");

    SDL_Texture* redRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");
    SDL_Texture* blueRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    SDL_Texture* yellowRectTexture = textureFromImage(window->renderer, "img/rect_yellow.png");
    SDL_Texture* yellowRectHoverTexture = textureFromImage(window->renderer, "img/rect_yellow_hover.png");
    
    SDL_Texture* cahootTexture = textureFromMessage(window->renderer, "Cahoot", setColor("Black"));
    SDL_Texture* titleTexture = textureFromMessage(window->renderer, "Comment voulez-vous appeler votre paquet?", setColor("Black"));
    SDL_Texture* menuTextTexture = textureFromMessage(window->renderer, "Revenir au menu principal", setColor("Black"));
    SDL_Texture* menuTextHoverTexture = textureFromMessage(window->renderer, "Revenir au menu principal", setColor("Yellow"));
    

    States* logo = setStates(cahootTexture, cahootTexture);
    States* title = setStates(titleTexture, titleTexture);
    States* settings = setStates(buttonSettingsTexture, buttonSettingsHoverTexture);
    States* quitApp = setStates(buttonLeaveTexture, buttonLeaveHoverTexture);
    States* menu = setStates(buttonMenuTexture, buttonMenuHoverTexture);
    States* menuText = setStates(menuTextTexture, menuTextHoverTexture);
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonMenuRect = {
        MARGIN,
        MARGIN + (SETTINGS_HEIGHT - MENU_HEIGHT)/2,
        MENU_WIDTH,
        MENU_HEIGHT
    };
    SDL_Rect menuTextRect = {
        buttonMenuRect.x + buttonMenuRect.w + 10,
        buttonMenuRect.y,
        getTextWidth("Revenir au menu principal", 0, MENU_HEIGHT),
        MENU_HEIGHT
    };
    SDL_Rect buttonSettingsRect = {
        SCREEN_WIDTH - SETTINGS_WIDTH - MARGIN,
        MARGIN,
        SETTINGS_WIDTH,
        SETTINGS_HEIGHT
    };
    SDL_Rect logoRect = {
        (SCREEN_WIDTH - getTextWidth("Cahoot", 0, 100)) / 2,
        MARGIN / 10,
        getTextWidth("Cahoot", 0, 100),
        100
    };
    SDL_Rect titleRect = {
        (SCREEN_WIDTH - getTextWidth("Comment voulez-vous appeler votre paquet?", 0, 50)) / 2,
        100,
        getTextWidth("Comment voulez-vous appeler votre paquet?", 0, 50),
        50
    };
    SDL_Rect redRect = {
        PACKET_RECT_STARTX,
        PACKET_RECT_STARTY + MARGIN,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect greenRect = {
        PACKET_RECT_ENDX - PACKET_RECT_WIDTH,
        PACKET_RECT_STARTY + MARGIN,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - LEAVE_WIDTH) / 2,
        SCREEN_HEIGHT - LEAVE_HEIGHT - MARGIN,
        LEAVE_WIDTH,
        LEAVE_HEIGHT
    };
    // printf("%d\n", buttonLeaveRect.y);



    Node* first = NULL;
    addButtonToList(&first, logoRect, logo, empty(), NULL, 0, 0);
    addButtonToList(&first, titleRect, title, empty(), NULL, 0, 0);
    addButtonToList(&first, buttonLeaveRect, quitApp, empty(), NULL, 0, 1);
    addButtonToList(&first, buttonSettingsRect, settings, empty(), NULL, 0, 2);
    addButtonToList(&first, buttonMenuRect, menu, menuTextRect, menuText, 0, 3);
    addButtonToList(&first, redRect, red, empty(), NULL, 1, 4);
    addButtonToList(&first, greenRect, green, empty(), NULL, 1, 4);
    
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