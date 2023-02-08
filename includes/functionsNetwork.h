#ifndef _NETWORK
#define _NETWORK

#include "struct.h" // QuestionData*

typedef struct {
    int client_id;
    int sock_fd;
    QuestionData* game_packet; // packet
    int nb_questions;
} Client_Args;

typedef struct {
	int max_clients;
	char ip[16]; // INET_ADDRSTRLEN

} Server_Args;

void* client_handler(void* arg);

void* startServer(void* arg);

char* getIp();

#endif