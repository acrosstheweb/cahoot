// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/hosting.h"
#include "../includes/functionsDisplay.h"
#include "../includes/functions.h"

int hosting(Window* window, char* packetName) {

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



    Node* first = NULL;
    addTemplateToList(&first, window, 1, 0, 0, "");
    
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
                            if (current->button.isHovered && current->button.isClickable){
                                return current->button.isClickable;
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
        SDL_RenderCopy(window->renderer, messageTexture, NULL, &messageRect);
        SDL_RenderCopy(window->renderer, ipTexture, NULL, &ipRect);
        SDL_RenderCopy(window->renderer, packetTexture, NULL, &packetRect);

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}