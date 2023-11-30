/********************************************************
 * * nom : clientTCP.c
 * * Auteur : AF 16/09/2023
 * * version : 0.2
 * * descr : client TCP echo
 * * licence : GPL
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char **argv, char **env) {
    if (argc < 3) {
        fprintf(stderr, "usage : clientTCP server port\n");
        return 1;
    }

    const char* serverName = argv[1];
    const char* serviceName = argv[2];

    // Récupérer l'adresse IP du serveur
    struct hostent* server = gethostbyname(serverName);
    if (server == NULL) {
        fprintf(stderr, "Impossible de résoudre le nom du serveur\n");
        return 1;
    }

    // Afficher l'adresse IP rendue par gethostbyname
    printf("Adresse IP : %s\n", inet_ntoa(*((struct in_addr*)server->h_addr)));

    // Afficher le port
    printf("Port : %s\n", serviceName);

    // Initialiser la structure sockaddr_in
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(serviceName));
    serverAddress.sin_addr = *((struct in_addr*)server->h_addr);

    // Utiliser la structure sockaddr_in pour communiquer avec le serveur

    return 0;
}

