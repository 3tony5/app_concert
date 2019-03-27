//J'ai changé le types places afin de pouvoir l'utilisé également pour les commandes. A VOIR ! 
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

places salle[3];

//ci-dessous l'application places qui va contabilisé le nombres de places disponibles pour l'application concert.
places compte_places(places place) {
    int i;
    places result;
    result.categorie = place.categorie;
    result.nbPlaces = 0;

    for ( i = 0; i > place.nbPlaces; i--) {

        salle[place.categorie - 1].nbPlaces--;
        result.nbPlaces--;
        if (salle[place.categorie - 1].nbPlaces == 0) {
            return result;
        }

    }
    return result;
}


int main (int argc, char **argv) {
    // On définit un tableau contenant les informations sur les 3 catégories de places
    places place;

    // Catégorie 1 : à proximité de la scène
    salle[0].categorie = 1;
    salle[0].nbPlaces = 50;


    // Catégorie 2 : fausse au centre de la salle
    salle[1].categorie = 2;
    salle[1].nbPlaces = 150;


    // Catégorie 3 : Tribunes supérieures
    salle[2].categorie = 3;
    salle[2].nbPlaces = 100;


    int socket_concert;
	struct sockaddr_in adresse_concert;
	int lgadresse_concert;
	int socket_RV;
   	struct sockaddr_in adresseRV;
	int lgadresseRV;
	struct hostent * hote_concert;
	unsigned short port;

	/*création de la socket RV*/
	if ((socket_RV = socket(AF_INET, SOCK_STREAM,0)) == -1)
	{
		perror("socket RV");
		exit(1);
	}

	/*preparation de l'adresse locale */
	
	adresseRV.sin_family = AF_INET;
	adresseRV.sin_port = htons((unsigned int)PORT_PLACES);
	adresseRV.sin_addr.s_addr = htonl(INADDR_ANY);
	lgadresseRV = sizeof(adresseRV);

	/* attachement de la socket a l'adresse locale */
	if ((bind(socket_RV, (struct sockaddr *)&adresseRV, lgadresseRV)) == -1)
	{
		perror("bind RV");
		exit(1);
	}

	/* declaration d'ouverture du service */
	if (listen(socket_RV, 10) == -1)
	{
		perror("listen RV");
		exit(2);
	}

	lgadresse_concert = sizeof(adresse_concert);

    // while 1
    while (1) {
        // On attend que concert se connecte à la socket
		socket_concert = accept(socket_RV, (struct sockaddr *) &adresse_concert, &lgadresse_concert);
		if (socket_concert == -1) {
			perror("places accept");
			exit(3);
		}
        else {
		    printf("Application concert connecté avec succès à places !!\n");
            fflush(stdout);
        }

        while (1) {
            //attend information transactions

        
            //lecture debut transaction
            if (read(socket_concert, &place, sizeof(places)) < 0) {
                perror("read places");
                exit(4);

            }
	printf("categories demandé %i, nb places demandé %i\n",place.categorie,place.nbPlaces);
            // si transactions>0
            if (place.nbPlaces > 0) {
                // places->categories =+ transactions
                salle[place.categorie-1].nbPlaces = salle[place.categorie-1].nbPlaces + place.nbPlaces;

                //envoie transactions à concert
                if (write(socket_concert, &place, sizeof(places)) != sizeof(places)) {
                    perror("write places");
                    exit(5);
                }
		printf("retour de transaction envoyé\n");
                
            //sinon pour i de 0 à transaction
            }
            else {
                // compte le nombre de place reservable 
                place = compte_places(place);
                // renvoie le nombre de reservé 
                if (write(socket_concert, &place, sizeof(places)) != sizeof(places)) {

                    perror("write");
                    exit(6);
                }
		printf("retour de transaction envoyé categories : %i places : %i \n",place.categorie,place.nbPlaces );
            //fin si
            }
        //fin while
        }
    
    //fin while connexion
    }

    return EXIT_SUCCESS;
}
