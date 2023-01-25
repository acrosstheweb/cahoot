// TODO: cleanup on leave function to free all resources in array
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
    SDL_Texture* select1Texture = textureFromMessage(window->renderer, "Créer un paquet", 0);
    SDL_Texture* select1HoverTexture = textureFromMessage(window->renderer, "Créer un paquet", 0);

    States* logo = setStates(cahootTexture, cahootTexture);
    States* settings = setStates(buttonSettingsTexture, buttonSettingsHoverTexture);
    States* quitApp = setStates(buttonLeaveTexture, buttonLeaveHoverTexture);
    States* select1 = setStates(buttonSelectTexture, buttonSelectHoverTexture);
    States* select1Text = setStates(select1Texture, select1HoverTexture);

    // Définir les dimensions
    int margin = 24;
    int buttonSelectWidth = 50, buttonSelectHeight = 50;
    int buttonLeaveWidth = 50, buttonLeaveHeight = 50;
    int buttonSettingsWidth = 50, buttonSettingsHeight = 50;

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
    SDL_Rect buttonSelect1Rect = {
        (SCREEN_WIDTH - buttonSelectWidth) / 2 - buttonSelectWidth,
        100,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect1TextRect = {
        (SCREEN_WIDTH - buttonSelectWidth) / 2 - buttonSelectWidth,
        100,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect2Rect = {
        (SCREEN_WIDTH - buttonSelectWidth) / 2 - buttonSelectWidth,
        200,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - buttonLeaveWidth) / 2,
        SCREEN_HEIGHT - (SCREEN_HEIGHT / 6),
        buttonLeaveWidth,
        buttonLeaveHeight
    };



    Node* first = NULL;
    addButtonToList(&first, buttonSettingsRect, settings, emptyRect, NULL);
    addButtonToList(&first, buttonLeaveRect, quitApp, emptyRect, NULL);
    addButtonToList(&first, buttonSelect1Rect, select1, buttonSelect1TextRect, select1Text);
    
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
                    break;
                case SDL_QUIT:
                    return;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);

        Node *current = first;
        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }
        SDL_RenderCopy(window->renderer, cahootTexture, NULL, &logoRect);

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return;
}

