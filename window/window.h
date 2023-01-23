#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#include <SDL2/SDL.h>


typedef struct {
    SDL_Window *sdl_window;
    SDL_Renderer *renderer;
} Window;

Window *create_window();

void destroy_window(Window *window);