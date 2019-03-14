// ci dessous l'application concert qui va faire le serveur du projet.

#include "gestionnaire_concert.h"

int main(int argc, char **argv)
{
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