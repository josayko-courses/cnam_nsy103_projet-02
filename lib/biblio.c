#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXMESS 100

/*******************************************************************
   Boucle d'attendre et de reception d'un message UDP

   -> En argument d'entree :
              - numero de Port d'écoute,
              - buffer message pour receptionner le message

   -> En argument de sortie :
               0 = OK et le buffer message en paramètre d'entrée
                   contient le message
              -1 = faute

 Auteurs : Emile Geahchan
 version du 2/12/20
 *******************************************************************/

int reception(unsigned short port, char* message)
{
    int sock;
    struct sockaddr_in local, distant;
    unsigned short distant_port;
    char distant_adresse[50];
    char* ptn_adresse = distant_adresse;
    int len, n;
    errno = 0;

    /* Création socket */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        goto faute1;

    /* Bind */
    memset(&local, 0, sizeof(local)); // precaution
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY; // une des adresses IP locales
    local.sin_port = htons(port); // numéro de port
    if (bind(sock, (const struct sockaddr*)&local, sizeof(local)) < 0)
        goto faute2;

    /* Attente et Réception */
    n = recvfrom(sock, (char*)message, MAXMESS, 0, (struct sockaddr*)&distant, &len);
    message[n] = '\0';

    /* Retour OK */
    close(sock);
    return (0);

/* USAGE */
usage:
    printf("Usage : serveur <Port>\n>");
    return (-1);

/* CAS DE FAUTES */
faute1:
    perror("faute socket serveur : ");
    return (-1);
faute2:
    perror("faute bind serveur : ");
    return (-1);
}

/*******************************************************************
   Emission d'un message UDP

   -> En argument d'entree :
              - Adresse IP du serveur en notation décimale pointée
              - Port d'écoute du serveur
              - Message à envoyer

   -> En argument de sortie :
               0 = OK
              -1 = faute

 Auteurs : Emile Geahchan
 version du 2/12/20
 *******************************************************************/

int emission(char* ip, unsigned short port, char* message)
{
    int sock;
    unsigned long adresse;
    struct sockaddr_in local, distant;
    int len, n;
    errno = 0;

    /* Création socket */
    if (inet_aton(ip, (struct in_addr*)&adresse) == 0)
        goto usage;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        goto faute1;

    /* Bind */
    memset(&local, 0, sizeof(local)); // precaution
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY; // une des adresses IP locales
    local.sin_port = 0; // numero de port affecté par le système
    if (bind(sock, (const struct sockaddr*)&local, sizeof(local)) < 0)
        goto faute2;

    /* Envoi message */
    memset(&distant, 0, sizeof(&distant)); // precaution
    distant.sin_family = AF_INET;
    distant.sin_addr.s_addr = adresse; // adresse IP serveur
    distant.sin_port = htons(port); // numéro de port serveur
    sendto(sock, (const char*)message, strlen(message), MSG_CONFIRM,
        (const struct sockaddr*)&distant, sizeof(distant));

    /* Retour OK */
    close(sock);
    return (0);

/* USAGE */
usage:
    printf("Usage : client <IP> <Port> <message>\n>");
    return (-1);

/* CAS DE FAUTES */
faute1:
    perror("faute socket client : ");
    return (-1);
faute2:
    perror("faute bind client : ");
    return (-1);
}

void debug(char* message)
{
    printf("%s\n", message);
}
