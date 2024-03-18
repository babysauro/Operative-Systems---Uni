#include "sensore.h"
#include "aggregatore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

void aggregatore(int id_coda_sensore, int id_code_collettori[3]) {

    printf("Avvio processo aggregatore...\n");


    /* TBD: Completare questa funzione, avviando un thread scrittore e 3 thread lettori *
     *
     *      Nota: occorre passare ai thread sia il puntatore ad un oggetto-monitor,
     *      sia l'identificativo della coda di messaggi su cui ricevere/inviare.
     */
    int ret;

    pthread_t lettori[3];
    pthread_t scrittore;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    
    //Monitor
    MonitorLS * ml =(MonitorLS*)malloc (sizeof(MonitorLS));

    ml->num_lettori = 0;
    pthread_mutex_init(&ml->mutex, NULL);
    pthread_cond_init(&ml->cv_scrittore, NULL);

 

    //Lettori
    Parametri*p_lettori[3];
    for (int i=0; i<3; i++)
    {
        p_lettori[i] = malloc(sizeof(Parametri));
        p_lettori[i]->id_coda = id_code_collettori[i];
        p_lettori[i]->m = ml;

        ret = pthread_create(&lettori[i], &attr, thread_lettore, (void*)p_lettori[i]);

        if (ret<0)
        {
            perror("ERRORE pthread_create lettore");
            exit(1);
        }
    }

    //Scrittore
    Parametri*p_scrittore = malloc(sizeof(Parametri));
    p_scrittore->id_coda = id_coda_sensore;
    p_scrittore->m = ml;

    ret = pthread_create(&scrittore, &attr, thread_scrittore, p_scrittore);

    if (ret<0)
    {
        perror("ERRORE pthread_create scrittore");
        exit(1);
    }

    //Join dei thread
    pthread_join(scrittore, NULL);

    for (int i=0; i<3; i++)
        pthread_join(lettori[i], NULL);


    //Free
    free(p_scrittore);

    for(int i=0; i<3; i++)
        free(p_lettori[i]);
    
}


void * thread_lettore(void * x) {

    /*Fatto da me*/
    Parametri* dati = (Parametri*) x;

    for(int i=0; i<10; i++) {

        int valore;

        sleep(1);

        /* TBD: Chiamare il metodo "lettura()" del monitor */ 
        lettura(dati->m, &valore);

        printf("Aggregatore: Invio valore=%d\n", valore);

        /* TBD: Inviare il messaggio */
        messaggio msg;
        msg.valore = valore;
        msg.tipo = TIPO;

        int ret = msgsnd(dati->id_coda, &msg, sizeof(messaggio)-sizeof(long), 0);

        if (ret<0)
        {
            perror("ERRORE send lettura");
            exit(1);
        }

    }


    pthread_exit(NULL);
}

void * thread_scrittore(void * x) {

    /*Fatto da me*/
    Parametri* dati = (Parametri*) x;

    for(int i=0; i<10; i++) {

        printf("Aggregatore: In attesa di messaggi...\n");

        /* TBD: Ricevere il messaggio */
        int ret;
        messaggio ric;
        ret = msgrcv(dati->id_coda, &ric, sizeof(messaggio)-sizeof(long), 0, 0);

        if (ret<0)
        {
            perror("ERRORE msgrcv aggregatore");
            exit(1);
        }

        int valore = ric.valore; /* TBD */

        printf("Aggregatore: Ricevuto valore=%d\n", valore);

        /* TBD: Chiamare il metodo "scrittura()" del monitor */
        scrittura(dati->m, valore);
    }

    pthread_exit(NULL);
}

void lettura(MonitorLS * m, int * valore) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    m->num_lettori ++;
    pthread_mutex_unlock(&m->mutex);

    *valore = m->variabile;

    printf("Aggregatore: Lettura valore=%d\n", *valore);

    /*Fatto da me*/
    pthread_mutex_lock(&m->mutex);
    m->num_lettori --;

    if (m->num_lettori == 0)
        pthread_cond_signal(&m->cv_scrittore);

    pthread_mutex_unlock(&m->mutex);

}

void scrittura(MonitorLS * m, int valore) {
    
    /* TBD: Completare il metodo, con la sincronizzazione */
    pthread_mutex_lock(&m->mutex);

    while (m->num_lettori>0)
    {
        pthread_cond_wait(&m->cv_scrittore, &m->mutex);
    }

    printf("Aggregatore: Scrittura valore=%d\n", valore);

    m->variabile = valore;

    /*Fatto da me*/
    pthread_mutex_unlock(&m->mutex);
}