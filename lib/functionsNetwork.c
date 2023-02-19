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
#include "../includes/struct.h"

/**
 * Permet de lancer le serveur de jeu sur le port 13337
 *
 * @param max_clients
 * @return
 */
void* startServer(void* arg){
    Server_Args* server_args = (Server_Args*) arg;
	
	char serv_lan_ip[INET_ADDRSTRLEN];
	
	int max_clients = (server_args->max_clients) * 2;
	strcpy(serv_lan_ip, server_args->ip);
    
	int port = 13337;
	int clients[max_clients], response_read;
	char* response = malloc(sizeof(char) * 1024);
	char** responses = malloc(sizeof(char*) * max_clients);
	char* serv_welcome_msg = "Welcome to Cards server\n";

	// Verif que la création du socket est OK
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("Error creating the socket\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = inet_addr(serv_lan_ip);

	// Verif que l'attribution du port est OK
	if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0){
		printf("Failed to bind a port to the server ip\n");
		exit(EXIT_FAILURE);
	}

	// Verif que l'écoute des requêtes est OK
	if(listen(server_socket, max_clients) < 0){
		printf("Failed to listen\n");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	// ALL GOOD
	printf("The server has started and is currently listening on %s:%d\n", serv_lan_ip, port);
	int connected;
	// ACCEPTE AUTANT DE CONNEXIONS AU SERVEUR QUE max_clients
	for(int c = 0; c < max_clients; c++){
		if( (clients[c] = accept(server_socket, NULL, NULL)) < 0){
			printf("Failed to accept client n°%d\n", c);
			close(server_socket);
			exit(EXIT_FAILURE);
		} else {
			// +1 Client Connecté !
			(*(server_args->clients)) = c/2;
			if (c == 1){
				(*(server_args->clients)) = 1;
			}
		}
	}

	// On crée les threads qui éxecuteront la fonction client_handler
	printf("creation thread jeu\n");
	pthread_t threads[max_clients];
	Client_Args client_thread_args[max_clients];

	// boucle de jeu
	for (int c = 0; c < max_clients; c++) {
		if(c % 2 != 0){
			client_thread_args[c].client_id = c;
			client_thread_args[c].sock_fd = clients[c];
			client_thread_args[c].game_packet = server_args->game_packet;
			client_thread_args[c].nb_questions = server_args->nb_questions;
			client_thread_args[c].good_answers = 0;
			if (pthread_create(&threads[c], NULL, client_handler, &client_thread_args[c]) != 0) {
				perror("pthread_create failed\n");
				close(server_socket);
				exit(EXIT_FAILURE);
			}
		}
	}

	// Fin des threads
	for (int c = 0; c < max_clients; c++) {
		if(c % 2 != 0){
			printf("Thread client %d eteint\n", c);
			pthread_join(threads[c], NULL);
		}
	}

	// fin de la boucle de jeu, on regarde le score
	printf("SCOREBOARD : \n");
	for (int c = 0; c < max_clients; c++) {
		if(c % 2 != 0){
			printf(">>> Client[%d] : %d\n",client_thread_args[c].client_id, client_thread_args[c].good_answers);
		}
	}

	if (close(server_socket) == 0){
		printf("Socket serveur ferme !\n");
	} else {
		printf("Erreur fermeture socket n°%d -> %s\n", errno, strerror(errno));
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
	int nb_questions = args->nb_questions;
	QuestionData** game_packet = args->game_packet;
	int reponse;
	int q_index;
	int valread = 0;
	int serial_size = (sizeof(char) * 16) + (sizeof(char*) * 4);

    for(q_index = 0; q_index < nb_questions; q_index++) {
		char* serialized = malloc(sizeof(char) * serial_size);
		serialized = serializeQuestionData(*game_packet + q_index);

		send(sock_fd, serialized, serial_size, 0);
		recv(sock_fd, &reponse, sizeof(reponse), 0);

		valread = ntohl(reponse); // Valread = l'index de la reponse saisie par le joueur
		if(strcmp((*game_packet + q_index)->answers[valread], (*game_packet)->correct_answer) == 0){
			printf("Ah oe oe oe oeeeee, c'est gagneeeee (enfin juste +1 point quoi)\n");
			args->good_answers++;
		}else{
			printf("Ah bah t tah nul\n");
		}
		printf("client response : %d\n", valread);
    }
	
	QuestionData* quit = malloc(sizeof(QuestionData));
	quit->question = malloc(sizeof(char) * (strlen("tah_la_deconnexion") + 1));
	quit->question = "tah_la_deconnexion";
	quit->answers = malloc(sizeof(char*) * 4);
	for (int i = 0; i < 4; i++){
		quit->answers[i] = malloc(sizeof(char));
		quit->answers[i] = "";
	}
	
	char* serial_quit = malloc(sizeof(char) * serial_size);
	serial_quit = serializeQuestionData(quit);
	send(sock_fd, serial_quit, serial_size, 0);
	
	close(args->sock_fd);

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
		printf ("Error number : %d \nError message : %s \n" , errno , strerror(errno));
        return NULL;
    }

    close(sock);
    
    return lanIp;
}

// Function to serialize the QuestionData structure
char* serializeQuestionData(QuestionData* questionData) {
    int totalLength = 0;
    int i;

    // Calculate the total length of the serialized data
    totalLength += strlen(questionData->question) + 1;
    totalLength += sizeof(int);
    for (i = 0; i < 4; i++) {
        totalLength += strlen(questionData->answers[i]) + 1;
    }

    // Allocate memory for the serialized data
    char* serializedData = (char*) malloc(totalLength);

    // Copy the question into the serialized data
    strcpy(serializedData, questionData->question);
    int currentIndex = strlen(questionData->question) + 1;

    currentIndex += sizeof(int);

    // Copy the answers into the serialized data
    for (i = 0; i < 4; i++) {
        strcpy(serializedData + currentIndex, questionData->answers[i]);
        currentIndex += strlen(questionData->answers[i]) + 1;
    }

    return serializedData;
}

QuestionData* deserializeQuestionData(char* serializedData) {
    QuestionData* questionData = (QuestionData*) malloc(sizeof(QuestionData));

    // Get the question from the serialized data
    questionData->question = serializedData;

	// Get the number of answers from the serialized data
    int currentIndex = strlen(questionData->question) + 1;
    currentIndex += sizeof(int);

    // Allocate memory for the answers
    questionData->answers = (char**) malloc(4 * sizeof(char*));

    // Get the answers from the serialized data
    int i;
    for (i = 0; i < 4; i++) {
        questionData->answers[i] = serializedData + currentIndex;
        currentIndex += strlen(questionData->answers[i]) + 1;
    }

    return questionData;
}