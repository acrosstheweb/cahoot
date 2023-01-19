#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/packet.h"

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

        if (!(choice >= 1 && choice <= 2))
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
    char *message = " ";
    char *stopWord = "-stop";
    char *textInput = (char *) malloc(sizeof(char) * 30);

    do
    {
        system("clear");
        if(strcmp(message, " ") != 0)
        {
            printf("%s\n", message);
            message = " ";
        }
        choice = menu();

        switch (choice) {

            case 1:

                system("clear");
                printf("Entrez '%s' pour sortir.\n", stopWord);

                printf("Quel nom voulez-vous donner à votre nouveau paquet ?\n");
                scanf("%[^\n]s", textInput);
                getchar();
                //do
                //{

                    //TODO : vérification des caracères non autorisés '/\*|<>:?"'
                //}
                //while (strcmp(textInput, stopWord) != 0);

                createPacket(textInput);
                message = "Vous avez bien créé le paquet !";

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