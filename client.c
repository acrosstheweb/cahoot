#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 13337

void search_ips();
void* ip_handler(void* arg);

typedef struct {
    char thread_ip_addr[INET_ADDRSTRLEN];
    int thread_sock_fd;
} thread_args_t;

char** servers_list = NULL; // Tableau qui contiendra les adresses ip des serveurs trouvés sur le LAN
unsigned char nb_servers = 0; // tah l'opti de faire un unsigned char (0 à 255)
pthread_mutex_t thread_lock;

int main(){

	search_ips();

	return 0;
}

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

    printf("\n\n%d serveur(s) trouvés : \n\n", nb_servers);
    for(int j = 0; j < nb_servers; j++){
        printf("\t>>> %s\n", servers_list[j]);
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
        // Incrémenter la variable `thread_lock` de manière sécurisée avec un verrou
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