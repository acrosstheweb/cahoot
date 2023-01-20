#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 13337
#define MAX_CLIENTS 2

typedef struct {
    int client_id;
    int client_sock_fd;
} thread_args_t;

void* client_handler(void* arg) {
	thread_args_t* args = (thread_args_t*) arg;
	int client_id = args->client_id;
	int client_sock_fd = args->client_sock_fd;
	char response[256] = {0};

	// Send the question to the client
	char* question = "What is your name ?\n";
	send(client_sock_fd, question, strlen(question)+1, 0);

	// Receive the response from the client
	recv(client_sock_fd, response, strlen(response)+1, 0);
	printf("Client %d: %s\n", client_id, response);

	return NULL;
}

int main(){

	int clients[MAX_CLIENTS], response_read;
	char* serv_welcome_msg = "Welcome to Cards server";

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("Error creating the socket");
		exit(EXIT_FAILURE);
	}

	// Define the serv address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to a specified IP and Port
	if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0){
		printf("Failed to bind a port to the server ip");
		exit(EXIT_FAILURE);
	}

	// Configure the maximum pendig requests that can be accepted and listen to connection requests
	if(listen(server_socket, 3) < 0){
		printf("Faied to listen");
		exit(EXIT_FAILURE);
	}

	printf("The server has started and is currently listening\n");

	for(int c = 0; c < MAX_CLIENTS; c++){
		if( (clients[c] = accept(server_socket, NULL, NULL)) < 0){
			printf("Failed to accept client n°%d\n", c);
			exit(EXIT_FAILURE);
		}
	}

	// Create threads to handle communication with each client
    	pthread_t threads[MAX_CLIENTS];
    	thread_args_t thread_args[MAX_CLIENTS];

    	for (int c = 0; c < MAX_CLIENTS; c++) {
        	thread_args[c].client_id = c;
        	thread_args[c].client_sock_fd = clients[c];
        	if (pthread_create(&threads[c], NULL, client_handler, &thread_args[c]) != 0) {
            		perror("pthread_create failed");
            		exit(EXIT_FAILURE);
        	}
    	}

    	// Wait for the threads to finish
    	for (int c = 0; c < MAX_CLIENTS; c++) {
        	pthread_join(threads[c], NULL);
    	}


	printf("\nGoodbye\n");


	return 0;

}
