//#warning struct
#ifndef _STRUCT
#define _STRUCT

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_LEN 15

typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* renderer;
    TTF_Font* font;
} Window;

typedef struct {
    SDL_Texture* normal;
    SDL_Texture* hover;
} States;

typedef struct {
  SDL_Rect refRect;
  SDL_Rect inputRect;
  char *text;
} TextInput;

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
