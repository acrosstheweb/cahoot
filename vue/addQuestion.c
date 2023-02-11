// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/menu.h"
#include "../includes/functionsDisplay.h"
#include "../includes/functionsPacket.h"
#include "../includes/functions.h"


int addQuestion(Window* window, char* packetName) {

    // Créer les textures
    SDL_Texture* blueRectTexture = textureFromImage(window->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage(window->renderer, "img/rect_blue_hover.png");
    SDL_Texture* redRectTexture = textureFromImage(window->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage(window->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage(window->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage(window->renderer, "img/rect_green_hover.png");

    SDL_Texture* validateTexture = textureFromMessage(window->renderer, "Enregistrer", setColor("Black"), window->font);

    States* blue = setStates(blueRectTexture, blueRectHoverTexture);
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);
    States* validate = setStates(validateTexture, validateTexture);


    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect questionRect = {
        (SCREEN_WIDTH - PACKET_RECT_WIDTH) / 2,
        SCREEN_HEIGHT / 2 - PACKET_RECT_HEIGHT - 25,
        QUESTION_RECT_WIDTH,
        QUESTION_RECT_HEIGHT
    };
    SDL_Rect questionInputRect = {
        0,
        0,
        200,
        50
    };
    SDL_Rect answer1Rect = {
        questionRect.x,
        questionRect.y + PACKET_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer1InputRect = {
        0,
        0,
        100,
        25
    };
    SDL_Rect answer2Rect = {
        questionRect.x + ANSWER_RECT_WIDTH + MARGIN / 2,
        questionRect.y + PACKET_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer2InputRect = {
        0,
        0,
        100,
        25
    };
    SDL_Rect answer3Rect = {
        questionRect.x,
        answer1Rect.y + ANSWER_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer3InputRect = {
        0,
        0,
        100,
        25
    };
    SDL_Rect answer4Rect = {
        questionRect.x + ANSWER_RECT_WIDTH + MARGIN / 2,
        answer1Rect.y + ANSWER_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer4InputRect = {
        0,
        0,
        100,
        25
    };
    SDL_Rect validateRect = {
        questionRect.x,
        answer3Rect.y + ANSWER_RECT_HEIGHT + MARGIN / 2,
        QUESTION_RECT_WIDTH,
        50
    };
    SDL_Rect validateTextRect = {
        validateRect.x + (validateRect.w- getTextWidth("Enregistrer", 25)) / 2,
        validateRect.y + (validateRect.h - 25) / 2,
        getTextWidth("Enregistrer", 25),
        25
    };
    
    char questionText[MAX_LEN + 1] = "";
    char answer1Text[MAX_LEN + 1] = "";
    char answer2Text[MAX_LEN + 1] = "";
    char answer3Text[MAX_LEN + 1] = "";
    char answer4Text[MAX_LEN + 1] = "";

    TextInput inputs[5];
    inputs[0].refRect = questionRect;
    inputs[0].inputRect = questionInputRect;
    inputs[0].text = questionText;
    inputs[1].refRect = answer1Rect;
    inputs[1].inputRect = answer1InputRect;
    inputs[1].text = answer1Text;
    inputs[2].refRect = answer2Rect;
    inputs[2].inputRect = answer2InputRect;
    inputs[2].text = answer2Text;
    inputs[3].refRect = answer3Rect;
    inputs[3].inputRect = answer3InputRect;
    inputs[3].text = answer3Text;
    inputs[4].refRect = answer4Rect;
    inputs[4].inputRect = answer4InputRect;
    inputs[4].text = answer4Text;

    int activeInput = -1;

    Node* first = NULL;
    addTemplateToList(&first, window, 1, 1, 0, "");
    addButtonToList(&first, questionRect, blue, empty(), NULL, 1, 11);
    addButtonToList(&first, answer1Rect, green, empty(), NULL, 1, 12);
    addButtonToList(&first, answer2Rect, red, empty(), NULL, 1, 13);
    addButtonToList(&first, answer3Rect, red, empty(), NULL, 1, 14);
    addButtonToList(&first, answer4Rect, red, empty(), NULL, 1, 15);
    addButtonToList(&first, validateRect, green, validateTextRect, validate, 1, 16);

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
                                if (current->button.returnValue <= TO_MENU){
                                    return current->button.returnValue;
                                } else if (current->button.returnValue <= 15){
                                    activeInput = current->button.returnValue - 11;
                                }  else if (strlen(questionText) > 0 && strlen(answer1Text) > 0 && strlen(answer2Text) > 0 && strlen(answer3Text) > 0 && strlen(answer4Text) > 0) {
                                    addQuestionToFile(packetName, questionText, answer1Text, answer2Text, answer3Text, answer4Text);
                                    return ADD_QUESTION;
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
                        strlen(questionText) > 0 && strlen(answer1Text) > 0 && strlen(answer2Text) > 0 && strlen(answer3Text) > 0 && strlen(answer4Text) > 0) {
                            addQuestionToFile(packetName, questionText, answer1Text, answer2Text, answer3Text, answer4Text);
                            return ADD_QUESTION;
                        } else if (e.key.keysym.sym == SDLK_TAB && (e.key.keysym.mod & KMOD_SHIFT)) {
                            activeInput--;
                            if (activeInput == -1){
                                activeInput = 5;
                            }
                        } else if (e.key.keysym.sym == SDLK_TAB) {
                            activeInput++;
                            if (activeInput == 5){
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
                    if (activeInput >= 0 && activeInput < 5) {
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

        if (first != NULL) {
            do {
                display(window->renderer, current->button);
                current = current->next;
            }while (current != first);
        }

        for (int i = 0; i < 5; i++){
            updateInputText(window, inputs[i].text, inputs[i].refRect, inputs[i].inputRect, 0);
        }
        
        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return 1;
}