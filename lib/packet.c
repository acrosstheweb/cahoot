#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "packet.h"

FILE* createPacket(char* packetName)
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

    free(filePath);
    return filePointer;
}

void addQuestion(FILE* packet)
{

}

void deletePacket(char* packetName)
{
    struct stat s = {0};
    char* directory = "packets/";
    char* extension = ".json";
    size_t len = strlen(directory) + strlen(packetName) + strlen(extension);

    char* filePath = malloc(len + 1);

    strcpy(filePath, directory);
    strcat(filePath, packetName);
    strcat(filePath, extension);

    if (stat(filePath, &s) == -1) {
        printf("Packet doesn't exist, cannot delete it.\n");
        return;
    }

    if (stat(directory, &s) == -1) {
        printf("No existing packets, cannot delete any.\n");
        return;
    }

    if (remove(filePath) == 0) {
        printf("Deleted successfully");
    } else {
        printf("Unable to delete the file");
    }
   return;
}