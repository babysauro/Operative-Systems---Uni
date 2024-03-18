#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include <pthread.h>

#define DIM 3

typedef struct{

	int buffer[DIM];

	int threads_prio_1;
	int threads_prio_2;
	int count; 
	int testa;
	int coda;

	/* TBD: Aggiungere ulteriori variabili per la sincronizzazione */
	pthread_mutex_t mutex;
	pthread_cond_t no_pieno_1;
	pthread_cond_t no_pieno_2;
	pthread_cond_t no_vuoto;
    
} PriorityProdCons;


void inizializza_prod_cons(PriorityProdCons * p);
void produci_alta_prio(PriorityProdCons * p);
void produci_bassa_prio(PriorityProdCons * p);
void consuma(PriorityProdCons * p);
void rimuovi_prod_cons(PriorityProdCons * p);

#endif
