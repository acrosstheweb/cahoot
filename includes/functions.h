//#warning functions
#ifndef FUNCTIONS
#define FUNCTIONS

#include "struct.h"

int check(char* packetName, char* forbidden, int len);

char* stringFromArray(char array[]);

void initiateConfig(Conf* conf);

#endif