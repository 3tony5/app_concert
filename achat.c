// ci dessous l'application achat qui va contenir les clients de l'application concert
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

int main(){
    int transFini=0;
    int codeBancaire,prix;
    places achat, vente;
    char  validation[200];
    // etablir la connexion avec concert

    //tant que transaction non terminés.
        while (transFini==0){


    //demander un certain nombre de places pour une certaines categories
    prinf("categories désirés 1,2 ou 3 ?");
    scanf("%i",&achat.categories);
    prinf("nombre de places désirés ?");
    scanf("%i",&achat.nbPlaces);

    // envoyer ces données a concert
    if (write(sock,achat,sizeof (places))!=sizeof(places)){

        perror("write");
        exit(3);
    }

    // récuperer le nombres de places autoriser
    if(read(sock,&vente,sizeof(places))<0)
        {
            perror("read");
            exit(3);

        }
    //si nbautorisé = nbdemandé
        if(vente.nbPlaces==achat.nbPlaces){

if(read(sock,&prix,sizeof(int))<0)
        {
            perror("read");
            exit(3);

        }
    //demandé et envoyer code carte bleu
            printf ("cela vous fera %i \n",prix);
            printf("veillez entrez votre code de carte bleu\n");
            scanf("%i",&codeBancaire);
             if (write(sock,codeBancaire,sizeof (places))!=sizeof(places)){

                 perror("write");
                 exit(3);
             }

    // attendre ok de concert
             if(read(sock,&validation,sizeof(char)*201)<0)
        {
            perror("read");
            exit(3);

        }
    // fin de la transaction
        transFini=1;
    // Fin si sinon si nbautorisé =0 fin de la transaction
        } 
        if(vente.nbPlaces==0){
            printf("plus de places dans la categories demandé. \n");
            transFini=1;
        }
    // sinon présenté offres 
        printf("il reste %i places dans cette categories. en voulez-vous?");
        scanf ("%s", reponse);
        if( strcmp(reponse,"non")==0){
           transFini=1; 
        }
    // fin du tant que 
        }

    return EXIT_SUCCESS;
}
