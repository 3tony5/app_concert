// ci dessous l'application concert qui va faire le serveur du projet.
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
// fonctionnement du thread
// le thread possede en parametre le numero de la sock du client achat.c
//entete
// initialise/paretre connexion

//while transaction non fini

// lit données achat

// envoie des données à places (nb places - nb déja réservé)

//lit la réponse de places

//envoie la réponse de places à achat;

//si nb places demandé == nbplaces reservé

//envoie prix 

//attend code carte bleu

//envoie ok
// reset nb places reservé à 0

//fin transaction

//fin si

//si nb places réservé ==0

// fin transaction

// fin si sinon

// fin while

// rend le nombre de reservé a places.c





int main(int argc, char **argv) {

	int pere=0;
	fini=0;
	int socket_PLACES;
	struct sockaddr_in adresse_places;
	struct hostent *hote;
 	places places,reserve, areserve,places_res;
char textvalidation [200];
strcpy(textvalidation,"ok");

	printf("creation de la socket locale");
	if ((socket_PLACES = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("creation socket locale");
		exit(3);
	}

	adresse_places.sin_family = AF_INET;
	adresse_places.sin_port = htons(PORT_PLACES);
	//recuperer l'hote
	//bcopy(hote->h_addr, &adresse_places.sin_addr, hote->h_length);
	printf("L'adresse en notation pointee est : %s\n", inet_ntoa(adresse_places.sin_addr));
	fflush(stdout);

	if (connect(socket_places, (struct sockaddr *) &adresse_places, sizeof(adresse_places)) == -1) {
		perror("Connexion avec le socket places");
		exit(4);
	}
	// mise en place de la socket concert
	int socket_concert;
	struct sockaddr_in adresse_concert;
	int lgadresse_concert;
   	
	while( pere==0){

	if ((socket_CONCERT = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("creation socket concert");
		exit(1);
	}

	adresse_concert.sin_family = AF_INET;
	adresse_concert.sin_port = htons(PORT_CONCERT);
	adresse_concert.sin_addr.s_addr = htonl(INADDR_ANY);

	lgadresse_concert = sizeof(adresse_concert);

	if ((bind(socket_concert, (struct sockaddr *) &adresse_concert, lgadresse_concert)) == -1) {
		perror("bind concert");
	  	exit(2);
	}






	// -------------------------------------------------
	// mise en place de la socket pour connexion à places
    

	
	/* la connexion à été accepté*/
	if (fork()!=0){
		pere=1;
	} 

//while transaction non fini
while(fini ==0){
// lit données achat
 if(read(socket_concert,&places,sizeof(places))<0)
        {
            perror("read");
            exit(3);

        }
// envoie des données à places (nb places - nb déja réservé)
areserve.nbPlaces=places.nbPlaces - reserve.nbPlaces;// est negatif si reservation de places, positif si liberation de places
 if (write(socket_PLACES,areserve,sizeof (int))!=sizeof(places)){

        perror("write");
        exit(3);
    }

//lit la réponse de places
 if(read(socket_PLACES,&places_res,sizeof(places))<0)
        {
            perror("read");
            exit(3);

        }
//envoie la réponse de places à achat;
reserve.nbPlaces=reserve.nbPlaces+ places_res.nbPlaces;
//si nb places demandé == nbplaces reservé
if(nb_places == nb_reserve){
//envoie prix 

//attend code carte bleu
 if(read(socket_concert,&places,sizeof(places))<0)
        {
            perror("read");
            exit(3);

        }
//envoie ok
 if (write(socket_concert,textevalidation,sizeof (int))!=sizeof(places)){

        perror("write");
        exit(3);
    }

//fin transaction
fini=1;
//fin si
}
//si nb places réservé ==0
if (resereve.nbPlaces == 0){


// fin transaction
fini=1;
// fin si sinon
}
// fin while
}
// rend le nombre de reservé a places.c
reserve.nbPlaces= 0- reserve.nbPlaces;
 if (write(socket_PLACES,reserve,sizeof (int))!=sizeof(places)){

        perror("write");
        exit(3);
    }


	}
	
	return EXIT_SUCCESS;
}
