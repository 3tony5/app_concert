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
    // etablir la connexion avec concert

    //tant que transaction non terminés.

    //demander un certain nombre de places pour une certaines categories

    // envoyer ces données a concert

    // récuperer le nombres de places autoriser

    //si nbautorisé = nbdemandé

    //demandé et envoyer code carte bleu

    // attendre ok de concert

    // fin de la transaction

    // Fin si sinon si nbautorisé =0 fin de la transaction

    // sinon présenté offres 

    // fin du tant que 


    return EXIT_SUCCESS;
}
