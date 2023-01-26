#ifndef _FUNCTIONS
#define _FUNCTIONS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

Window* create_window();

void newRenderer(Window* window);

void destroy_window(Window* window);

SDL_Rect empty();

SDL_Color setColor(char* name);
// void addButtonToList(Node** first, Button *button); ?
void addButtonToList(Node** first, SDL_Rect iconRect, States* icon, SDL_Rect textRect, States* text, int isOption, int isClickable);

void checkHover(Node* first, int mouseX, int mouseY);

int getTextWidth(char* message, int isBold, int height);

void print(Node* first);

SDL_Texture* textureFromMessage(SDL_Renderer* renderer, char* message, SDL_Color color);

SDL_Texture* textureFromImage(SDL_Renderer* renderer, char* imagePath);

States* setStates(SDL_Texture* normal, SDL_Texture* hover);

void display(SDL_Renderer* renderer, Button button);

#endif