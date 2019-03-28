// ci dessous l'application concert qui va faire le serveur du projet.
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
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

	int fils = 0;
	int fini = 0;
	int tarif[3];
	tarif[0] = 50;
	tarif[1] = 30;
	tarif[2] = 20;
	int prix;
	places places, reserve, areserve, places_res;
	reserve.nbPlaces=0;
	char textvalidation[200];
	strcpy(textvalidation, "ok");

	// mise en place de la socket places
	int socket_places;
	struct sockaddr_in adresse_places;
	struct hostent *hote;
 	
	// On verifie que l'administrateur saisi l'hote du serveur places
    if (argc != 2)
	{
		fprintf(stderr,"usage: %s places_hostname\n", argv[0]);
		return(EXIT_FAILURE);
	}

	if ((socket_places = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket places");
		exit(1);
	}
	printf("Création de la socket places\n");
	fflush(stdout);

	/* Récuperation de l'adresse IP du serveur (a partir de son nom) */
    if ((hote = gethostbyname(argv[1])) == NULL)
	{
        perror("gethostbyname");
        exit(2);
    }

	adresse_places.sin_family = AF_INET;
	adresse_places.sin_port = htons((unsigned int)PORT_PLACES);
	bcopy(hote->h_addr, &adresse_places.sin_addr, hote->h_length);

	if (connect(socket_places, (struct sockaddr *)&adresse_places, sizeof(adresse_places)) == -1)
	{
		perror("Connexion avec le socket places");
		exit(3);
	}

	// mise en place de la socket concert
	int socket_concert;
	struct sockaddr_in adresse_concert;
	int lgadresse_concert;
	int socket_RV;
   	struct sockaddr_in adresseRV;
	int lgadresseRV;

	// création de la socket RV
	if ((socket_RV = socket(AF_INET, SOCK_STREAM,0)) == -1)
	{
		perror("socket RV");
		exit(4);
	}

	// preparation de l'adresse locale
	adresseRV.sin_family = AF_INET;
	adresseRV.sin_port = htons((unsigned int)PORT_CONCERT);
	adresseRV.sin_addr.s_addr = htonl(INADDR_ANY);
	lgadresseRV = sizeof(adresseRV);

	// attachement de la socket a l'adresse locale
	if ((bind(socket_RV, (struct sockaddr *)&adresseRV, lgadresseRV)) == -1)
	{
		perror("bind RV");
		exit(5);
	}
	// declaration d'ouverture du service
	if (listen(socket_RV, 10) == -1)
	{
		perror("listen RV");
		exit(6);
	}


	// boucle d'attente de connexion
	while (fils == 0) {
		
		/* attente d'un client */
		lgadresse_concert = sizeof(adresse_concert);
		socket_concert = accept(socket_RV, (struct sockaddr *) &adresse_concert, &lgadresse_concert);
		if (socket_concert == -1  && errno == EINTR)
		{
			/* Reception d'un signal */
			continue;
		}
		if (socket_concert == -1)
		{  /* erreur */
			perror("accept concert");
			exit(7);
		}
		/* un client est arrivé : il est connecté via la socket de service */
		printf("Connexion acceptée !!\n");
		fflush(stdout);
		

		/* la connexion à été acceptée */
		int fk = fork();
		if (fk == -1){
			perror("fork");
			exit (8);
		}
		if (fk == 0) {
			fils = 1;
			//while transaction non finie
			while(fini == 0)
			{
				printf("debut transaction\n");
				// lit données achat
				do
				{
					if (read(socket_concert, &places, sizeof(places)) < 0)
					{
						perror("read concert places");
						exit(9);
					}
				}
				while (places.nbPlaces == 0);
				
				
				// envoie des données à places (nb places - nb déja réservé)
				areserve.nbPlaces = places.nbPlaces - reserve.nbPlaces;// est negatif si reservation de places, positif si liberation de places
				areserve.categorie = places.categorie;
				if (write(socket_places, &areserve, sizeof(places)) != sizeof(places))
				{
					perror("write places place");
					exit(10);
				}

				// lit la réponse de places
				if (read(socket_places, &places_res, sizeof(places)) < 0)
				{
					perror("read places reponse place");
					exit(11);
				}
			
					
				printf(">>Lecture de la réponse:\n\tCatégorie demandée %i\n\tNombre de places demandées %i\n\tDont %i étudiants\n", places_res.categorie, places_res.nbPlaces, places.nbEtudiants);
				fflush(stdout);
				
				// envoie la réponse de places à achat;
				reserve.nbPlaces = reserve.nbPlaces + places_res.nbPlaces;
				
				if (write(socket_concert, &reserve, sizeof(places)) != sizeof(places))
				{
					perror("write concert reponse place");
					exit(12);
				}

				// si nb places demandé == nbplaces reservé
				if (places.nbPlaces == reserve.nbPlaces) {
					// envoie prix sur la socket concert
					prix = (tarif[places.categorie - 1] * -(places.nbPlaces - places.nbEtudiants)) + (tarif[places.categorie - 1] * -places.nbEtudiants * REDUCTION_ETUDIANTE);
					if (write(socket_concert, &prix, sizeof(int)) != sizeof(int))
					{
						perror("write concert prix");
						exit(13);
					}

					// attend code carte bleu
					if (read(socket_concert, &places, sizeof(places)) < 0)
					{
						perror("read concert cb");
						exit(14);
					}
					
					// envoie ok
					if (write(socket_concert, textvalidation, strlen(textvalidation)+1) != strlen(textvalidation)+1)
					{
						perror("write concert validation");
						exit(15);
					}
					printf("Réservation acceptée et payé pour %i\n", places.nbPlaces);

					// fin transaction
					fini = 1;

				// fin si
				}
				else
				{
					reserve.nbPlaces = 0;
					areserve.nbPlaces = 0;
					places.nbPlaces = 0;
					places_res.nbPlaces = 0;
					
					printf("remise à zero\n");
				}

				//si nb places réservé == 0
				/*if (reserve.nbPlaces == 0)
				{
					// fin transaction
					fini=1;
				// fin si sinon
				}*/
			// fin while
			}

			// rend le nombre de reservé a places.c
			/*reserve.nbPlaces = 0 - reserve.nbPlaces;
			if (write(socket_places, &reserve.nbPlaces, sizeof(places)) != sizeof(places)) {
				perror("write places rendre places");
				exit(16);
			}
			*/
				
		}
		else
		{
			close(socket_concert);
		}
	}
	
	
	return EXIT_SUCCESS;
}
