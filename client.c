#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

int main(){

	char response[1024];
	int network_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Address assignement
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(13337);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// Check connection status
	int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	if(connection_status == -1){
		printf("Cannot connect to remote server socket\n>>> %s\n", strerror(errno));
		return 1;
	}

	// Receive data
	char quizz_response[23];
	recv(network_socket, &quizz_response, sizeof(quizz_response), 0);

	printf("The server sent the data : %s\n", quizz_response);

	printf(">>> ");
	scanf("%s", &response);

	send(network_socket, response, strlen(response), 0);

	close(network_socket);
	printf("\nGoodbye\n");

	return 0;
}
