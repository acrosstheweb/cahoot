// TODO: add resource function to add resource created to an array
// TODO: cleanup on leave function to free all resources in array
#include "window.h"

/*
    printf("---- MENU ----\n");
    printf("1 - Créer un Paquet\n");
    printf("2 - Voir mes paquets\n");
    printf("3 - Supprimer un paquet\n");
    printf("4 - Renommer un paquet\n");
    printf("5 - Ajouter une question à un paquet\n");
    printf("6 - Supprimer une question d'un paquet\n");
    printf("7 - Afficher les questions d'un paquet\n");
    printf("8 - EXIT\n");
 */
void menu(Window* window) {

    // Charger les images des boutons
    SDL_Surface* buttonSelect = IMG_Load("img/arrow_select.png");
    SDL_Surface* buttonLeave = IMG_Load("img/quit_program.png");

    // Créer les textures des boutons
    SDL_Texture* buttonSelectTexture = SDL_CreateTextureFromSurface(window->renderer, buttonSelect);
    SDL_Texture* buttonLeaveTexture = SDL_CreateTextureFromSurface(window->renderer, buttonLeave);

    // Obtenir les dimensions des boutons
    int buttonSelectWidth = 50, buttonSelectHeight = 50;
    int buttonLeaveWidth = 75, buttonLeaveHeight = 75;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonSelect1Rect = {
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

    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            // continuer l'événement de la souris
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (
                x >= buttonSelect1Rect.x && x <= buttonSelect1Rect.x + buttonSelect1Rect.w &&
                y >= buttonSelect1Rect.y && y <= buttonSelect1Rect.y + buttonSelect1Rect.h) {
                    option(1);
                } else if (
                x >= buttonSelect2Rect.x && x <= buttonSelect2Rect.x + buttonSelect2Rect.w &&
                y >= buttonSelect2Rect.y && y <= buttonSelect2Rect.y + buttonSelect2Rect.h) {
                    option(2);
                } else if (
                x >= buttonLeaveRect.x && x <= buttonLeaveRect.x + buttonLeaveRect.w &&
                y >= buttonLeaveRect.y && y <= buttonLeaveRect.y + buttonLeaveRect.h) {
                    SDL_DestroyTexture(buttonSelectTexture);
                    SDL_DestroyTexture(buttonLeaveTexture);
                    SDL_FreeSurface(buttonSelect);
                    SDL_FreeSurface(buttonLeave);
                    SDL_DestroyRenderer(window->renderer);
                    SDL_DestroyWindow(window->sdl_window);
                    return;
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(window->renderer);

        // Place les boutons sur la fenêtre
        SDL_RenderCopy(window->renderer, buttonSelectTexture, NULL, &buttonSelect1Rect);
        SDL_RenderCopy(window->renderer, buttonSelectTexture, NULL, &buttonSelect2Rect);
        SDL_RenderCopy(window->renderer, buttonLeaveTexture, NULL, &buttonLeaveRect);

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return;
}

void option(int option) {
    printf("Option %d choisie", option);
}