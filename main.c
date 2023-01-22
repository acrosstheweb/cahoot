#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/packet.h"
#include "lib/functions.h"
// #include "window/window.h"

int menu()
{
    int choice;
    int valid = 0;
    char *message = "Que voulez-vous faire ?";

    printf("---- MENU ----\n");
    printf("1 - Créer un Paquet\n");
    printf("2 - Voir mes paquets\n");
    printf("3 - Supprimer un paquet\n");
    printf("4 - Renommer un paquet\n");
    printf("5 - Ajouter une question à un paquet\n");
    printf("6 - Supprimer une question d'un paquet\n");
    printf("7 - Afficher les questions d'un paquet\n");
    printf("8 - EXIT\n");

    do
    {
        printf("%s\n", message);
        scanf("%d", &choice);
        getchar();

        if (!(choice >= 1 && choice <= 3))
            message = "Aïe ! Cette option n'existe pas encore ! Veuillez réessayer :";
        else
            valid = 1;
    }
    while (!valid);

    return choice;
}


int main()
{
    int choice;
    char *message = "";
    char *stopWord = "-stop";
    char *textInput = (char *) malloc(sizeof(char) * 30);
    char forbidden[9] = {'<', '>', '"', ':', '/', '\\', '|', '?', ' '};
    int len = sizeof(forbidden) / sizeof (forbidden[0]); // permet de retourner la taille du tableau

    // Window *window = create_window();
    //
    //
    // destroy_window(window);
    //
    // SDL_Quit();
    //
    // return 0;

    do
    {
        system("clear");
        if(strcmp(message, "") != 0)
        {
            printf("%s\n", message);
            message = "";
        }
        choice = menu();

        switch (choice) {

            case 1:

                system("clear");
                printf("Entrez '%s' pour sortir.\n", stopWord);

                printf("Quel nom voulez-vous donner à votre nouveau paquet ?\nCaractères interdits : %s\n", forbidden);
                scanf("%[^\n]s", textInput);
                getchar();

                if (!strcmp(textInput, stopWord)) {
                    break;
                }

                while (check(textInput, forbidden, len)){
                    printf("Erreur: votre saisie comporte au moins un caractère interdit.\nQuel nom voulez-vous donner à votre nouveau paquet ?\nCaractères interdits : %s\n", forbidden);
                    scanf("%[^\n]s", textInput);
                    getchar();
                }

                createPacket(textInput);
                message = "Vous avez bien créé le paquet !";

                break;

            case 3:

                system("clear");
                printf("Entrez '%s' pour sortir.\n", stopWord);

                printf("Quel paquet voulez-vous supprimer ?\n");
                scanf("%[^\n]s", textInput);
                getchar();

                if (!strcmp(textInput, stopWord)) {
                    break;
                }

                while (check(textInput, forbidden, len)) {
                    printf("Erreur: votre saisie comporte au moins un caractère interdit.\nQuel nom voulez-vous donner à votre nouveau paquet ?\nCaractères interdits : %s\n", forbidden);
                    scanf("%[^\n]s", textInput);
                    getchar();
                }

                message = deletePacket(textInput);

                break;

            case 2:
                printf("Bye\n");

                free(textInput);

                return 1;


            default:
                printf("Oups, nous ne savons pas ce qui a bien pu se passer...\n"
                       "Vous pouvez nous envoyer un message avec la manip effectuer pour que nous puissions regarder ça de plus près ! ;)\n");


                free(textInput);

                return 0;
        }
    }
    while (choice != 2);
}