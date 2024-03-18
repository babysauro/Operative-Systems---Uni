#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>


struct monitor {

	int stazione;
	int id_treno;

    //Altre variabili
	int num_cv_scrittori;
	int num_scrittori;
	int stato;
	int num_cv_lettori;
	int num_lettori;

	/* TBD: Aggiungere ulteriori variabili per la sincronizzazione */
	pthread_mutex_t mutex;
	pthread_cond_t aggiorna;
	pthread_cond_t read;
	
};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void scrivi_stazione(struct monitor * m, int stazione);
int leggi_stazione(struct monitor * m);

#define OCCUPATO 1
#define LIBERO 0


#endif
