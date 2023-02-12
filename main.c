// IDEA: dossier de gif applaudissement en random si bonne réponse
// IDEA: meme chat (animation tournante) si mauvaise réponse
#include <stdio.h>
#include <stdlib.h>
#include "includes/struct.h"
#include "includes/functions.h"
#include "includes/functionsDisplay.h"
#include "includes/functionsPacket.h"
#include "includes/functionsNetwork.h"
#include "includes/menu.h"
#include "includes/createPacket.h"
#include "includes/managePackets.h"
#include "includes/addQuestion.h"
#include "includes/hosting.h"
#include "includes/choosePacket.h"
#include "includes/manageQuestions.h"
#include "includes/client.h"
#include "includes/settings.h"

int main() {

    Conf* conf = readConfig();
    int choice = 10;
    char* packetName = malloc(sizeof(char*));
    Window* window = create_window();
    QuestionData* questionData = NULL;
    int questionsNb = 0;

    /*
    0 - menu
    1 - quit
    2 - settings
    3 - create packet
    4 - manage packets
    5 - host game
    6 - join game
    7 - add question
    8 - modify questions
    */
    do{
        if (choice == 10){
            choice = menu(window);
        }

        switch (choice) {
            case QUIT_WINDOW:
                bye(window);
                break;
                            
            case 2:
                choice = settings(window, &conf);
                break;
            
            case MENU_CREATE_PACKET:
                newRenderer(window);
                choice = createPacket(window, &packetName);
                break;

            case MENU_MANAGE_PACKET:
                newRenderer(window);
                choice = managePackets(window, &packetName, &questionData, &questionsNb);
                break;

            case MENU_HOST_GAME:
                newRenderer(window);
                choice = managePackets(window, &packetName, &questionData, &questionsNb);
                // choice = choosePacket(window, &packetName);
                // if (choice == 5){
                choice = hosting(window, packetName, questionData, questionsNb);
                // }
                break;

            case MENU_JOIN_GAME:
                choice = client(window, &conf->ip_base);
                break;

            case 7:
                choice = addQuestion(window, packetName);
                break;

            case 8:
                choice = manageQuestions(window, packetName, questionData, questionsNb);
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
