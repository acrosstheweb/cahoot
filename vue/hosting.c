// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/hosting.h"
#include "../includes/functionsDisplay.h"
#include "../includes/functions.h"
#include "../includes/functionsNetwork.h"
#include <pthread.h> // pthread_create
#include "../includes/functionsPacket.h"


#define MAX_CLIENTS 2

int hosting(Window* window, char* packetName, QuestionData* packet_to_play, int questionsNb) {

    // Créer les textures
    char* ip = getIp();
    char* packet = malloc(sizeof(char) * (strlen("avec le paquet ") + strlen(packetName) + 1));
    packet = strcpy(packet, "avec le paquet ");
    packet = strcat(packet, packetName);
    SDL_Texture* messageTexture = textureFromMessage(window->renderer, "Vous hebergez une partie sur l'adresse suivante:", setColor("Black"), window->font);
    SDL_Texture* ipTexture = textureFromMessage(window->renderer, ip , setColor("Black"), window->font);
    SDL_Texture* packetTexture = textureFromMessage(window->renderer, packet , setColor("Black"), window->font);

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect messageRect = {
        (SCREEN_WIDTH - getTextWidth("Vous hebergez une partie sur l'adresse suivante:", 50)) / 2,
        200,
        getTextWidth("Vous hebergez une partie sur l'adresse suivante:", 50),
        50
    };
    SDL_Rect ipRect = {
        (SCREEN_WIDTH - getTextWidth(ip, 50)) / 2,
        275,
        getTextWidth(ip, 50),
        50
    };
    SDL_Rect packetRect = {
        (SCREEN_WIDTH - getTextWidth(packet, 50)) / 2,
        350,
        getTextWidth(packet, 50),
        50
    };

    pthread_t threadServer;
	Server_Args server_args;
    // QuestionData* questionData = NULL;
    // QuestionData** packet_to_play = malloc(sizeof(QuestionData*));
    // packet_to_play = &questionData;
    // printf("sizeof packet_to_play before : %d\n", sizeof(packet_to_play));
    // int* questionNb = malloc(sizeof(int));
    // readPacket(packetName, &packet_to_play, &questionsNb);
    // printf("nb_packets after : %d\n", *questionsNb);
    // printf("sizeof packet_to_play after : %d\n", sizeof(packet_to_play));

    for (int i = 0; i < questionsNb; i++){
		printf("Question %d : %s\n", i, packet_to_play[i].question);
	}

    server_args.max_clients = MAX_CLIENTS;
    strcpy(server_args.ip, ip);
    server_args.game_packet = &packet_to_play;
    server_args.nb_questions = questionsNb;

    if (pthread_create(&threadServer, NULL, startServer, &server_args) != 0) {
        perror("pthread_create failed");
        exit(EXIT_FAILURE);
    }

    Node* first = NULL;
    addTemplateToList(&first, window, 1, 1, 0, "");
    
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
                                if (current->button.returnValue == 1 || current->button.returnValue == 10){
                                    // Fin du thread
                                    if (pthread_join(threadServer, NULL) == 0){
                                        printf("Thread Serveur éteint\n");
                                    }
                                }
                                return current->button.returnValue;
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

        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }
        
        
        SDL_RenderCopy(window->renderer, messageTexture, NULL, &messageRect); // Affichage de "Vous hebergez...""
        SDL_RenderCopy(window->renderer, ipTexture, NULL, &ipRect); // Affichage de l'ip
        SDL_RenderCopy(window->renderer, packetTexture, NULL, &packetRect); // Affichage du packet séléctionné


        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }

    return 1;
}