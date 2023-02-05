#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../includes/struct.h"
#include "../includes/functionsDisplay.h"

Window* create_window() {
    SDL_Init(SDL_INIT_VIDEO);

    Window* window = malloc(sizeof(Window));
    if(window == NULL){
        printf("Echec malloc window");
        exit(EXIT_FAILURE);
    }

    // Initialisation de la fenêtre
    window->sdl_window = SDL_CreateWindow("Cahoot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window->sdl_window == NULL) {
        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    window->renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);
    
    window->font = initTTF();

    return window;

}

void newRenderer(Window* window){
    SDL_DestroyRenderer(window->renderer);
    window->renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);
}

void destroy_window(Window *window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdl_window);
    TTF_CloseFont(window->font);
    TTF_Quit();
    free(window);
    SDL_Quit();
    return;
}

/**
 * Permet d'avoir un rectangle vide
 *
 */
SDL_Rect empty(){
    SDL_Rect emptyRect = {0, 0, 0, 0};
    return emptyRect;
}

/**
 * Permet d'avoir une couleur par son nom
 *
 * @param name
 */
SDL_Color setColor(char* name){
    SDL_Color Black = {27, 27, 27, 255};
    SDL_Color Red = {208, 19, 23, 255};
    SDL_Color Green = {2, 167, 125, 255};
    SDL_Color Blue = {55, 101, 174, 255};
    SDL_Color Yellow = {253, 197, 0, 255};
    if (strcmp(name, "Red") == 0){
        return Red;
    } else if (strcmp(name, "Green") == 0){
        return Green;
    } else if (strcmp(name, "Blue") == 0){
        return Blue;
    } else if (strcmp(name, "Yellow") == 0){
        return Yellow;
    }
    return Black;
}

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

TTF_Font* initTTF(){
    if (TTF_Init() != 0){
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    TTF_Font* Montserrat = TTF_OpenFont("fonts/Montserrat-Bold.ttf", 144);
    // ༼ つ ◕_◕ ༽つ
    if (Montserrat == NULL){
        printf("Erreur de OpenFont : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(-1);
    }

    return Montserrat;
}
/**
 * Permet d'avoir la largeur "naturelle" d'un texte
 *
 * @param message
 * @param height
 * @param Monsterrat
 * @return
 */
int getTextWidth(char* message, int height){
    int w = 0;
    int h = 0;
    if (TTF_Init() != 0){
        printf("Erreur de TTF_Init() : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    TTF_Font* Montserrat = TTF_OpenFont("fonts/Montserrat-Bold.ttf", 144);
    // ༼ つ ◕_◕ ༽つ
    if (Montserrat == NULL){
        printf("Erreur de OpenFont : \n%s\n", TTF_GetError());
        SDL_Quit();
        exit(-1);
    }
    if (!TTF_SizeText(Montserrat, message, &w, &h)){
        w = (w*height)/176;
    } else {
        printf("Erreur de TTF_SizeText() : \n%s\n", TTF_GetError());
        return 0;
    }
    TTF_CloseFont(Montserrat);
    TTF_Quit();
    return w;
}

/**
 * Permet de créer la texture d'un texte pour SDL
 *
 * @param renderer
 * @param message
 * @param color
 * @return
 */
SDL_Texture* textureFromMessage(SDL_Renderer* renderer, char* message, SDL_Color color, TTF_Font* font){

    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    free(surface); // A vérifier
    // TTF_Quit();
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

void addTemplateToList(Node** first, Window* window, int displayLogo, int displayBackToMenu, int displaySettings, char* titleText){
    if (displayLogo){
        SDL_Texture* cahootTexture = textureFromMessage(window->renderer, "Cahoot", setColor("Black"), window->font);
        States* logo = setStates(cahootTexture, cahootTexture);
        SDL_Rect logoRect = {
            (SCREEN_WIDTH - getTextWidth("Cahoot", 100)) / 2,
            MARGIN / 10,
            getTextWidth("Cahoot", 100),
            100
        };
        addButtonToList(first, logoRect, logo, empty(), NULL, 0, 0);
    }
    if (displayBackToMenu){
        SDL_Texture* buttonMenuTexture = textureFromImage(window->renderer, "img/back_to_menu.png");
        SDL_Texture* buttonMenuHoverTexture = textureFromImage(window->renderer, "img/back_to_menu_hover.png");
        SDL_Texture* menuTextTexture = textureFromMessage(window->renderer, "Menu principal", setColor("Black"), window->font);
        SDL_Texture* menuTextHoverTexture = textureFromMessage(window->renderer, "Menu principal", setColor("Yellow"), window->font);
        States* menu = setStates(buttonMenuTexture, buttonMenuHoverTexture);
        States* menuText = setStates(menuTextTexture, menuTextHoverTexture);
        SDL_Rect buttonMenuRect = {
            MARGIN,
            MARGIN + (SETTINGS_HEIGHT - MENU_HEIGHT)/2,
            MENU_WIDTH,
            MENU_HEIGHT
        };
        SDL_Rect menuTextRect = {
            buttonMenuRect.x + buttonMenuRect.w + 10,
            buttonMenuRect.y - MENU_HEIGHT / 2,
            getTextWidth("Menu principal", MENU_HEIGHT * 2),
            MENU_HEIGHT * 2
        };
        addButtonToList(first, buttonMenuRect, menu, menuTextRect, menuText, 0, 10);
    }
    if (displaySettings){
        SDL_Texture* buttonSettingsTexture = textureFromImage(window->renderer, "img/settings.png");
        SDL_Texture* buttonSettingsHoverTexture = textureFromImage(window->renderer, "img/settings_hover.png");
        States* settings = setStates(buttonSettingsTexture, buttonSettingsHoverTexture);
        SDL_Rect buttonSettingsRect = {
            SCREEN_WIDTH - SETTINGS_WIDTH - MARGIN,
            MARGIN,
            SETTINGS_WIDTH,
            SETTINGS_HEIGHT
        };
        addButtonToList(first, buttonSettingsRect, settings, empty(), NULL, 0, 2);
    }
    
    SDL_Texture* titleTexture = textureFromMessage(window->renderer, titleText, setColor("Black"), window->font);
    States* title = setStates(titleTexture, titleTexture);
    SDL_Rect titleRect = {
        (SCREEN_WIDTH - getTextWidth(titleText, 50)) / 2,
        100,
        getTextWidth(titleText, 50),
        50
    };
    addButtonToList(first, titleRect, title, empty(), NULL, 0, 0);
    
    SDL_Texture* buttonLeaveTexture = textureFromImage(window->renderer, "img/quit_program.png");
    SDL_Texture* buttonLeaveHoverTexture = textureFromImage(window->renderer, "img/quit_program_hover.png");
    States* quitApp = setStates(buttonLeaveTexture, buttonLeaveHoverTexture);
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - LEAVE_WIDTH) / 2,
        SCREEN_HEIGHT - LEAVE_HEIGHT - MARGIN,
        LEAVE_WIDTH,
        LEAVE_HEIGHT
    };
    addButtonToList(first, buttonLeaveRect, quitApp, empty(), NULL, 0, 1);
}

void updateInputText(Window* window, char* inputText, SDL_Rect refRect, SDL_Rect inputRect){
    SDL_Texture *texture = textureFromMessage(window->renderer, inputText, setColor("Black"), window->font);
    inputRect.w = getTextWidth(inputText, inputRect.h);
    inputRect.x = refRect.x + (refRect.w- inputRect.w) / 2;
    inputRect.y = refRect.y + (refRect.h- inputRect.h) / 2;
    SDL_RenderCopy(window->renderer, texture, NULL, &inputRect);
}