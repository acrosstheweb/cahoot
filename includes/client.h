//#warning createPacket
#ifndef _CLIENT
#define _CLIENT

#include "struct.h"

int client(Window* window, char** network_ip_base);

void connectToServer(Window** window, char* ip);

void search_ips(char* network_ip_base);

void* ip_handler(void* arg);


#endif