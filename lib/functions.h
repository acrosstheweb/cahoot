#ifndef FUNCTIONS
#define FUNCTIONS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"



int check(char* packetName, char* forbidden, int len);

void checkHover(Node* first, int mouseX, int mouseY);
// void addButtonToList(Node** first, Button *button); ?
void addButtonToList(Node** first, SDL_Rect iconRect, States* icon, SDL_Rect textRect, States* text);

void print(Node* first);

SDL_Texture* textureFromMessage(SDL_Renderer* renderer, char* message, int isBold);

SDL_Texture* textureFromImage(SDL_Renderer* renderer, char* imagePath);

States* setStates(SDL_Texture* normal, SDL_Texture* hover);

void display(SDL_Renderer* renderer, Button button);

#endif