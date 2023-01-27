// IDEA: dossier de gif applaudissement en random si bonne réponse
// IDEA: meme chat (animation tournante) si mauvaise réponse
#include <stdio.h>
#include <stdlib.h>
#include "includes/struct.h"
#include "includes/functionsDisplay.h"
#include "includes/menu.h"
#include "includes/createPacket.h"
#include "includes/managePackets.h"

int main() {
    int choice;
    Window* window = create_window();

    do{
        choice = 0;
        choice = menu(window);

        switch (choice) {
            case 1:
                break;
            
            case 3:
                newRenderer(window);
                choice = createPacket(window);
            break;

            case 4:
                newRenderer(window);
                choice = managePackets(window);
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
