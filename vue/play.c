// TODO: cleanup on leave function to free all resources in array
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/functionsDisplay.h"
#include "../includes/functionsPacket.h"
#include "../includes/functions.h"
#include "../includes/play.h"


int play(Window*** window, QuestionData* questionData) {

    // Créer les textures
    SDL_Texture* blueRectTexture = textureFromImage((**window)->renderer, "img/rect_blue.png");
    SDL_Texture* blueRectHoverTexture = textureFromImage((**window)->renderer, "img/rect_blue_hover.png");
    SDL_Texture* redRectTexture = textureFromImage((**window)->renderer, "img/rect_red.png");
    SDL_Texture* redRectHoverTexture = textureFromImage((**window)->renderer, "img/rect_red_hover.png");
    SDL_Texture* greenRectTexture = textureFromImage((**window)->renderer, "img/rect_green.png");
    SDL_Texture* greenRectHoverTexture = textureFromImage((**window)->renderer, "img/rect_green_hover.png");
    SDL_Texture* yellowRectTexture = textureFromImage((**window)->renderer, "img/rect_yellow.png");
    SDL_Texture* yellowRectHoverTexture = textureFromImage((**window)->renderer, "img/rect_yellow_hover.png");

    SDL_Texture* questionTexture = textureFromMessage((**window)->renderer, questionData->question, setColor("Black"), (**window)->font);
    SDL_Texture* answer1Texture = textureFromMessage((**window)->renderer, questionData->answers[0], setColor("Black"), (**window)->font);   
    SDL_Texture* answer2Texture = textureFromMessage((**window)->renderer, questionData->answers[1], setColor("Black"), (**window)->font);
    SDL_Texture* answer3Texture = textureFromMessage((**window)->renderer, questionData->answers[2], setColor("Black"), (**window)->font);
    SDL_Texture* answer4Texture = textureFromMessage((**window)->renderer, questionData->answers[3], setColor("Black"), (**window)->font);

    States* blue = setStates(blueRectTexture, blueRectHoverTexture);
    States* red = setStates(redRectTexture, redRectHoverTexture);
    States* green = setStates(greenRectTexture, greenRectHoverTexture);
    States* yellow = setStates(yellowRectTexture, yellowRectHoverTexture);
    States* question = setStates(questionTexture, questionTexture);
    States* answer1 = setStates(answer1Texture, answer1Texture);
    States* answer2 = setStates(answer2Texture, answer2Texture);
    States* answer3 = setStates(answer3Texture, answer3Texture);
    States* answer4 = setStates(answer4Texture, answer4Texture);

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect questionRect = {
        (SCREEN_WIDTH - PACKET_RECT_WIDTH) / 2,
        SCREEN_HEIGHT / 2 - PACKET_RECT_HEIGHT - 25,
        QUESTION_RECT_WIDTH,
        QUESTION_RECT_HEIGHT
    };
    SDL_Rect questionTextRect = {
        questionRect.x + (questionRect.w - getTextWidth(questionData->question, 50) ) / 2,
        questionRect.y + (questionRect.h + 50) / 2,
        getTextWidth(questionData->question, 50),
        50
    };
    SDL_Rect answer1Rect = {
        questionRect.x,
        questionRect.y + PACKET_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer1TextRect = {
        answer1Rect.x + (answer1Rect.w - getTextWidth(questionData->answers[0], 25)) / 2,
        answer1Rect.y + (answer1Rect.h + 25) / 2,
        100,
        25
    };
    SDL_Rect answer2Rect = {
        questionRect.x + ANSWER_RECT_WIDTH + MARGIN / 2,
        questionRect.y + PACKET_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer2TextRect = {
        answer2Rect.x + (answer2Rect.w - getTextWidth(questionData->answers[1], 25)) / 2,
        answer2Rect.y + (answer2Rect.h + 25) / 2,
        100,
        25
    };
    SDL_Rect answer3Rect = {
        questionRect.x,
        answer1Rect.y + ANSWER_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer3TextRect = {
        answer3Rect.x + (answer3Rect.w - getTextWidth(questionData->answers[2], 25)) / 2,
        answer3Rect.y + (answer3Rect.h + 25) / 2,
        100,
        25
    };
    SDL_Rect answer4Rect = {
        questionRect.x + ANSWER_RECT_WIDTH + MARGIN / 2,
        answer1Rect.y + ANSWER_RECT_HEIGHT + MARGIN / 2,
        ANSWER_RECT_WIDTH,
        ANSWER_RECT_HEIGHT
    };
    SDL_Rect answer4TextRect = {
        answer4Rect.x + (answer4Rect.w - getTextWidth(questionData->answers[3], 50)) / 2,
        answer4Rect.y + (answer4Rect.h + 25) / 2,
        100,
        25
    };

    Node* first = NULL;
    addTemplateToList(&first, (**window), 1, 0, 0, "");
    addButtonToList(&first, questionRect, yellow, questionTextRect, question, 0, 0); // Question
    addButtonToList(&first, answer1Rect, red, answer1TextRect, answer1, 1, 11); 
    addButtonToList(&first, answer2Rect, green, answer2TextRect, answer2, 1, 12);
    addButtonToList(&first, answer3Rect, blue, answer3TextRect, answer3, 1, 13);
    addButtonToList(&first, answer4Rect, yellow, answer4TextRect, answer4, 1, 14);

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
                                if (current->button.returnValue){
                                    return current->button.returnValue;
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
        SDL_free(clipboard);
        SDL_SetRenderDrawColor((**window)->renderer, 0xF1, 0xFA, 0xEE, 0xFF);
        SDL_RenderClear((**window)->renderer);

        if (first != NULL) {
            do {
                display((**window)->renderer, current->button);
                current = current->next;
            }while (current != first);
        }
        
        // Mettre à jour l'affichage
        SDL_RenderPresent((**window)->renderer);
    }
    return 1;
}