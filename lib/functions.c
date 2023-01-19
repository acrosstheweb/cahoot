#include <stdio.h>
#include <string.h>

int check(char* packetName, char* forbidden, int len)
{
    int i, j;
    for (i = 0; i < strlen(packetName); i++)
    {
        for (j = 0; j < len; j++)
        {
            if (packetName[i] == forbidden[j])
                return 1;
        }
    }

    return 0;
}