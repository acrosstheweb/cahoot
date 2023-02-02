//#warning functions
#ifndef FUNCTIONS
#define FUNCTIONS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

int check(char* packetName, char* forbidden, int len);

char* stringFromArray(char array[]);

#endif