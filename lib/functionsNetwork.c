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

void connectToServer(char* ip){
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("Error creating the socket");
		exit(EXIT_FAILURE);
	}

    struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(13337);
	server_address.sin_addr.s_addr = inet_addr(ip);

    // CONNEXION
	if(connect(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1){
		printf("Cannot connect to remote server socket\n");
		return 1;
	}

    char serv_msg[256];
	if( recv(server_socket, &serv_msg, sizeof(serv_msg), 0) != -1){
	    printf("The server sent the data : \n\t %s\n", serv_msg);
    }

    close(server_socket);
    printf("Ciao");
}

/**
 * Liste les adresses ip qui écoutent sur le port 13337 
 * 
*/
void search_ips(){

	char ip_addr[INET_ADDRSTRLEN], tmp_ip_base[INET_ADDRSTRLEN];
	char ip_base[INET_ADDRSTRLEN] = "192.168.42."; // TODO: changer en fonction du vmnet / à entrer dans fichier de conf
    int byte, z, sockfd;
	char last_byte[4];

    pthread_t threads[255];
	thread_args_t thread_args[255];

    pthread_mutex_init(&thread_lock, NULL);

    // Boucle à travers toute les IP du LAN
    for (byte = 1; byte < 255; byte++){
    
        // Create a socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
            perror("Error creating socket");
            return;
        }

        // Set the address to be a specific IP address
		strcpy(tmp_ip_base, ip_base); // On met l'ip base dans une var tmp | tmp_ip_base = ip_base

		sprintf(last_byte, "%d", byte); // On convertit le dernier octet de l'ip en string | last_byte = (str)byte
		strcat(ip_base, last_byte); // Pour le concaténer à l'ip base (ip base est maintenant devenu l'adresse ip complète) | ip_base = ip_base + last_byte
		strcpy(ip_addr, ip_base); // On copie ip base (devenu l'adresse ip complète) dans ip_addr | ip_addr = ip_base

		strcpy(ip_base, tmp_ip_base); // On met le contenu de la var tmp dans l'ip base | ip_base = tmp_ip_base

        // On crée les threads qui éxecuteront la fonction ip_handler
        // printf("ip_addr len(%s) %d dans INET_ADDRSTRLEN : %d", ip_addr, strlen(ip_addr), INET_ADDRSTRLEN);
        // return;
        
        strcpy(thread_args[byte].thread_ip_addr, ip_addr);
		thread_args[byte].thread_sock_fd = sockfd;
		if (pthread_create(&threads[byte], NULL, ip_handler, &thread_args[byte]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
		}
    }

    pthread_mutex_destroy(&thread_lock);

    return; // stop fonction, car la fin des threads dure +2 min

    // Fin des threads
	for (byte = 1; byte < 255; byte++) {
        pthread_join(threads[byte], NULL);
        // close(thread_args[byte]->thread_sock_fd);
	}

	printf("\nGoodbye\n");

}

void* ip_handler(void* arg){
    thread_args_t* args = (thread_args_t*) arg;
    
    char ip_addr[INET_ADDRSTRLEN];
    strcpy(ip_addr, args->thread_ip_addr);

    int sock_fd = args->thread_sock_fd;

    //printf("thread ip %s\n", ip_addr);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ip_addr);

    if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0){
        // Incrémenter la variable `nb_servers` de manière sécurisée avec un verrou
        printf("\n\nServeur trouvé à l'adresse : %s:%d\n\n", ip_addr, PORT);
        pthread_mutex_lock(&thread_lock);
        servers_list = realloc(servers_list, sizeof(char*) * (nb_servers + 1));
        servers_list[nb_servers] = malloc(sizeof(char) * INET_ADDRSTRLEN);
        memcpy(servers_list[nb_servers], ip_addr, strlen(ip_addr) + 1);
        nb_servers++;
        pthread_mutex_unlock(&thread_lock);
    }else{
        close(sock_fd);
        //fflush(stdout);
    }

    return NULL;
}