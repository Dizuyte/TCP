/********************************************************
* * nom : clientTCP.c
* * Auteur : NetCo 15/12/2023
* * version : 0.2
* * descr : client TCP echo
* * licence : GPL
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv, char **env) {
    if (argc < 3) {
        fprintf(stderr, "utilisation : clientTCP serveur port\n");
        return 1;
    }

    // Obtenir les informations d'adresse du serveur
    struct hostent *serveur = gethostbyname2(argv[1], AF_INET6);
    if (serveur == NULL) {
        fprintf(stderr, "Impossible de résoudre l'adresse du serveur.\n");
        return 1;
    }

    // Initialiser la structure sockaddr_in6
    struct sockaddr_in6 adresse_serveur;
    memset(&adresse_serveur, 0, sizeof(adresse_serveur));
    adresse_serveur.sin6_family = AF_INET6;
    adresse_serveur.sin6_port = htons(atoi(argv[2]));
    memcpy(&adresse_serveur.sin6_addr, serveur->h_addr_list[0], serveur->h_length);

    // Créer une socket
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Échec de création de la socket");
        return 1;
    }

    // Se connecter au serveur
    if (connect(sockfd, (struct sockaddr*) &adresse_serveur, sizeof(adresse_serveur)) < 0) {
        perror("Échec de la connexion au serveur");
        return 1;
    }

    // Connexion réussie !
    printf("Connecté au serveur.\n");

    // Chaîne de caractères à envoyer
    char *message;
    if (argc >= 4) {
        message = argv[3];  // Utiliser la chaîne passée en argument
    } else {
        message = "CIEL";  // Utiliser la chaîne "CIEL" par défaut
    }

    // Envoyer la chaîne de caractères
    int bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Échec de l'envoi des données");
        return 1;
    }

    printf("Données envoyées : %s\n", message);

    // Recevoir les données du serveur dans un buffer
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Échec de la réception des données");
        return 1;
    }

    // Ajouter le caractère de fin de chaîne pour terminer correctement la chaîne
    buffer[bytes_received] = '\0';

    printf("Données reçues : %s\n", buffer);

    // Effectuer les opérations souhaitées avec le serveur connecté

    // Fermer la socket
    close(sockfd);

    return 0;
}
