#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h> // close
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // getsockname
#include <errno.h> // errno
#include <pthread.h>
#include "../includes/functionsNetwork.h"
#include "../includes/functionsDisplay.h"
#include "../includes/client.h"
#include "../includes/struct.h"
#include "../includes/play.h"
#include "../includes/menu.h"

#define PORT 13337

char** servers_list = NULL; // Tableau qui contiendra les adresses ip des serveurs trouvés sur le LAN
unsigned char nb_servers = 0; // tah l'opti de faire un unsigned char (0 à 255)
pthread_mutex_t thread_lock;

int client(Window* window, char** network_ip_base){


    search_ips(*network_ip_base);
    // servers_list est alimenté

    printf("\n\n%d serveur(s) trouvés : \n\n", nb_servers);
    for(int j = 0; j < nb_servers; j++){
        printf("\t>>>[%d] %s\n", j+1, servers_list[j]);
    }

    // // TODO: Choix du serveur à laisser au client ....
    // int serv_choice; 
    // printf("A quel serveur se connecter [nombre] : \n>>> ");
    // scanf("%d", &serv_choice);

    // serv_choice--; // adapter à index d'une liste


    int page = 0;

    // Créer les textures
    SDL_Texture* nextTexture = textureFromImage(window->renderer, "img/next.png");
    SDL_Texture* nextHoverTexture = textureFromImage(window->renderer, "img/next_hover.png");
    SDL_Texture* prevTexture = textureFromImage(window->renderer, "img/prev.png");
    SDL_Texture* prevHoverTexture = textureFromImage(window->renderer, "img/prev_hover.png");
    SDL_Texture* redRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");
    SDL_Texture* blueRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    SDL_Texture* yellowRectTexture = textureFromImage(window->renderer, "img/rect_yellow.png");
    SDL_Texture* yellowRectHoverTexture = textureFromImage(window->renderer, "img/rect_yellow_hover.png");
    
    States* next = setStates(nextTexture, nextHoverTexture);
    States* prev = setStates(prevTexture, prevHoverTexture);
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);
    States* blue = setStates(blueRectTexture, blueRectHoverTexture);
    States* yellow = setStates(yellowRectTexture, yellowRectHoverTexture);

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect nextRect = {
        SCREEN_WIDTH - MARGIN - NEXT_RECT_WIDTH,
        (SCREEN_HEIGHT - NEXT_RECT_HEIGHT) / 2,
        NEXT_RECT_WIDTH,
        NEXT_RECT_HEIGHT
    };
    SDL_Rect prevRect = {
        MARGIN,
        (SCREEN_HEIGHT - PREV_RECT_HEIGHT) / 2,
        PREV_RECT_WIDTH,
        PREV_RECT_HEIGHT
    };
    SDL_Rect redRect = {
        PACKET_RECT_STARTX + MARGIN / 4,
        PACKET_RECT_STARTY + MARGIN / 2,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect greenRect = {
        PACKET_RECT_ENDX - PACKET_RECT_WIDTH - MARGIN / 4,
        PACKET_RECT_STARTY + MARGIN / 2,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect blueRect = {
        PACKET_RECT_STARTX + MARGIN / 4,
        PACKET_RECT_ENDY -PACKET_RECT_HEIGHT-MARGIN,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect yellowRect = {
        PACKET_RECT_ENDX - PACKET_RECT_WIDTH - MARGIN / 4,
        PACKET_RECT_ENDY - PACKET_RECT_HEIGHT - MARGIN,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };



    Node* first = NULL;
    addTemplateToList(&first, window, 1, 1, 0, "===SERVEURS DISPONIBLES===");
    addButtonToList(&first, prevRect, prev, empty(), NULL, 1, 11);
    addButtonToList(&first, nextRect, next, empty(), NULL, 1, 12);

    for (int i = 0; i < nb_servers; i++){
        SDL_Texture* textTexture = textureFromMessage(window->renderer, *(servers_list + i), setColor("Black"), window->font);
        States* text = setStates(textTexture, textTexture);
        switch (i % 4){
        case 0:
            addButtonToList(&first, redRect, red, (SDL_Rect){
                redRect.x + (redRect.w - getTextWidth(*(servers_list + i), 50)) / 2,
                redRect.y + (redRect.h - 50) / 2,
                getTextWidth(*(servers_list + i), 50),
                50},
                text, 1, 15 + i);
            break;

        case 1:
            addButtonToList(&first, greenRect, green, (SDL_Rect){
                greenRect.x + (greenRect.w- getTextWidth(*(servers_list + i), 50)) / 2,
                greenRect.y + (greenRect.h - 50) / 2,
                getTextWidth(*(servers_list + i), 50),
                50
                }, text, 1, 15 + i);
            break;

        case 2:
            addButtonToList(&first, blueRect, blue, (SDL_Rect){
                blueRect.x + (blueRect.w- getTextWidth(*(servers_list + i), 50)) / 2,
                blueRect.y + (blueRect.h - 50) / 2,
                getTextWidth(*(servers_list + i), 50),
                50
                }, text, 1, 15 + i);
            break;

        case 3:
            addButtonToList(&first, yellowRect, yellow, (SDL_Rect){
                yellowRect.x + (yellowRect.w- getTextWidth(*(servers_list + i), 50)) / 2,
                yellowRect.y + (yellowRect.h - 50) / 2,
                getTextWidth(*(servers_list + i), 50),
                50
                }, text, 1, 15 + i);
            break;
        
        default:
            break;
        }
    }
    
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        Node* current = first;
        int x, y;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEMOTION:
                    x = e.motion.x;
                    y = e.motion.y;
                    checkHover(first, x, y);
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    x = e.motion.x;
                    y = e.motion.y;
                    checkHover(first, x, y);
                    if (first != NULL) {
                        do {
                            if (current->button.isHovered && current->button.returnValue){
                                if (current->button.returnValue <= TO_MENU){
                                    return current->button.returnValue;
                                } else if (current->button.returnValue == 11){
                                    if (page > 0){
                                        page--;
                                    }
                                } else if (current->button.returnValue == 12){
                                    if (page < nb_servers / 4){
                                        page++;
                                    }
                                } else if (current->button.returnValue - 15 >= page * 4 && current->button.returnValue - 15 < (page+1) * 4){
                                    connectToServer(&window, *(servers_list + current->button.returnValue - 15)); // mascarade
                                    return 10;
                                }
                            }
                            current = current->next;
                        }while (current != first);
                    }
                    break;

                case SDL_QUIT:
                    return 1;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);

        if (page == 0){
        } else if (page == (nb_servers) / 4){

        }

        if (first != NULL) {
            do {
                if (current->button.returnValue <= 10){
                    display(window->renderer, current->button);
                } else if (current->button.returnValue < 15){
                    if ((current->button.icon == prev && page > 0) || (current->button.icon == next && page < nb_servers / 4 && nb_servers > 4)){
                        display(window->renderer, current->button);
                    }
                } else {
                    if (current->button.returnValue - 15 >= page * 4 && current->button.returnValue - 15 < (page+1) * 4) {
                        display(window->renderer, current->button);
                    }
                }
                current = current->next;
            }while (current != first);
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}

void connectToServer(Window** window, char* ip){
    printf("in connectToServer\n");
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
		printf("Cannot connect to remote server socket : err n°%d : %s\n", errno , strerror(errno));
		return;
	}else{
        printf("tah ?\n");
    }

    int rep;
    int counter = 0;
    int max_char = 256;
    int serial_size = (sizeof(char) * 16) + (sizeof(char*) * 4);

    char buffer[serial_size];

    while(1){
        if(recv(server_socket, &buffer, serial_size, 0) != -1){
            QuestionData* questionData = deserializeQuestionData(buffer);

            if(strcmp(questionData->question, "tah_la_deconnexion") == 0){
                printf("Ciao\n");
                close(server_socket);
                return;
            }

            rep = play(&window, questionData);
            rep = rep-11;
            rep = htonl(rep);
            send(server_socket, &rep, sizeof(rep), 0);
            rep = ntohl(rep);
        }
    }
}

/**
 * Liste les adresses ip qui écoutent sur le port 13337 
 * 
*/
void search_ips(char* network_ip_base){

	char ip_addr[INET_ADDRSTRLEN], tmp_ip_base[INET_ADDRSTRLEN];
	char ip_base[INET_ADDRSTRLEN];
    strcpy(ip_base, network_ip_base); // TODO: changer en fonction du vmnet / à entrer dans fichier de conf
    int byte, z, sockfd;
	char last_byte[4];

    pthread_t threads[255];
	Ip_Args thread_args[255];

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
    Ip_Args* args = (Ip_Args*) arg;
    
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