//ci-dessous l'application places qui va contabilisé le nombres de places disponibles pour l'application concert.
#include "gestionnaire_concert.h"

int main (){
    // On définit un tableau contenant les informations sur les 3 catégories de places
    places salle[3];

    // Catégorie 1 : à proximité de la scène
    salle[0].categorie = 1;
    salle[0].nbPlaces = 100;
    salle[0].dispo = 100;
    salle[0].prix = 50;

    // Catégorie 2 : fausse au centre de la salle
    salle[0].categorie = 2;
    salle[0].nbPlaces = 350;
    salle[0].dispo = 350;
    salle[0].prix = 30;

    // Catégorie 3 : Tribunes supérieures
    salle[0].categorie = 3;
    salle[0].nbPlaces = 220;
    salle[0].dispo = 210;
    salle[0].prix = 20;

// connexion à concert

// while 1

//attend information transactions

// si transactions>0

// places->categories =+ transactions

//envoie transactions à concert

//sinon pour i de 0 à transaction

//places->categories =- 1
// if places == 0 envoie transactions+i break?

//fin si
//fin pour
//fin while

return 0;
}
