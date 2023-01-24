// TODO: add resource function to add resource created to an array
// TODO: cleanup on leave function to free all resources in array
#include "window.h"

/*
    printf("---- MENU ----\n");
    printf("1 - Créer un Paquet\n");
    printf("2 - Voir mes paquets\n");
    printf("3 - Supprimer un paquet\n");
    printf("4 - Renommer un paquet\n");
    printf("5 - Ajouter une question à un paquet\n");
    printf("6 - Supprimer une question d'un paquet\n");
    printf("7 - Afficher les questions d'un paquet\n");
    printf("8 - EXIT\n");
 */
void menu(Window* window) {

    // Charger les images des boutons
    SDL_Surface* buttonSelect = IMG_Load("img/arrow_select.png");
    SDL_Surface* buttonSelectHover = IMG_Load("img/arrow_select_hover.png");
    SDL_Surface* buttonLeave = IMG_Load("img/quit_program.png");
    SDL_Surface* buttonLeaveHover = IMG_Load("img/quit_program_hover.png");
    SDL_Surface* buttonSettings = IMG_Load("img/settings.png");
    SDL_Surface* buttonSettingsHover = IMG_Load("img/settings_hover.png");

    // Créer les textures des boutons
    SDL_Texture* buttonSelectTexture = SDL_CreateTextureFromSurface(window->renderer, buttonSelect);
    SDL_Texture* buttonSelectHoverTexture = SDL_CreateTextureFromSurface(window->renderer, buttonSelectHover);
    SDL_Texture* buttonLeaveTexture = SDL_CreateTextureFromSurface(window->renderer, buttonLeave);
    SDL_Texture* buttonLeaveHoverTexture = SDL_CreateTextureFromSurface(window->renderer, buttonLeaveHover);
    SDL_Texture* buttonSettingsTexture = SDL_CreateTextureFromSurface(window->renderer, buttonSettings);
    SDL_Texture* buttonSettingsHoverTexture = SDL_CreateTextureFromSurface(window->renderer, buttonSettingsHover);
    SDL_Texture* test = NULL;
    SDL_Texture* testLeave = NULL;

    // Obtenir les dimensions des boutons
    int buttonSelectWidth = 50, buttonSelectHeight = 50;
    int buttonLeaveWidth = 50, buttonLeaveHeight = 50;
    int buttonSettingsWidth = 50, buttonSettingsHeight = 50;

    // Définir les positions des boutons (x, y, w, h)
    SDL_Rect buttonSelect1Rect = {
        (SCREEN_WIDTH - buttonSelectWidth) / 2 - buttonSelectWidth,
        100,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonSelect2Rect = {
        (SCREEN_WIDTH - buttonSelectWidth) / 2 - buttonSelectWidth,
        200,
        buttonSelectWidth,
        buttonSelectHeight
    };
    SDL_Rect buttonLeaveRect = {
        (SCREEN_WIDTH - buttonLeaveWidth) / 2,
        SCREEN_HEIGHT - (SCREEN_HEIGHT / 6),
        buttonLeaveWidth,
        buttonLeaveHeight
    };
    SDL_Rect buttonSettingsRect = {
        SCREEN_WIDTH - buttonSettingsWidth / 2,
        buttonSettingsHeight,
        buttonSettingsWidth,
        buttonSettingsHeight
    };
    
    Button* first = NULL;
    Button* current = NULL;
    addButtonToList(first, buttonSettingsRect, buttonSettingsTexture, buttonSettingsHoverTexture);
    printf("first: %p\nnext: %p\n", first, first->next);
    addButtonToList(first, buttonLeaveRect, buttonLeaveTexture, buttonLeaveHoverTexture);
    printf("first: %p\nnext: %p\nnextnext: %p", first, first->next, first->next->next);


    
    // Boucle principale
    int quit = 0;
    while (!quit) {
        SDL_Event e;
        
        //je suppose, bau pire on met ttf ou jsp quoi SDL_font à null tout le temps
        //laisse comme ça pour l'instant, on changera plus tard si besoin oui

        /* while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEMOTION) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                while(current->next != buttons->first){
                    if ( mouseX >= current->rect.x && mouseX <= current->rect.x + current->rect.w &&
                        mouseY >= current->rect.y && mouseY <= current->rect.y + current->rect.h)
                    {
                        hover(window, current);
                    }
                    current = current->next;
                }
            }
        } */
        printf("jaaj");
        int x, y;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEMOTION:
                    x = e.motion.x;
                    y = e.motion.y;
                    checkHover(first, x, y);
                    break;
                case SDL_QUIT:
                    return;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(window->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(window->renderer);

        while(current->next != first){
            if (current->isHovered)
            {
                // hover(window, current);
                SDL_RenderCopy(window->renderer, current->textureHovered, NULL, &(current->rect));
            } else {
                SDL_RenderCopy(window->renderer, current->textureHovered, NULL, &(current->rect));
            }
        }
    

/* 
        while(current->next != buttons->first){
            SDL_RenderCopy(window->renderer, current->texture, NULL, &(current->rect));
            current = current->next;
        } */

        // Mettre à jour l'affichage
        SDL_RenderPresent(window->renderer);
    }
    return;
}

void option(int option)/* il veut quoi lui ->*/{
    printf("Option %d choisie", option);
}

void checkHover(Button* b, int mouseX, int mouseY){
    Button* current = b;
    while (current->next != b){
        if ( mouseX >= b->rect.x && mouseX <= b->rect.x + b->rect.w &&
            mouseY >= b->rect.y && mouseY <= b->rect.y + b->rect.h)
        {
            b->isHovered = 1;
        } else {
            b->isHovered = 0;
        }
        current = current->next;
    }
}

void addButtonToList(Button* button, SDL_Rect rect, SDL_Texture* texture, SDL_Texture* textureHovered){
    printf("in function, button : %p\n", button);
    if (!button){
        button = malloc(sizeof(Button));
        if(button == NULL){
            printf("Echec malloc button");
            exit(EXIT_FAILURE);
        }
        button->isHovered = 0;
        button->next = button;
        button->rect = rect;
        button->texture = texture;
        button->textureHovered = textureHovered;
    } else {
        Button* first = button;
        while (button->next != first){
            button = button->next;
        }

        button->next = malloc(sizeof(Button));
        if(button->next == NULL){
            printf("Echec malloc button->next");
            exit(EXIT_FAILURE);
        }
        button->rect = rect;
        button->texture = texture;
        button->textureHovered = textureHovered;
        button->next->next = first;
    }
}

/* void display(ButtonList* buttonList)
{
    if (buttonList->first == NULL){
        printf("\nList is empty\n");
    }
    else {
        struct Button* temp;
        temp = last->next;
  
        // While first node is not
        // reached again, print,
        // since the list is circular
        do {
            printf("\nID = %d", temp->id);
            temp = temp->next;
        } while (temp != last->next);
    }
} */