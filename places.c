//J'ai changé le types places afin de pouvoir l'utilisé également pour les commandes. A VOIR ! 
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <netinet/ip.h> 
#include <pthread.h>
#include <sched.h>
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
    for ( i = 0; i < place.nbPlaces; i--) {
        salle[place.categorie - 1].nbPlaces--;
        result.nbPlaces--;
        if (salle[place.categorie - 1].nbPlaces == 0) {
            return result;
        }

    }
    return result;
}


int main () {
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


    // connexion à concert
    int socket_places;
    struct sockaddr_in adresse_places;
    int lgadresse_places;
   
    if ((socket_places = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("creation socket places");
        exit(1);
    }
	
	adresse_places.sin_family = AF_INET;
	adresse_places.sin_port = htons(PORT_PLACES);
	adresse_places.sin_addr.s_addr = htonl(INADDR_ANY);
	
	lgadresse_places = sizeof(adresse_places);
	
	if ((bind(socket_places, (struct sockaddr *) &adresse_places, lgadresse_places)) == -1)
	{
	  	perror("bind places");
	  	exit(2);
	}

    // while 1
    while (1) {
        // On attend que concert se connecte à la socket
		int accept_concert = accept(socket_places, (struct sockaddr *) &adresse_places, &lgadresse_places);
		if (accept_concert == -1) {
			perror("places accept");
			exit(3);
		}
        else {
		    printf("Application places connecté avec succès à concert !!\n");
        }
        //attend information transactions

    
        //lecture debut transaction
        if (read(socket_places, &place, sizeof(places)) < 0) {
            perror("read places");
            exit(4);

        }

        // si transactions>0
        if (place.nbPlaces > 0) {
            // places->categories =+ transactions
            salle[place.categorie-1].nbPlaces = salle[place.categorie-1].nbPlaces + place.nbPlaces;

            //envoie transactions à concert
            if (write(socket_places, &place, sizeof(places)) != sizeof(places)) {
                perror("write places");
                exit(5);
            }
            
        //sinon pour i de 0 à transaction
        }
        else {
            // compte le nombre de place reservable 
            place = compte_places(place);
            // renvoie le nombre de reservé 
            if (write(socket_places, &place, sizeof(places)) != sizeof(places)) {

                perror("write");
                exit(6);
            }

        //fin si
        }
    
    //fin while
    }

    return EXIT_SUCCESS;
}
