#include <stdio.h>
#include <string.h>

int check(char* packetName, char* forbidden, int len)
{
    for (int j = 0; j < len; j++)
        {
            if (strchr(packetName, forbidden[j]))
                return 1;
        }

    return 0;
}