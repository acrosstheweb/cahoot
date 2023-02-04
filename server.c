#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "includes/functions.h"

#define MAX_CLIENTS 20

typedef struct {
    int client_id;
    int sock_fd;
} thread_args_t;

void* client_handler(void* arg);

int main(){
	int port = 13337;
	int clients[MAX_CLIENTS], response_read;
	char* serv_lan_ip = getIp();
	char* response = malloc(sizeof(char) * 1024);
	char** responses = malloc(sizeof(char*) * MAX_CLIENTS);
	char* serv_welcome_msg = "Welcome to Cards server";

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("Error creating the socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = inet_addr(serv_lan_ip);

	if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0){
		printf("Failed to bind a port to the server ip");
		exit(EXIT_FAILURE);
	}

	if(listen(server_socket, 3) < 0){
		printf("Faied to listen");
		exit(EXIT_FAILURE);
	}

	printf("The server has started and is currently listening on %s:%d\n", serv_lan_ip, port);

	for(int c = 0; c < MAX_CLIENTS; c++){
		if( (clients[c] = accept(server_socket, NULL, NULL)) < 0){
			printf("Failed to accept client n°%d\n", c);
			exit(EXIT_FAILURE);
		}
	}

	// On crée les threads qui éxecuteront la fonction client_handler
	pthread_t threads[MAX_CLIENTS];
	thread_args_t thread_args[MAX_CLIENTS];

	for (int c = 0; c < MAX_CLIENTS; c++) {
		thread_args[c].client_id = c;
		thread_args[c].sock_fd = clients[c];
		if (pthread_create(&threads[c], NULL, client_handler, &thread_args[c]) != 0) {
				perror("pthread_create failed");
				exit(EXIT_FAILURE);
		}
	}

	// Fin des threads
	for (int c = 0; c < MAX_CLIENTS; c++) {
		pthread_join(threads[c], NULL);
	}


	printf("\nGoodbye\n");


	return 0;

}

void* client_handler(void* arg){
    thread_args_t* args = (thread_args_t*) arg;
    int client_id = args->client_id;
    int sock_fd = args->sock_fd;
    char buffer[1024] = {0};

    while (1) {
    	// Envoie la question au client
    	char* question = "What is your name?\n";
    	send(sock_fd, question, strlen(question), 0);

    	// Reçoit sa réponse
    	int valread = read(sock_fd, buffer, 1024);
    	printf("Client %d: %s", client_id, buffer);
    }

    return NULL;
}