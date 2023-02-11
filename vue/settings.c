// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/functionsDisplay.h"
#include "../includes/functionsPacket.h"
#include "../includes/functions.h"


int settings(Window* window, Conf** conf) {

    // Créer les textures
    SDL_Texture* blueRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    SDL_Texture* redRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");

    SDL_Texture* validateTexture = textureFromMessage(window->renderer, "Enregistrer", setColor("Black"), window->font);
    SDL_Texture* pathTexture = textureFromMessage(window->renderer, "Packets path : ", setColor("Black"), window->font);
    SDL_Texture* ipTexture = textureFromMessage(window->renderer, "IP base : ", setColor("Black"), window->font);
    SDL_Texture* maxConnectionsTexture = textureFromMessage(window->renderer, "Maximum connections when hosting : ", setColor("Black"), window->font);

    States* blue = setStates(blueRectTexture, blueRectHoverTexture);
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);
    States* validate = setStates(validateTexture, validateTexture);


    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect pathRect = {
        175,
        200,
        getTextWidth("Packets path : ", 50),
        50
    };
    SDL_Rect pathInputRect = {
        pathRect.x + pathRect.w + MARGIN,
        200,
        100,
        50
    };
    SDL_Rect ipRect = {
        175,
        275,
        getTextWidth("IP base : ", 50),
        50
    };
    SDL_Rect ipInputRect = {
        ipRect.x + ipRect.w + MARGIN,
        275,
        100,
        50
    };
    SDL_Rect maxConnectionsRect = {
        175,
        350,
        getTextWidth("Maximum connections when hosting : ", 50),
        50
    };
    SDL_Rect maxConnectionsInputRect = {
        maxConnectionsRect.x + maxConnectionsRect.w + MARGIN,
        350,
        100,
        50
    };
    SDL_Rect validateRect = {
        (SCREEN_WIDTH - 300) / 2,
        SCREEN_HEIGHT - 200,
        300,
        50
    };
    SDL_Rect validateTextRect = {
        validateRect.x + (validateRect.w- getTextWidth("Enregistrer", 25)) / 2,
        validateRect.y + (validateRect.h - 25) / 2,
        getTextWidth("Enregistrer", 25),
        25
    };

    TextInput inputs[3];
    inputs[0].refRect = pathRect;
    inputs[0].inputRect = pathInputRect;
    inputs[0].text = (*conf)->packetPath;
    inputs[1].refRect = ipRect;
    inputs[1].inputRect = ipInputRect;
    inputs[1].text = (*conf)->ip_base;
    inputs[2].refRect = maxConnectionsRect;
    inputs[2].inputRect = maxConnectionsInputRect;
    inputs[2].text = (*conf)->maxConnections;

    int activeInput = -1;

    Node* first = NULL;
    addTemplateToList(&first, window, 1, 1, 0, "");
    addButtonToList(&first, pathRect, NULL, empty(), NULL, 1, 11);
    addButtonToList(&first, ipRect, NULL, empty(), NULL, 1, 12);
    addButtonToList(&first, maxConnectionsRect, NULL, empty(), NULL, 1, 13);
    addButtonToList(&first, validateRect, green, validateTextRect, validate, 1, 15);

    SDL_StartTextInput();
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        Node* current = first;
        int x, y;
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
                            if (current->button.isHovered && current->button.returnValue){
                                if (current->button.returnValue <= 10){
                                    return current->button.returnValue;
                                } else if (current->button.returnValue < 15){
                                    activeInput = current->button.returnValue - 11;
                                }  else if (strlen((*conf)->ip_base) > 0 && strlen((*conf)->packetPath) > 0 && strlen((*conf)->maxConnections) > 0) {
                                    if (inputs[0].text[strlen(inputs[0].text) - 1] != '/'){
                                        inputs[activeInput].text[strlen(inputs[activeInput].text) - 1] = '\0';
                                        strcat(inputs[0].text, "/\0");
                                    }
                                    modifyConfig(*conf);
                                    return 10;
                                }
                            }
                            current = current->next;
                        }while (current != first);
                    }
                    break;
                
                case SDL_KEYDOWN:
                    if (activeInput >= 0 && activeInput < 5) {
                        clipboard = NULL;
                        if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputs[activeInput].text) > 0) { // Backspace
                            *(inputs[activeInput].text + strlen(inputs[activeInput].text) - 1) = '\0';
                        } else if (e.key.keysym.sym == SDLK_c && (e.key.keysym.mod & KMOD_CTRL)) { // Copy
                            if (SDL_SetClipboardText(inputs[activeInput].text) != 0){
                                printf("Erreur de SDL_SetClipboardText(): %s\n", SDL_GetError());
                            }
                        } else if (e.key.keysym.sym == SDLK_v && (e.key.keysym.mod & KMOD_CTRL)) { // Paste
                            clipboard = SDL_GetClipboardText();
                            if (!SDL_HasClipboardText()){
                                printf("Erreur de SDL_GetClipboardText(): %s\n", SDL_GetError());
                            } else {
                                if (strlen(inputs[activeInput].text) + strlen(clipboard) <= MAX_LEN){
                                    strcat(inputs[activeInput].text, clipboard);
                                } else {
                                    memcpy(inputs[activeInput].text + strlen(inputs[activeInput].text), clipboard, MAX_LEN - strlen(inputs[activeInput].text));
                                }
                            }
                        } else if ((e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) &&
                        strlen((*conf)->ip_base) > 0 && strlen((*conf)->packetPath) > 0 && strlen((*conf)->maxConnections) > 0) {
                            if (inputs[0].text[strlen(inputs[0].text) - 1] != '/'){
                                inputs[activeInput].text[strlen(inputs[activeInput].text) - 1] = '\0';
                                strcat(inputs[0].text, "/\0");
                            }
                            modifyConfig(*conf);
                            return 10;
                        } else if (e.key.keysym.sym == SDLK_TAB && (e.key.keysym.mod & KMOD_SHIFT)) {
                            activeInput--;
                            if (activeInput == -1){
                                activeInput = 5;
                            }
                        } else if (e.key.keysym.sym == SDLK_TAB) {
                            activeInput++;
                            if (activeInput == 3){
                                activeInput = 0;
                            }
                        }
                    } else if (activeInput == -1){
                        if (e.key.keysym.sym == SDLK_TAB) {
                            activeInput = 0;
                        }
                    }
                    break;

                case SDL_TEXTINPUT:
                    if ((activeInput == 2 && (*(e.text.text) >= '0' && *(e.text.text) <= '9')) || 
                    (activeInput == 1 && ((*(e.text.text) >= '0' && *(e.text.text) <= '9') || *(e.text.text) == '.')) || 
                    (activeInput == 0 && strlen(inputs[0].text) < 20 && ((*(e.text.text) >= '0' && *(e.text.text) <= '9') || (*(e.text.text) >= 'a' && *(e.text.text) <= 'z') || (*(e.text.text) >= 'A' && *(e.text.text) <= 'Z') || *(e.text.text) == '-' || *(e.text.text) == '_' || *(e.text.text) == '.' || *(e.text.text) == '/'))){
                        if (activeInput == 0){
                            inputs[activeInput].text[strlen(inputs[activeInput].text)] = '\0';
                        }
                        if (strlen(inputs[activeInput].text) == 0){
                            strcpy(inputs[activeInput].text, e.text.text);
                        } else {
                            strcat(inputs[activeInput].text, e.text.text);
                        }
                    }
                    break;

                case SDL_QUIT:
                    return 1;
            }
        }

        // Effacer l'écran
        SDL_free(clipboard);
        SDL_SetRenderDrawColor(window->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear(window->renderer);

        SDL_RenderCopy(window->renderer, pathTexture, NULL, &pathRect);
        SDL_RenderCopy(window->renderer, ipTexture, NULL, &ipRect);
        SDL_RenderCopy(window->renderer, maxConnectionsTexture, NULL, &maxConnectionsRect);

        for (int i = 0; i < 3; i++){
            updateInputText(window, inputs[i].text, inputs[i].refRect, inputs[i].inputRect, 1);
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