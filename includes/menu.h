//#warning window
#ifndef WINDOW
#define WINDOW

#include "struct.h"

#define QUIT_WINDOW 1
#define TO_SETTINGS 2
#define MENU_CREATE_PACKET 3
#define MENU_MANAGE_PACKET 4
#define MENU_HOST_GAME 5
#define MENU_JOIN_GAME 6
#define ADD_QUESTION 7
#define MANAGE_QUESTION 8
#define TO_MENU 10

int menu(Window* window);
#endif