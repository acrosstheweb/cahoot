// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/functionsDisplay.h"
#include "../includes/functions.h"

#define MAX_LEN 30


int createPacket(Window* window) {
    int colorNum = rand() % 4;

    TTF_Init();
    if (TTF_Init() != 0)
    {
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

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
    int inputLen = 0;
    char *composition;

    Node* first = NULL;
    addTemplateToList(&first, window->renderer, 1, 1, 1, "Comment voulez-vous appeler votre paquet? 30 caracteres max.");
    addButtonToList(&first, colorRect, color, empty(), NULL, 0, 0);

    SDL_StartTextInput();
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        Node* current = first;
        int x, y, j;
        SDL_TextInputEvent textEvent;
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
                    if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) { // Backspace
                        inputText[strlen(inputText) - 1] = '\0';
                        printf("backspace\n");
                    }
                //         inputText[inputLen - 1] = '\0';
                //         inputLen--;
                //     } else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) { // Copy
                //         char* temp = NULL;
                //         memcpy(temp, inputText, inputLen + 1);
                //         SDL_SetClipboardText(temp);
                //         free(temp);
                //     } else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { // Paste
                //         char* temp = SDL_GetClipboardText();
                //         j  = 0;
                //         if (strlen(temp) > MAX_LEN - inputLen){
                //             for (int i = inputLen; i < MAX_LEN; i++){
                //                 inputText[i] = temp[j];
                //                 j++;
                //                 inputLen--;
                //             }
                //         } else {
                //             for (int i = 0; i <= strlen(temp); i++){
                //                 inputText[inputLen + i] = temp[j];
                //                 j++;
                //                 inputLen--;
                //             }
                //         }
                //     }
                //     getLastElement(&first)->button.text->normal = textureFromMessage(window->renderer, getStringFromArray(inputText, inputLen), setColor("Black"));
                    break;

                case SDL_TEXTINPUT:
                    textEvent = e.text;
                    strcat(inputText, textEvent.text);
                    j = 1;
                    // j = 0;
                    // for (int i = 0; i < strlen(e.text.text); i++){
                    //     if (inputLen < MAX_LEN){
                    //         inputText[inputLen + i] = e.text.text[j];
                    //         j++;
                    //         inputLen++;
                    //     }
                    // }
                    break;

                case SDL_QUIT:
                    return 1;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);
        if (j){
            SDL_Texture *texture = textureFromMessage(window->renderer, inputText, setColor("Black"));
            inputRect.w = getTextWidth(inputText, 0, 50);
            inputRect.x = (SCREEN_WIDTH - inputRect.w) / 2;
            SDL_RenderCopy(window->renderer, texture, NULL, &inputRect);
        }

        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}