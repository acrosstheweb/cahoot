#ifndef _FUNCTIONS
#define _FUNCTIONS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define MARGIN 24

#define SELECT_WIDTH 25
#define SELECT_HEIGHT 25
#define LEAVE_WIDTH 50
#define LEAVE_HEIGHT 50
#define SETTINGS_WIDTH 50
#define SETTINGS_HEIGHT 50
#define MENU_WIDTH 50
#define MENU_HEIGHT 25
#define PACKET_RECT_WIDTH 472
#define PACKET_RECT_HEIGHT 224
#define PACKET_RECT_STARTX 155
#define PACKET_RECT_ENDX 1125
#define PACKET_RECT_STARTY 150
#define PACKET_RECT_ENDY 646
#define QUESTION_RECT_WIDTH 472
#define QUESTION_RECT_HEIGHT 224
#define ANsWER_RECT_WIDTH 224
#define ANsWER_RECT_HEIGHT 100

Window* create_window();

void newRenderer(Window* window);

void destroy_window(Window* window);

SDL_Rect empty();

SDL_Color setColor(char* name);
// void addButtonToList(Node** first, Button *button); ?
void addButtonToList(Node** first, SDL_Rect iconRect, States* icon, SDL_Rect textRect, States* text, int isOption, int isClickable);

void checkHover(Node* first, int mouseX, int mouseY);

TTF_Font* initTTF();

int getTextWidth(char* message, int height);
// int getTextWidth(char* message, int isBold, int height);

void print(Node* first);

SDL_Texture* textureFromMessage(SDL_Renderer* renderer, char* message, SDL_Color color, TTF_Font* font);

SDL_Texture* textureFromImage(SDL_Renderer* renderer, char* imagePath);

States* setStates(SDL_Texture* normal, SDL_Texture* hover);

void display(SDL_Renderer* renderer, Button button);

void addTemplateToList(Node** first, SDL_Renderer* renderer, int displayLogo, int displayBackToMenu, int displaySettings, char* titleText, TTF_Font* font);

#endif