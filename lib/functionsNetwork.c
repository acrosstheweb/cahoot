#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h> // close
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // getsockname
#include <errno.h> // errno
#include <pthread.h> // pthread_create
#include "../includes/functionsNetwork.h"

/**
 * Permet de lancer le serveur de jeu sur le port 13337
 *
 * @param max_clients
 * @return
 */
void* startServer(void* arg){
    Server_Args* args = (Server_Args*) arg;
	
	char serv_lan_ip[INET_ADDRSTRLEN];
	
	int max_clients = args->max_clients;
	strcpy(serv_lan_ip, args->ip);
    
	int port = 13337;
	int clients[max_clients], response_read;
	char* response = malloc(sizeof(char) * 1024);
	char** responses = malloc(sizeof(char*) * max_clients);
	char* serv_welcome_msg = "Welcome to Cards server";

	// Verif que la création du socket est OK
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("Error creating the socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = inet_addr(serv_lan_ip);

	// Verif que l'attribution du port est OK
	if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0){
		printf("Failed to bind a port to the server ip");
		exit(EXIT_FAILURE);
	}

	// Verif que l'écoute des requêtes est OK
	if(listen(server_socket, max_clients) < 0){
		printf("Failed to listen");
		exit(EXIT_FAILURE);
	}

	// ALL GOOD
	printf("The server has started and is currently listening on %s:%d\n", serv_lan_ip, port);

	// ACCEPTE AUTANT DE CONNEXIONS AU SERVEUR QUE max_clients
	for(int c = 0; c < max_clients; c++){
		if( (clients[c] = accept(server_socket, NULL, NULL)) < 0){
			printf("Failed to accept client n°%d\n", c);
			exit(EXIT_FAILURE);
		}else{
			// +1 Client Connecté !
			printf("+1 Client connecté\n");
		}
	}

	// On crée les threads qui éxecuteront la fonction client_handler
	pthread_t threads[max_clients];
	Client_Args thread_args[max_clients];

	for (int c = 0; c < max_clients; c++) {
		thread_args[c].client_id = c;
		thread_args[c].sock_fd = clients[c];
		if (pthread_create(&threads[c], NULL, client_handler, &thread_args[c]) != 0) {
				perror("pthread_create failed");
				exit(EXIT_FAILURE);
		}
	}

	// Fin des threads
	for (int c = 0; c < max_clients; c++) {
		printf("Thread client %d eteint", c);
		pthread_join(threads[c], NULL);
	}

	if(close(server_socket) == -1){
		printf("Erreur fermeture socket n°%d -> %s", errno, strerror(errno));
	}
	printf("\nGoodbye\n");

	return NULL;
}

/**
 * Permet d'initier la boucle de jeu Envoi-Question/Reception-Reponse, grâce à un thread par joueur
 *
 * @param max_clients
 * @return
 */
void* client_handler(void* arg){
    Client_Args* args = (Client_Args*) arg;
    int client_id = args->client_id;
    int sock_fd = args->sock_fd;
	char buffer[1024] = {0};
	
    while (1) {
    	// Envoie la question au client
		char* question = "What is your name?\n";
		send(sock_fd, question, strlen(question)+1, 0);

    	// Reçoit sa réponse
    	int valread = read(sock_fd, buffer, 1024);
    	printf("Client %d: %s", client_id, buffer);
    }

	fflush(stdout);

    return NULL;
}


/**
 * Permet de récupérer l'ip de la machine qui éxécute la fonction
 * 
 * @return char* lanIp 
 */
char* getIp(){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;
	
	struct sockaddr_in serv;
    
    int sock = socket ( AF_INET, SOCK_DGRAM, 0);
    
    if(sock < 0)
    {
		perror("Socket error");
	}
    
	memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );

    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );
    
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);
    	
	char* lanIp = malloc(sizeof(char) * 64);
    const char* p = inet_ntop(AF_INET, &name.sin_addr, lanIp, 100);
    	
	if(p == NULL)
	{
		printf ("Error number : %d . Error message : %s \n" , errno , strerror(errno));
        return NULL;
    }

    close(sock);
    
    return lanIp;
}
