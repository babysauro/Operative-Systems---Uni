#ifndef _AGGREGATORE_H_
#define _AGGREGATORE_H_

#include <pthread.h>

typedef struct {

    int variabile;

    /* TBD: Completare la struttura dati monitor */
    int num_lettori;
    pthread_mutex_t mutex;
    pthread_cond_t cv_scrittore;

} MonitorLS;

/*Fatto da me*/
typedef struct {
    MonitorLS * m;
    int id_coda;
}Parametri;

void aggregatore(int id_coda_sensore, int id_code_collettori[3]);
void * thread_lettore(void *);
void * thread_scrittore(void *);
void lettura(MonitorLS *, int * valore);
void scrittura(MonitorLS *, int valore);

#endif