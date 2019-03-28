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

int main(int argc, char **argv)
{
    int transFini = 0;
    int codeBancaire, prix;
    places achat, vente;
    char  validation[200], reponse[5];
    int sock;
    struct sockaddr_in adresse_concert;
    struct hostent *hote;

    // On verifie que l'utilisateur saisi l'hote du serveur concert
    if (argc != 2)
	{
		fprintf(stderr, "usage: %s concert_hostname\n", argv[0]);
		return(EXIT_FAILURE);
	}

    /* Etablir la connexion avec concert */

    // creation de la socket locale
    if ((sock = socket(AF_INET, SOCK_STREAM,0)) == -1) {
        perror("socket");
        exit(1);
    }

    // recuperation de l'adresse IP du serveur (a partir de son nom)
    if ((hote = gethostbyname(argv[1])) == NULL)
    {
        perror("gethostbyname");
        exit(2);
    }

    // preparation de l'adresse du serveur
    adresse_concert.sin_family = AF_INET;
    adresse_concert.sin_port = htons((unsigned int)PORT_CONCERT);
    bcopy(hote->h_addr, &adresse_concert.sin_addr, hote->h_length);

    // demande de connexion au serveur
    if (connect(sock, (struct sockaddr *)&adresse_concert, sizeof(adresse_concert)) == -1 )
    {
        perror("connect concert");
        exit(3);
    }
    // le serveur a accepte la connexion
    printf("Connexion acceptée !!\n");
    fflush(stdout);


    /* Debut du traitement des données */

    //tant que transaction non terminés.
    while (transFini == 0) {
	printf("debut de transaction\n");
        //demander un certain nombre de places pour une certaine catégorie
        do
        {
            printf("Nombre de places désirées ?\n");
            scanf("%i", &achat.nbPlaces);
        }
        while (achat.nbPlaces < 0);
        do
        {
            printf("Catégorie désirée 1, 2 ou 3 ?\n");
            scanf("%i", &(achat.categorie));
        }
        while (achat.categorie != 1 && achat.categorie != 2 && achat.categorie != 3);
        do
        {
            printf("Nombre de places étudiante ?\n");
            scanf("%i", &achat.nbEtudiants);
        }
        while ((achat.nbEtudiants < 0) || (achat.nbEtudiants > achat.nbPlaces));
	    achat.nbPlaces = -achat.nbPlaces;
        achat .nbEtudiants = -achat.nbEtudiants;
		
        // envoyer ces données a concert
        if (write(sock, &achat, sizeof(places)) != sizeof(places))
        {
            perror("write concert places");
            exit(4);
        }

        // récupere le nombres de places autorisée
        if (read(sock, &vente, sizeof(places)) < 0)
        {
            perror("read concert places");
            exit(5);
        }
        //si nbautorisé = nbdemandé
        if (vente.nbPlaces == achat.nbPlaces){

            if (read(sock, &prix, sizeof(int)) < 0) {
                perror("read concert prix");
                exit(6);
            }

            //demandé et envoyer code carte bleu
            printf ("Cela vous coutera %i €\n", prix);
            printf("Veuillez entrer votre code de carte bancaire pour procéder au paiement\n");
            scanf("%i", &codeBancaire);
            if (write(sock, &codeBancaire, sizeof(places)) != sizeof(places))
            {
                perror("write concert cb");
                exit(7);
            }

            // attendre ok de concert
            if (read(sock, &validation, sizeof(char)*201) < 0)
            {
                perror("read concert validation");
                exit(8);
            }
            if (strcmp (validation,"ok") == 0){
                printf("Achat réussi avec succès !!\n");
            }
            // fin de la transaction
            transFini = 1;
        // Fin si sinon si nbautorisé = 0 fin de la transaction
        }
        else
        {
			achat.categorie = -1;
			achat.nbPlaces = -1;
			achat.nbEtudiants = -1;
            if (vente.nbPlaces == 0)
            {
                printf("Nombre de places dans la categorie demandée insuffisant. \n");
                transFini = 1;
            }
            // sinon présentation des autres offres 
            else
            {	
				do
				{
		            printf("Il reste %i places dans cette categorie. En voulez-vous? (oui, non)", -vente.nbPlaces);
		            scanf ("%s", reponse);
		            if (strcmp(reponse, "non") == 0)
		            {
		                transFini = 1; 
		            }
				}
				while ((strcmp(reponse, "non") == 0) && (strcmp(reponse, "oui") == 0));
            }
        }
    // fin du tant que 
	
    }
close (sock);
    return EXIT_SUCCESS;
}
