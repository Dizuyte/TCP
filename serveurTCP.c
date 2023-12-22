/********************************************************
* * nom : serveur.c
* * Auteur : NG 15/12/2023
* * version : 0.1
* * descr : serveur TCP echo
* * licence : GPL
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "utilisation : serveurTCP port\n");
        return 1;
    }

    // Obtenir le numéro de port à partir des arguments de ligne de commande
    int port = atoi(argv[1]);

    // Créer une socket
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Échec de création de la socket");
        return 1;
    }

    // Initialiser la structure sockaddr_in6
    struct sockaddr_in6 adresse_serveur;
    memset(&adresse_serveur, 0, sizeof(adresse_serveur));
    adresse_serveur.sin6_family = AF_INET6;
    adresse_serveur.sin6_port = htons(port);
    adresse_serveur.sin6_addr = in6addr_any;

    // Lier la socket à l'adresse et au port spécifiés
    if (bind(sockfd, (struct sockaddr*) &adresse_serveur, sizeof(adresse_serveur)) < 0) {
        perror("Échec de liaison de la socket");
        return 1;
    }

    // Écouter les connexions entrantes
    if (listen(sockfd, 5) < 0) {
        perror("Échec de l'écoute des connexions");
        return 1;
    }

    printf("Serveur en attente de connexions...\n");

    while (1) {
        // Accepter une nouvelle connexion
        struct sockaddr_in6 adresse_client;
        socklen_t taille = sizeof(adresse_client);
        int newsockfd = accept(sockfd, (struct sockaddr*) &adresse_client, &taille);
        if (newsockfd < 0) {
            perror("Échec de l'acceptation de la connexion");
            return 1;
        }

        // Obtenir l'adresse IP du client
        char clientIP[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(adresse_client.sin6_addr), clientIP, sizeof(clientIP));
        printf("Client connecté : %s\n", clientIP);

        // Recevoir les données du client dans un buffer
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("Échec de la réception des données");
            return 1;
        }

        // Ajouter le caractère de fin de chaîne pour terminer correctement la chaîne
        buffer[bytes_received] = '\0';

        printf("Données reçues : %s\n", buffer);

        // Envoyer les données de retour au client
        int bytes_sent = send(newsockfd, buffer, strlen(buffer), 0);
        if (bytes_sent < 0) {
            perror("Échec de l'envoi des données");
            return 1;
        }

        printf("Données renvoyées : %s\n", buffer);

        // Fermer la socket de la nouvelle connexion
        close(newsockfd);
    }

    // Fermer la socket d'écoute
    close(sockfd);

    return 0;
}
