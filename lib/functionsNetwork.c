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
    Server_Args* args = (Server_Args*) arg;
	
	char serv_lan_ip[INET_ADDRSTRLEN];
	
	int max_clients = args->max_clients;
	strcpy(serv_lan_ip, args->ip);
    
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

	// ACCEPTE AUTANT DE CONNEXIONS AU SERVEUR QUE max_clients
	for(int c = 0; c < max_clients; c++){
		if( (clients[c] = accept(server_socket, NULL, NULL)) < 0){
			printf("Failed to accept client n°%d\n", c);
			close(server_socket);
			exit(EXIT_FAILURE);
		}else{
			// +1 Client Connecté !
			printf("+1 Client connecté\n");
		}
	}

	// On crée les threads qui éxecuteront la fonction client_handler
	printf("creation thread jeu\n");
	pthread_t threads[max_clients];
	Client_Args thread_args[max_clients];

	for (int c = 0; c < max_clients; c++) {
		if(c % 2 != 0){
			thread_args[c].client_id = c;
			thread_args[c].sock_fd = clients[c];
			thread_args[c].game_packet = args->game_packet;
			thread_args[c].nb_questions = args->nb_questions;
			if (pthread_create(&threads[c], NULL, client_handler, &thread_args[c]) != 0) {
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
	int valread = 0;

    for(int q_index = 0; q_index < nb_questions; q_index++) {
    	// printf("jaaj\n");
		// send(sock_fd, (*game_packet + q_index)->question, strlen((*game_packet + q_index)->question) + 1, 0); // Envoie la question au client
		// printf("question [%d] sent\n", q_index);
		// send(sock_fd, (*game_packet + q_index)->answers[0], strlen((*game_packet + q_index)->answers[0]) + 1, 0); // Envoie la premiere reponse
		// send(sock_fd, (*game_packet + q_index)->answers[1], strlen((*game_packet + q_index)->answers[1]) + 1, 0);
		// send(sock_fd, (*game_packet + q_index)->answers[2], strlen((*game_packet + q_index)->answers[2]) + 1, 0);
		// send(sock_fd, (*game_packet + q_index)->answers[3], strlen((*game_packet + q_index)->answers[3]) + 1, 0);
		
		int serial_size = (sizeof(char) * 16) + (sizeof(char*) * 4);
		char* serialized = malloc(sizeof(char) * serial_size);
		serialized = serializeQuestionData(game_packet[q_index]);
		printf("*(game_packet + q_index)->question : %s\n", game_packet[q_index]->question);

		send(sock_fd, serialized, serial_size, 0);
    	
		recv(sock_fd, &reponse, sizeof(reponse), 0);

		valread = ntohl(reponse); // Valread = l'index de la reponse saisie par le joueur
		printf("client response : %d\n", valread);
    }
	char quit_signal[18] = "tah_la_deconnexion";
	send(sock_fd, quit_signal, strlen(quit_signal)+1, 0);
	
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
		printf ("Error number : %d . Error message : %s \n" , errno , strerror(errno));
        return NULL;
    }

    close(sock);
    
    return lanIp;
}

// Function to serialize the QuestionData structure
char* serializeQuestionData(QuestionData* questionData) {
	printf("in serialize : %s\n", questionData->question);
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