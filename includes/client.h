//#warning createPacket
#ifndef _CLIENT
#define _CLIENT

#include "struct.h"

int client(Window* window);

void connectToServer(char* ip);

void search_ips();

void* ip_handler(void* arg);


#endif