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

int main(int argc, char **argv)
{
	// mise en place de la socket pour connexion à concert
    int socket_PLACES;
	struct sockaddr_in adresse_places;
	struct hostent *hote;

	printf("creation de la socket locale");
	if ((socket_PLACES = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("creation socket locale");
		exit(1);
	}

	adresse_places.sin_family = AF_INET;
	adresse_places.sin_port = htons(PORT_PLACES);
	//recuperer l'hote
	//bcopy(hote->h_addr, &adresse_places.sin_addr, hote->h_length);
	printf("L'adresse en notation pointee est : %s\n", inet_ntoa(adresse_places.sin_addr));
	fflush(stdout);

	if (connect(socket_places, (struct sockaddr *) &adresse_places, sizeof(adresse_places))==-1)
	{
		perror("Connexion avec le socket places");
		exit(2);
	}

	return EXIT_SUCCESS;
}