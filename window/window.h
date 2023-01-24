// TODO: remplacer option par toutes les options possibles
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Window *sdl_window;
    SDL_Renderer *renderer;
} Window;

void menu(Window* window);

void option(int option);