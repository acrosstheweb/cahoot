// TODO: remplacer option par toutes les options possibles
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* renderer;
} Window;

typedef struct Button Button;
struct Button{
    int id;
    int isHovered;
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Texture* textureHovered;
    Button* next;
};

typedef struct {
    int size;
    Button* first;
} ButtonList;

void menu(Window* window);

void checkHover(Button* b, int mouseX, int mouseY);

void addButtonToList(SDL_Rect rect, SDL_Texture* texture, SDL_Texture* textureHovered);