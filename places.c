//J'ai changé le types places afin de pouvoir l'utilisé également pour les commandes. A VOIR ! 


//ci-dessous l'application places qui va contabilisé le nombres de places disponibles pour l'application concert.
#include "gestionnaire_concert.h"
places compteplaces(places place) {
    int i;
    places result;
    result.categorie = place.categorie;
    result.nbPlaces=0;
    for ( i = 0; i < place.nbPlaces; i--)
    {
        salle[place.categorie-1]--;
        result.nbPlaces--;
        if(salle[place.categorie-1]==0 ){

            return result;
        }

    }
    return result;
}

places salle[3];

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
    while (true) {
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
        if(read(sock, &place, sizeof(places)) < 0) {
            perror("read places");
            exit(4);

        }

        // si transactions>0
        if (place.nbPlaces>0) {
            // places->categories =+ transactions
            salle[place.categories-1] = salle[place.categories-1] + place.nbPlaces;

            //envoie transactions à concert
            if (write(sock, place, sizeof(places)) != sizeof(places)) {
                perror("write places");
                exit(5);
            }
            
        //sinon pour i de 0 à transaction
        }
        else {
            // compte le nombre de place reservable 
            places = compteplaces(places);
            // renvoie le nombre de reservé 
            if (write(sock, place, sizeof(places)) != sizeof(places)) {

                perror("write");
                exit(6);
            }

        //fin si
        }
    
    //fin while
    }

    return EXIT_SUCCESS;
}
