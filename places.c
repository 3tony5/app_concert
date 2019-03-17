//ci-dessous l'application places qui va contabilisé le nombres de places disponibles pour l'application concert.
#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "gestionnaire_concert.h"

int main (){
    // On définit un tableau contenant les informations sur les 3 catégories de places
    places salle[3];

    // Catégorie 1 : à proximité de la scène
    salle[0].categorie = 1;
    salle[0].nbPlaces = 50;
    salle[0].dispo = 50;
    salle[0].prix = 50;

    // Catégorie 2 : fausse au centre de la salle
    salle[1].categorie = 2;
    salle[1].nbPlaces = 150;
    salle[1].dispo = 150;
    salle[1].prix = 30;

    // Catégorie 3 : Tribunes supérieures
    salle[2].categorie = 3;
    salle[2].nbPlaces = 100;
    salle[2].dispo = 100;
    salle[2].prix = 20;

    // mise en place de la socket pour connexion avec concert
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
	
	lgadresse_places = sizeof(adressePLACES);
	
	if ((bind(socket_places, (struct sockaddr *) &adresse_places, lgadresse_places)) == -1)
	{
	  	perror("bind places");
	  	exit(2);
	}

    while (true)
    {
        //attend information transactions

        // si transactions>0

        // places->categories =+ transactions

        //envoie transactions à concert

        //sinon pour i de 0 à transaction

        //places->categories =- 1
        // if places == 0 envoie transactions+i break?

        //fin si
        //fin pour

    }

    return EXIT_SUCCESS;
}
