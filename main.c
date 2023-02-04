// IDEA: dossier de gif applaudissement en random si bonne réponse
// IDEA: meme chat (animation tournante) si mauvaise réponse
#include <stdio.h>
#include <stdlib.h>
#include "includes/struct.h"
#include "includes/functionsDisplay.h"
#include "includes/functionsPacket.h"
#include "includes/menu.h"
#include "includes/createPacket.h"
#include "includes/managePackets.h"
#include "includes/addQuestion.h"

int main() {
    int choice = 10;
    char* packetName = malloc(sizeof(char*));
    Window* window = create_window();

    /*
    0 - menu
    1 - quit
    2 - settings
    3 - create packet
    4 - manage packets
    5 - host game
    6 - join game
    7 - add question
    */
    do{
        if (choice == 10){
            choice = menu(window);
        }

        switch (choice) {
            case 1:
                break;
            
            case 3:
                newRenderer(window);
                choice = createPacket(window, &packetName);
                break;

            case 4:
                newRenderer(window);
                choice = managePackets(window);
                break;

            case 7:
                choice = addQuestion(window, packetName);
                break;
            
            default:
                /*
                Afficher message (en ligne de commande au cas où ca crash ? )
                "Oups, je ne sais pas ce qui a bien pu se passer...\n"
                       "Vous pouvez nous envoyer un message avec la manip effectuer pour que nous puissions regarder ça de plus près ! ;)\n"
                */
                break;
        }
        
    }while (choice != 1);

    destroy_window(window);
    return 0;
}
