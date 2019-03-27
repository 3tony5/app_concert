// ci dessous l'application achat qui va contenir les clients de l'application concert
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "gestionnaire_concert.h"

int main(int argc, char **argv) {
    int transFini = 0;
    int codeBancaire, prix;
    places achat, vente;
    char  validation[200], reponse[5];
    int sock;
    struct sockaddr_in adresse_concert;
    struct hostent *hote;

    // etablir la connexion avec concert

    /* creation de la socket locale */
    if ((sock = socket(AF_INET, SOCK_STREAM,0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* recuperation de l'adresse IP du serveur (a partir de son nom) */
    if ((hote = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(2);
    }
    /* preparation de l'adresse du serveur */

    adresse_concert.sin_family = AF_INET;
    adresse_concert.sin_port = htons((unsigned int)PORT_CONCERT);
    bcopy(hote->h_addr, &adresse_concert.sin_addr, hote->h_length);

    printf("L'adresse en notation pointee %s\n", inet_ntoa(adresse_concert.sin_addr));
    fflush(stdout);

    /* demande de connexion au serveur */
    if (connect(sock, (struct sockaddr *) &adresse_concert, sizeof(adresse_concert)) == -1 ) {
        perror("connect");
        exit(3);
    }

    /* le serveur a accepte la connexion */
    printf("connexion acceptee\n");
    fflush(stdout);


    //debut du traitement des données

    //tant que transaction non terminés.
    while (transFini == 0) {

        //demander un certain nombre de places pour une certaines categories
        printf("categories désirés 1, 2 ou 3 ?");
        scanf("%i", &(achat.categorie));
        printf("nombre de places désirés ?");
        scanf("%i", &achat.nbPlaces);
	achat.nbPlaces=0-achat.nbPlaces;
        // envoyer ces données a concert
        if (write(sock, &achat, sizeof(places)) != sizeof(places)) {
            perror("write achat");
            exit(4);
        }

        // récupere le nombres de places autorisée
        if (read(sock, &vente, sizeof(places)) < 0) {
            perror("read achat");
            exit(5);
        }
        //si nbautorisé = nbdemandé
        if (vente.nbPlaces == achat.nbPlaces){

            if (read(sock, &prix, sizeof(int)) < 0) {
                perror("read");
                exit(6);
            }

            //demandé et envoyer code carte bleu
            printf ("cela vous fera %i \n", prix);
            printf("Veuillez entrez votre code de carte bleu\n");
            scanf("%i", &codeBancaire);
            if (write(sock, &codeBancaire, sizeof(places)) != sizeof(places)) {
                perror("write achat cb");
                exit(7);
            }

            // attendre ok de concert
            if (read(sock, &validation, sizeof(char)*201) < 0) {
                perror("read achat");
                exit(8);
            }
		if (strcmp (validation,"ok")==0){
		printf("achat réussi\n");
		}
            // fin de la transaction
            transFini = 1;
            // Fin si sinon si nbautorisé =0 fin de la transaction
        }
        else {
            if (vente.nbPlaces == 0) {
                printf("plus de places dans la categories demandé. \n");
                transFini = 1;
            }
            // sinon présenté offres 
            else {
                printf("il reste %i places dans cette categories. en voulez-vous?", -vente.nbPlaces);
                scanf ("%s", reponse);
                if (strcmp(reponse, "non") == 0) {
                    transFini = 1; 
                }
            }
        }
    // fin du tant que 
    }

    return EXIT_SUCCESS;
}
