/*
 * AUDUREAU Jules e21806010
 * AUPIAIS Anthony e21808738
 * Bibliotheque gestionnaire_concert.h
 */

typedef struct places
{
    int categorie;
    int nbPlaces;
    int nbEtudiants;
    int dispo;
    int prix;
} places;

#define REDUCTION_ETUDIANTE 0.8
#define PORT_PLACES 4567
#define PORT_CONCERT 4568