#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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