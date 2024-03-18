#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"


void inizializza_prod_cons(PriorityProdCons* p){

    /* TBD: Inizializzare tutte le variabili del monitor */
    pthread_mutex_init(&p->mutex, NULL);
    pthread_cond_init(&p->no_pieno_1, NULL);
    pthread_cond_init(&p->no_pieno_2, NULL);
    pthread_cond_init(&p->no_vuoto, NULL);
    
    p->threads_prio_1 = 0;
    p->threads_prio_2 = 0;
    p->count = 0;
    p->coda = 0;
    p->testa = 0;
    //Devo inizializzare il buffer!!

}

void rimuovi_prod_cons(PriorityProdCons* p){

    /* TBD: Disattivare mutex e variabili condition */
    pthread_mutex_destroy(&p->mutex);
    pthread_cond_destroy(&p->no_pieno_1);
    pthread_cond_destroy(&p->no_pieno_2);
    pthread_cond_destroy(&p->no_vuoto);

}

void produci_alta_prio(PriorityProdCons* p){

	int value;


    /* TBD: Implementare la sincronizzazione secondo lo schema del
     *      produttore-consumatore con vettore di buffer circolare.
     *
     *      Si introduca nel monitor una variabile "threads_prio_1"
     *      per contare il numero di produttori sospesi ad alta priorità.
     */

     //Ingresso nel monitor
     pthread_mutex_lock(&p->mutex);

     //Sospendere il porcesso se il vettore di buffer è pieno (usare while)
     while (p->count == DIM)
     {
        p->threads_prio_1 = p->threads_prio_1+1;

        //La produzione di alta priorità è in attesa
        printf("Attesa produttore alta priorità\n");

        pthread_cond_wait(&p->no_pieno_1, &p->mutex);

        //La produzione di alta priorità si risveglia
        printf("Risvegliio produttore alta priorità\n");

        p->threads_prio_1 = p->threads_prio_1-1;
     }


	value = rand() % 12;

    //Produzione
    p->buffer[p->testa] = value;
    p->testa = (p->testa+1) % DIM; //Incremento della testa

    p->count = p->count+1; //Incremento del contatore

    printf("Produzione priorità alta: %d\n", value);

    //Svegliare consumatore in attesa (se c'è)
    pthread_cond_signal(&p->no_vuoto);

    //Uscita dal monitor 
    pthread_mutex_unlock(&p->mutex);

}




void produci_bassa_prio(PriorityProdCons* p){

    int value;

    /* TBD: Implementare la sincronizzazione secondo lo schema del
     *      produttore-consumatore con vettore di buffer circolare.
	 *
	 *      I produttori a bassa priorità devono sospendersi sia nel caso di vettore
	 *      di buffer già pieno, sia nel caso vi siano produttori ad alta priorità già
	 *      in attesa.
     *
     *      Si introduca nel monitor una variabile "threads_prio_2"
     *      per contare il numero di produttori sospesi a bassa priorità.
     */

    //Entrata nel monitor
    pthread_mutex_lock(&p->mutex);

    //Sospendere nel caso il buffer sia pieno e se ci sono produttori di alta priorità in attesa
    while (p->count == DIM || p->threads_prio_1 > 0)
    {
      p->threads_prio_2 = p->threads_prio_2+1;

      //Il produttore a bassa priorità è in attesa
      printf("Attesa produttore a bassa priorità\n");

      pthread_cond_wait(&p->no_pieno_2, &p->mutex);

      //Il produttore a bassa priorità si sveglia
      printf("Risveglio produttore a bassa priorità\n");

      p->threads_prio_2 = p->threads_prio_2-1;
    }

	value = 13 + (rand() % 12);

    //Produzione
    p->buffer[p->testa] = value;
    p->testa = (p->testa +1) % DIM; //Incremento testa
    p->count = p->count+1;  //Incremento contatore

    printf("Produzione priorità bassa: %d\n", value);

    //Svegliare consumatore in attesa (se c'è)
    pthread_cond_signal(&p->no_vuoto);

    //Uscita dal monitor
    pthread_mutex_unlock(&p->mutex);

}


void consuma(PriorityProdCons* p){

	int value;


    /* TBD: Implementare la sincronizzazione secondo lo schema del
     *      produttore-consumatore con vettore di buffer circolare.
     */

    //Entrata nel monitor
    pthread_mutex_lock(&p->mutex);

    //Il buffer non deve essere VUOTO
    while(p->count == 0)
    {
        //Il consumatore deve stare in attesa
        printf("Attesa consumatore\n");

        pthread_cond_wait(&p->no_vuoto, &p->mutex);

        //Il consumatore si può svegliare
        printf("Risveglio consumatore\n");

    }

    /* Consumazione */

	value = /* TBD */ p->buffer[p->coda];
    p->coda = (p->coda+1) % DIM; //Incremento coda
    p->count = p->count-1; //Decremento contatore

	printf("Consumato valore %d\n", value);

//Signal condition     
if (p->threads_prio_1>0){
    pthread_cond_signal(&p->no_pieno_1);
}
else if (&p->no_pieno_2>0){
    pthread_cond_signal(&p->no_pieno_2);
}

    //Uscita dal monitor
    pthread_mutex_unlock(&p->mutex);


}
