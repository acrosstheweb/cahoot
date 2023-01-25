#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "includes/struct.h"
#include "includes/functions.h"

SDL_Color Black = {27, 27, 27};
SDL_Color Red = {208, 19, 23};
SDL_Color Green = {2, 167, 125};
SDL_Color Blue = {55, 101, 174};

int check(char* packetName, char* forbidden, int len) {

    for (int j = 0; j < len; j++){
        if (strchr(packetName, forbidden[j])){
            return 1;
        }
    }
    return 0;
}

void addButtonToList(Node** first, SDL_Rect iconRect, States* icon, SDL_Rect textRect, States* text){
    Node* node = malloc(sizeof(Node));
    node->button.isHovered = 0;
    node->button.iconRect= iconRect;
    node->button.icon = icon;
    node->button.textRect = textRect;
    node->button.text = text;
    node->next = *first;
 
    if (*first != NULL) {
        Node* temp = *first;
        while (temp->next != *first)
            temp = temp->next;
        temp->next = node;
    }
    else
        node->next = node;
 
    *first = node;
}

void checkHover(Node* first, int mouseX, int mouseY){
    Node* current = first;
    if (first != NULL) {
        do {
            if (( mouseX >= current->button.iconRect.x && mouseX <= current->button.iconRect.x + current->button.iconRect.w &&
                mouseY >= current->button.iconRect.y && mouseY <= current->button.iconRect.y + current->button.iconRect.h) ||
                ( mouseX >= current->button.textRect.x && mouseX <= current->button.textRect.x + current->button.textRect.w &&
                mouseY >= current->button.textRect.y && mouseY <= current->button.textRect.y + current->button.textRect.h))
            {
                current->button.isHovered = 1;
            } else {
                current->button.isHovered = 0;
            }
            current = current->next;
        } while (current != first);
    }
}

void print(Node* first) {
    Node *temp = first;
    if (first != NULL) {
        do {
        printf("current: %p\ncurrent->next: %p\n-----\n", temp, temp->next);
            temp = temp->next;
        }while (temp != first);
    }
}

SDL_Texture* textureFromMessage(SDL_Renderer* renderer, char* message, int isBold){
    if (TTF_Init() != 0){
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    TTF_Font* Montserrat = NULL;
    
    Montserrat = (isBold) ? TTF_OpenFont("fonts/Montserrat-Bold.ttf", 144) : TTF_OpenFont("fonts/Montserrat-ExtraBold.ttf", 144);
    // ༼ つ ◕_◕ ༽つ
    if (Montserrat == NULL){
        printf("Erreur de OpenFont : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(Montserrat, "Cahoot", Black); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    free(surface); // A vérifier
    TTF_Quit();
    return texture;
}

SDL_Texture* textureFromImage(SDL_Renderer* renderer, char* imagePath){
    SDL_Surface* image = IMG_Load(imagePath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

    free(image); // A vérifier
    return texture;

}

States* setStates(SDL_Texture* normal, SDL_Texture* hover){
    States *element = malloc(sizeof(States));
    element->normal = normal;
    element->hover = hover;
    return element;
}

void display(SDL_Renderer* renderer, Button button){
    if (button.isHovered){
        if (button.iconRect.x || button.iconRect.y || button.iconRect.w ||button.iconRect.h){
            SDL_RenderCopy(renderer, button.icon->hover, NULL, &(button.iconRect));
        }
        if (button.textRect.x || button.textRect.y || button.textRect.w ||button.textRect.h){
            SDL_RenderCopy(renderer, button.text->hover, NULL, &(button.textRect));
        }
    } else {
        if (button.iconRect.x || button.iconRect.y || button.iconRect.w ||button.iconRect.h){
            SDL_RenderCopy(renderer, button.icon->normal, NULL, &(button.iconRect));
        }
        if (button.textRect.x || button.textRect.y || button.textRect.w ||button.textRect.h){
            SDL_RenderCopy(renderer, button.text->normal, NULL, &(button.textRect));
        }
    }
}