#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 13337
#define MAX_CLIENTS 2

int main(){

	int clients[MAX_CLIENTS], response_read;
	char* response = malloc(sizeof(char) * 1024);
	char** responses = malloc(sizeof(char*) * MAX_CLIENTS);
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

	// Loop to ask questions to clients and receive their responses
	while(1){
		for(int c = 0; c < MAX_CLIENTS; c++){
			char* question = "What is your name ?\n";
			send(clients[c], question, strlen(question), 0);

			response_read = read(clients[c], response, 1024);
			responses[c] = response_read;
			printf("Client %d : %s", c, response);
		}
	}

	// Send welcome message
	// if(send(client_socket, serv_welcome_msg, sizeof(serv_welcome_msg), 0) != -1){
	//      printf("Message sent.\n");
	// }

	//close(server_socket);
	printf("\nGoodbye\n");


	return 0;

}
