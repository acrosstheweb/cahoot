#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/struct.h"
#include "../includes/functionsDisplay.h"

/**
 * Permet d'ajouter un bouton à une liste chaînée
 *
 * @param first
 * @param iconRect
 * @param icon
 * @param textRect
 * @param text
 */
void addButtonToList(Node** first, SDL_Rect iconRect, States* icon, SDL_Rect textRect, States* text, int isOption, int isClickable){
    Node* node = malloc(sizeof(Node));
    node->button.isHovered = 0;
    node->button.iconRect= iconRect;
    node->button.icon = icon;
    node->button.textRect = textRect;
    node->button.text = text;
    node->button.isOption = isOption;
    node->button.isClickable = isClickable;
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

/**
 * Permet de vérifier si un élement est survolé
 *
 * @param first
 * @param mouseX
 * @param mouseY
 */
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

/**
 * Permet d'afficher tous les éléments d'un paquet
 *
 * @param first
 */
void print(Node* first) {
    Node *temp = first;
    if (first != NULL) {
        do {
        printf("current: %p\ncurrent->next: %p\n-----\n", temp, temp->next);
            temp = temp->next;
        }while (temp != first);
    }
}


/**
 * Permet d'avoir la largeur "naturelle" d'un texte
 *
 * @param renderer
 * @param imagePath
 * @return
 */
int getTextWidth(char* message, int isBold, int height){
    int w = 0;
    int h = 0;
    if (TTF_Init() != 0){
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    TTF_Font* Montserrat = NULL;
    
    Montserrat = (isBold) ? TTF_OpenFont("fonts/Montserrat-ExtraBold.ttf", 144) : TTF_OpenFont("fonts/Montserrat-Bold.ttf", 144);
    // ༼ つ ◕_◕ ༽つ
    if (Montserrat == NULL){
        printf("Erreur de OpenFont : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }
    
    if (!TTF_SizeText(Montserrat, message, &w, &h)){
        w = (w*height)/176;
    } else {
        printf("ERROR: could not calculate text width\n");
        return 0;
    }

    TTF_Quit();
    return w;
}

/**
 * Permet de créer la texture d'un texte pour SDL
 *
 * @param renderer
 * @param message
 * @param isBold
 * @return
 */
SDL_Texture* textureFromMessage(SDL_Renderer* renderer, char* message, SDL_Color color){
    if (TTF_Init() != 0){
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    TTF_Font* Montserrat = NULL;
    
    Montserrat =  TTF_OpenFont("fonts/Montserrat-Bold.ttf", 144);
    // ༼ つ ◕_◕ ༽つ
    if (Montserrat == NULL){
        printf("Erreur de OpenFont : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(Montserrat, message, color); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    free(surface); // A vérifier
    TTF_Quit();
    return texture;
}

/**
 * Permet de créer la texture d'une image pour SDL
 *
 * @param renderer
 * @param imagePath
 * @return
 */
SDL_Texture* textureFromImage(SDL_Renderer* renderer, char* imagePath){
    SDL_Surface* image = IMG_Load(imagePath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

    free(image); // A vérifier
    return texture;

}

/**
 * Permet de définir les textures qui représentent un élément
 *
 * @param normal
 * @param hover
 * @return
 */
States* setStates(SDL_Texture* normal, SDL_Texture* hover){
    States *element = malloc(sizeof(States));
    element->normal = normal;
    element->hover = hover;
    return element;
}

/**
 * Permet d'afficher un élément
 *
 * @param renderer
 * @param button
 */
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