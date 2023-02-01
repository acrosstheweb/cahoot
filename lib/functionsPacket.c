#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../includes/functionsPacket.h"
#include <unistd.h>

FILE* createPacketFile(char* packetName)
{
    struct stat s = {0};
    char* directory = "packets/";
    char* extension = ".json";
    size_t len = strlen(directory) + strlen(packetName) + strlen(extension);

    char* filePath = malloc(len + 1);

    strcpy(filePath, directory);
    strcat(filePath, packetName);
    strcat(filePath, extension);

    if (stat(directory, &s) == -1) {
        mkdir(directory, 0777);
    }

    FILE* filePointer = fopen(filePath, "w");

    if (filePointer == NULL)
    {
        printf("Échec de la création d'un paquet\n");
        return NULL;
    }

    fprintf(filePointer, "[");
    fprintf(filePointer, "]");

    free(filePath);
    return filePointer;
}

void addQuestion(FILE* packet, char* question, char* answer1, char* answer2, char* answer3, char* answer4)
{

    fseek(packet, -2, SEEK_END); // on déplace le pointeur de fichier 2 octets avant la fin du fichier
    ftruncate(fileno(packet), ftell(packet)); // on découpe le fichier à cet emplacement

    // Est-ce qu'il faut faire un fopen(...) puis un fclose(...) ?
    fprintf(packet, "{\n");
    fprintf(packet, "   \"question\": \"%s\",\n", question);
    fprintf(packet, "   \"answer\": [\n");

    char* answers[4] = {answer1, answer2, answer3, answer4};
    int correct[4] = {1, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        fprintf(packet, "   {\n");
        fprintf(packet, "   \"answer\": \"%s\n", answers[i]);
        fprintf(packet, "   \"correct\": \"%s\n", correct[i] ? "true" : "false");
        fprintf(packet, "   }%s\n", i < 3 ? "," : "");
    }

    fprintf(packet, "   ]\n");
    fprintf(packet, "}\n");
    fprintf(packet, "]\n");
}

char* deletePacket(char* packetName)
{
    char* res = "";
    struct stat s = {0};
    char* directory = "packets/";
    char* extension = ".json";
    size_t len = strlen(directory) + strlen(packetName) + strlen(extension);

    char* filePath = malloc(len + 1);

    strcpy(filePath, directory);
    strcat(filePath, packetName);
    strcat(filePath, extension);

    if (stat(directory, &s) == -1) {
        res = "No existing packets, cannot delete any.\n";
    } else if (stat(filePath, &s) == -1) {
        res = "Packet doesn't exist, cannot delete it.\n";
    } else {
        if (remove(filePath) == 0) {
            res = "Vous avez bien supprimé le paquet !";
        } else {
            res = "Unable to delete the file";
        }
    }
    return res;
}