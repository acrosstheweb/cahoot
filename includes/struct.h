//#warning struct
#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* renderer;
} Window;

typedef struct {
    SDL_Texture* normal;
    SDL_Texture* hover;
} States;

typedef struct {
    int isHovered;
    int isOption;
    int isClickable;
    SDL_Rect iconRect;
    States* icon;
    SDL_Rect textRect;
    States* text;
} Button;

typedef struct Node Node;
struct Node{
    Button button;
    Node* next;
};

#endif
