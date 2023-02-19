#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/functions.h"

/**
 * Permet de vérifier que le nom d'un paquet ne contient pas de caractères interdits
 *
 * @param packetName
 * @param forbidden
 * @param len
 * @return
 */
int check(char* packetName, char* forbidden, int len) {

    for (int j = 0; j < len; j++){
        if (strchr(packetName, forbidden[j])){
            return 1;
        }
    }
    return 0;
}

char* stringFromArray(char array[]){
    char* res = array;
    int i = 1;
    while (*(array + i) != '\0'){
        strcat(res, array + i);
    }
    strcat(res, '\0');
    return res;
}

Conf* readConfig() {
    Conf* conf = malloc(sizeof(Conf));
    if (!conf){
        printf("Malloc error while reading conf\n");
        return NULL;
    }
    char* filePath = "./config.json";

    FILE* filePointer = fopen(filePath, "r");

    if (filePointer == NULL) {
        printf("Impossible to open file\n");
        return NULL;
    }
    // On calcule la taille du fichier
    fseek(filePointer, 0, SEEK_END);
    long size = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);

    // On alloue de la mémoire pour stocker le contenu du fichier
    char* buffer = (char *) malloc(size + 1);
    fread(buffer, size, 1, filePointer);
    buffer[size] = '\0';

    fclose(filePointer);

    char* token = strtok(buffer, "\"");
    token = strtok(NULL, "\"");

    //printf("%s\n", token);

    while (token != NULL) {
        if (strstr(token, "max_connection") != NULL) {
            token = strtok(NULL, "\n: ,");
            conf->maxConnections = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(conf->maxConnections, token);
        } else if (strstr(token, "packet_path") != NULL) {
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            conf->packetPath = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(conf->packetPath, token);
        } else if (strstr(token, "ip_base") != NULL) {
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            conf->ip_base = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(conf->ip_base, token);
        } else if (strstr(token, "pseudo") != NULL) {
        token = strtok(NULL, "\"");
        token = strtok(NULL, "\"");
        conf->pseudo = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(conf->pseudo, token);
    }
        token = strtok(NULL, "\"");
        token = strtok(NULL, "\"");
    }


    return conf;
}

char* modifyConfig(Conf* conf) {
    char* res = "";
    char* filePath = "./config.json";

    FILE* filePointer = fopen(filePath, "w");

    if (filePointer == NULL) {
        res = "Impossible to open file\n";
        return res;
    }

    fprintf(filePointer, "{\n");
    fprintf(filePointer, "\t\"max_connections\" : %s,\n", conf->maxConnections);
    fprintf(filePointer, "\t\"packet_path\" : \"%s\",\n", conf->packetPath);
    fprintf(filePointer, "\t\"ip_base\" : \"%s\"\n", conf->ip_base);
    fprintf(filePointer, "}");

    fclose(filePointer);

    return res;
}