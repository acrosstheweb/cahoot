#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../includes/functionsPacket.h"
#include <unistd.h>

char* getPacketPath(char* packetName){
    struct stat s = {0};
    char* directory = "packets/";
    char* extension = ".json";
    size_t len = strlen(directory) + strlen(packetName) + strlen(extension);

    char* filePath = malloc(sizeof(char) * len + 1);

    strcpy(filePath, directory);
    strcat(filePath, packetName);
    strcat(filePath, extension);

    if (stat(directory, &s) == -1) {
        mkdir(directory, 0777);
    }

    return filePath;
}

FILE* createPacketFile(char* packetName) {
    char* filePath = getPacketPath(packetName);
    FILE* filePointer = fopen(filePath, "w+");

    if (filePointer == NULL)
    {
        printf("Échec de la création de fichier lors de la création d'un paquet\n");
        return NULL;
    }

    free(filePath);
    return filePointer;
}

void addQuestionToFile(char *packetName, char* question, char* answer1, char* answer2, char* answer3, char* answer4) {
    char* filePath = getPacketPath(packetName);
    FILE* filePointer = fopen(filePath, "a+");

    fseek (filePointer, 0, SEEK_END);
    if (ftell(filePointer) == 0) {
        fprintf(filePointer, "[\n");
    } else {
        fseek(filePointer, -2, SEEK_END); // on déplace le pointeur de fichier 2 octets avant la fin du fichier
        ftruncate(fileno(filePointer), ftell(filePointer)); // on découpe le fichier à cet emplacement
        fprintf(filePointer, ",\n");
    }

    fprintf(filePointer, "\t{\n");
    fprintf(filePointer, "\t\t\"question\": \"%s\",\n", question);
    fprintf(filePointer, "\t\t\"answers\": [\n");

    char* answers[4] = {answer1, answer2, answer3, answer4};

    for (int i = 0; i < 4; i++) {
        fprintf(filePointer, "\t\t\t{\n");
        fprintf(filePointer, "\t\t\t\"answer\": \"%s\",\n", answers[i]);
        fprintf(filePointer, "\t\t\t\"correct\": \"%s\"\n", (i == 0) ? "true" : "false");
        fprintf(filePointer, "\t\t\t}%s\n", i < 3 ? "," : "");
    }

    fprintf(filePointer, "\t\t]\n\t}\n]");
    fclose(filePointer);
}

char* deletePacket(char* packetName)
{
    char* res = "";
    struct stat s = {0};
    char* directory = "packets/";
    char* extension = ".json";
    size_t len = strlen(directory) + strlen(packetName) + strlen(extension);

    char* filePath = malloc(sizeof(char) * (len + 1));

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

    free(filePath);
    return res;
}

void readPacket(char* packetName, QuestionData*** questionData, int** questionsNb) {
    char* res = "";
    char* filePath = getPacketPath(packetName);

    FILE* filePointer = fopen(filePath, "r");

    if (filePointer == NULL) {
        res = "Impossible to open file\n";
        return;
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

    // On parse le contenu du fichier pour stocker les données dans la structure
    char* token = strtok(buffer, "\"");
    int questionIndex = 0;
    int answerIndex = 0;

    while (token != NULL) {
        if (strstr(token, "question") != NULL) {
            **questionData = realloc(**questionData, sizeof(QuestionData) * (questionIndex + 1));
            // On stocke la question
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            (**questionData + questionIndex)->question = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy((**questionData + questionIndex)->question, token);
        } else if (strstr(token, "answers") != NULL) {
            // On stocke la première réponse
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            (**questionData + questionIndex)->correct_answer = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy((**questionData + questionIndex)->correct_answer, token);
            (**questionData + questionIndex)->answers = malloc(sizeof(char*) * 4);
            (**questionData + questionIndex)->answers[answerIndex] = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy((**questionData + questionIndex)->answers[answerIndex], token);
            answerIndex++;
        } else if (strstr(token, "answer") != NULL) {
            // On stocke les autres réponses
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            (**questionData + questionIndex)->answers[answerIndex] = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy((**questionData + questionIndex)->answers[answerIndex], token);
            answerIndex++;
        }
        if (answerIndex == 4) {
            // Fin d'une question, on réinitialise le compteur de réponse
            questionIndex++;
            answerIndex = 0;
        }
        token = strtok(NULL, "\"");
    }

    token = strtok(NULL, "{");
    **questionsNb = questionIndex;

    free(buffer);
    free(filePath);
}

char* modifyPacket(char* packetName, QuestionData* questionData, int questionsNb) {
    char* res = "";
    char* filePath = getPacketPath(packetName);

    // Réécriture du fichier JSON avec les modification
    FILE* filePointer = fopen(filePath, "w");
    if (filePointer == NULL) {
        res = "Impossible to open file\n";
        return res;
    }

    fprintf(filePointer, "[\n");
    for (int i = 0; i < questionsNb; i++) {
        fprintf(filePointer, "\t{\n");
        fprintf(filePointer, "\t\t\"question\": \"%s\",\n", (questionData)[i].question);
        fprintf(filePointer, "\t\t\"answers\": [\n");
        for (int j = 0; j < 4; j++) {
            fprintf(filePointer, "\t\t\t{\n");
            fprintf(filePointer, "\t\t\t\"answer\": \"%s\",\n", (questionData)[i].answers[j]);
            fprintf(filePointer, "\t\t\t\"correct\": \"false\"\n");
            fprintf(filePointer, "\t\t\t}");
            if (j < 3) {
                fprintf(filePointer, ",");
            }
            fprintf(filePointer, "\n");
        }
        fprintf(filePointer, "\t\t]\n");
        fprintf(filePointer, "\t}");
        if (i < questionsNb - 1) {
            fprintf(filePointer, ",");
        }
        fprintf(filePointer, "\n");
    }
    fprintf(filePointer, "]");

    fclose(filePointer);

    return res;

}

void deleteQuestion(QuestionData** questionData, int* questionsNb, int questionToDelete) {
    if (questionToDelete < *questionsNb){
        for (int i = questionToDelete; i < (*questionsNb) - 1; i++){
            strcpy((*questionData)[i].question, (*questionData)[i + 1].question);
            for (int j = 0; j < 4; j++){
                strcpy((*questionData)[i].answers[j], (*questionData)[i + 1].answers[j]);
            }
        }
    }
    (*questionsNb)--;
    *questionData = realloc(*questionData, sizeof(QuestionData) * (*questionsNb));
}

char** listPackets(int* packetNb){
    char **packetList = NULL;
    *packetNb = 0;
    char* directory = "packets/";
    struct stat s = {0};
    if (stat(directory, &s) == -1) {
        return NULL;
    }
    DIR* rep = NULL;
    struct dirent* fileName = NULL;
    rep = opendir(directory);
    fileName = readdir(rep);
    while (fileName != NULL){
        if (*(fileName->d_name) != '.'){
            packetList = realloc(packetList, sizeof(char*) * ((*packetNb) + 1));
            *(packetList + *packetNb) = malloc(sizeof(char) * (strlen(fileName->d_name) - 4));
            memcpy(*(packetList + *packetNb), fileName->d_name, strlen(fileName->d_name) - 4);
            *(*(packetList + *packetNb) + strlen(fileName->d_name) - 5) = '\0';
            (*packetNb)++;
        }
        fileName = readdir(rep);
    }
    return packetList;
}