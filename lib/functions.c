#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h> // close
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // getsockname
#include <errno.h> // errno

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

/**
 * Permet de récupérer l'ip de la machine qui éxécute la fonction
 */
char* getIp(){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;
	
	struct sockaddr_in serv;
    
    int sock = socket ( AF_INET, SOCK_DGRAM, 0);
    
    if(sock < 0)
    {
		perror("Socket error");
	}
    
	memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );

    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );
    
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);
    	
	char* lanIp = malloc(sizeof(char) * 64);
    const char* p = inet_ntop(AF_INET, &name.sin_addr, lanIp, 100);
    	
	if(p == NULL)
	{
		printf ("Error number : %d . Error message : %s \n" , errno , strerror(errno));
        return NULL;
    }

    close(sock);
    
    return lanIp;
}