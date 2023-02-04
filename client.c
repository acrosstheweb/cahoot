#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 13337
#define ETIMEDOUT 20

typedef struct {
    char ip_addr[INET_ADDRSTRLEN];
    int sock_fd;
} thread_args_t;

void search_ips();
void* ip_handler(void* arg);

int main(){

	search_ips();

	return 0;
}

void search_ips(){

	char tmp_ip_base[INET_ADDRSTRLEN];
	char ip_base[INET_ADDRSTRLEN] = "192.168.42."; // TODO: changer en fonction du vmnet / à entrer dans fichier de conf
    char ip_addr[INET_ADDRSTRLEN];
    int byte, z, sockfd;
	char last_byte[3];

    pthread_t threads[255];
	thread_args_t thread_args[255];

    // Boucle à travers toute les IP du LAN
    for (byte = 1; byte < 255; byte++){
    
        // printf("%d ", byte);  
        // if(byte%15 == 0){
        //     printf("\n\n");
        // }


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

        strcpy(thread_args[byte].ip_addr, ip_addr);
		thread_args[byte].sock_fd = sockfd;
		if (pthread_create(&threads[byte], NULL, ip_handler, &thread_args[byte]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
		}
    

    }

    // Fin des threads
	for (byte = 1; byte < 255; byte++) {
		pthread_join(threads[byte], NULL);
        close(thread_args[byte].sock_fd);
	}

	printf("\nGoodbye\n");

}

void* ip_handler(void* arg){
    thread_args_t* args = (thread_args_t*) arg;
    char ip_addr[INET_ADDRSTRLEN];
    strcpy(ip_addr, args->ip_addr);
    int sock_fd = args->sock_fd;
    char buffer[1024] = {0};
    struct sockaddr_in serv_addr;
    printf("thread ip %s\n", ip_addr);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ip_addr);

    if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0){
        int len = sizeof(serv_addr);
        int z = getsockname(sock_fd, (struct sockaddr *) &serv_addr, &len);
        if(z == -1){
            printf("getsockname failed\n");
            exit(EXIT_FAILURE);
        }
        printf("\n\n\n\nServeur trouvé à l'adresse : %s:%d\n\n\n\n", ip_addr, PORT);

    }else{
        fflush(stdout);
    }

    return NULL;
}