#include "window.h"

Window *create_window()
{
    Window *window = malloc(sizeof(Window));

    // Initialisation de la fenêtre
    window->sdl_window = SDL_CreateWindow("SDL_TP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window->sdl_window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    // Initialisation du fond de la fenêtre ? Enfin, je crois
    window->renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (window->renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(window->sdl_window);
        SDL_Quit();
        exit(-1);
    }

    SDL_SetRenderDrawColor(window->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(window->renderer);
    SDL_RenderPresent(window->renderer);

    return window;

}

void destroy_window(Window *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdl_window);
    free(window);
}