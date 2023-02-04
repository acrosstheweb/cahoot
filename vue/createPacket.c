// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/functionsDisplay.h"
#include "../includes/functionsPacket.h"
#include "../includes/functions.h"


int createPacket(Window* window, char** packetName) {
    int colorNum = rand() % 4;

    char* subtitleText = "15 caracteres max.";
    SDL_Texture* subtitleTexture = textureFromMessage(window->renderer, subtitleText, setColor("Black"), window->font);
    States* subtitle = setStates(subtitleTexture, subtitleTexture);

    // Créer les textures
    SDL_Texture* colorRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    
    if (colorNum == 0){
        colorRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
        colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    } else if (colorNum == 1){
        colorRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
        colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");
    } else if (colorNum == 2){
        colorRectTexture = textureFromImage(window->renderer, "img/rect_yellow.png");
        colorRectHoverTexture = textureFromImage(window->renderer, "img/rect_yellow_hover.png");
    }
    States* color = setStates(colorRectTexture, colorRectHoverTexture);


    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect subtitleRect = {
        (SCREEN_WIDTH - getTextWidth(subtitleText, 25)) / 2,
        150,
        getTextWidth(subtitleText, 25),
        25
    };
    SDL_Rect colorRect = {
        (SCREEN_WIDTH - PACKET_RECT_WIDTH) / 2,
        (SCREEN_HEIGHT - PACKET_RECT_HEIGHT) / 2,
        PACKET_RECT_WIDTH,
        PACKET_RECT_HEIGHT
    };
    SDL_Rect inputRect = {
        SCREEN_WIDTH / 2,
        SCREEN_HEIGHT / 2,
        200,
        50
    };
    
    char inputText[MAX_LEN + 1] = "";

    Node* first = NULL;
    addTemplateToList(&first, window, 1, 1, 1, "Comment voulez-vous appeler votre paquet?");
    addButtonToList(&first, subtitleRect, subtitle, empty(), NULL, 0, 0);
    addButtonToList(&first, colorRect, color, empty(), NULL, 0, 0);

    SDL_StartTextInput();
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        Node* current = first;
        int x, y, textAdded = 0;
        char* clipboard = malloc(sizeof(char)*672);
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
                
                case SDL_KEYDOWN:
                    clipboard = NULL;
                    if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) { // Backspace
                        *(inputText + strlen(inputText) - 1) = '\0';
                    } else if (e.key.keysym.sym == SDLK_c && (e.key.keysym.mod & KMOD_CTRL)) { // Copy
                        if (SDL_SetClipboardText(inputText) != 0){
                            printf("Erreur de SDL_SetClipboardText(): %s\n", SDL_GetError());
                        }
                    } else if (e.key.keysym.sym == SDLK_v && (e.key.keysym.mod & KMOD_CTRL)) { // Paste
                        textAdded = 1;
                        clipboard = SDL_GetClipboardText();
                        if (!SDL_HasClipboardText()){
                            printf("Erreur de SDL_GetClipboardText(): %s\n", SDL_GetError());
                        } else {
                            if (strlen(inputText) + strlen(clipboard) <= MAX_LEN){
                                strcat(inputText, clipboard);
                            } else {
                                memcpy(inputText + strlen(inputText), clipboard, MAX_LEN - strlen(inputText));
                            }
                        }
                    } else if ((e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) && strlen(inputText) > 0) {
                        *packetName = malloc(sizeof(char) * (strlen(inputText) + 1));
                        memcpy(*packetName, inputText, strlen(inputText) + 1);
                        
                        return 7;
                    }
                    break;

                case SDL_TEXTINPUT:
                    if (strlen(inputText) == 0){

                        strcpy(inputText, e.text.text);
                    } else {
                        strcat(inputText, e.text.text);
                    }
                    textAdded = 1;
                    break;

                case SDL_QUIT:
                    return 1;
            }
        }

        // Effacer l'écran
        SDL_free(clipboard);
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);

        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }

        // if (textAdded){
            SDL_Texture *texture = textureFromMessage(window->renderer, inputText, setColor("Black"), window->font);
            inputRect.w = getTextWidth(inputText, 50);
            inputRect.x = (SCREEN_WIDTH - inputRect.w) / 2;
            inputRect.y = colorRect.y * 1.5 - inputRect.h;
            SDL_RenderCopy(window->renderer, texture, NULL, &inputRect);
        // }
        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}