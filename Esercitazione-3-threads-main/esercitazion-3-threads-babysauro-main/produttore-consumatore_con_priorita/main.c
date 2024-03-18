#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"


void * produttore_alta_priorita(void * p)
{
	int i;

	PriorityProdCons * prodcons = (PriorityProdCons *) p;
	    
    
	for(i=0; i<3; i++) {

		/* TBD: Chiamare il metodo di produzione alta priorità */
		produci_alta_prio(prodcons);

		sleep(2);
	}

	pthread_exit(NULL);
}


void * produttore_bassa_priorita(void * p)
{
	int i;

    PriorityProdCons * prodcons = (PriorityProdCons *) p;    
    
	for(i=0; i<3; i++) {
        
		/* TBD: Chiamare il metodo di produzione bassa priorità */
		produci_bassa_prio(prodcons);

		sleep(1);
	}
    
	pthread_exit(NULL);
}


void * consumatore(void * p)
{
    
	int i;

    PriorityProdCons * prodcons = (PriorityProdCons *) p;
    
	for(i=0; i<12; i++) {
        
		/* TBD: Chiamare il metodo consuma() */
		consuma(prodcons);
        
		sleep(1);
	}
    
	pthread_exit(NULL);
}




int main(int argc, char *argv[])
{
	


	pthread_t threads[5];
	pthread_attr_t attr;
    
	int rc;
	int i;

	srand(time(NULL));

    
	PriorityProdCons * prodcons = (PriorityProdCons *) malloc (sizeof(PriorityProdCons)); /* TBD: Allocare un oggetto monitor */
    
    
	/* TBD: Inizializzare l'oggetto monitor */
	inizializza_prod_cons(prodcons);


	/* TBD: Creare 3 thread produttore bassa priorità, 
	 *      1 thread produttore alta priorità,
	 *      e 1 thread consumatore.
	 */
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//Thread produttore bassa priorità
	for (int i=0; i<3; i++)
	 pthread_create(&threads[i], &attr, produttore_bassa_priorita, (void*)prodcons);

	
	//THread produttore alta priorità
	pthread_create(&threads[3], &attr, produttore_alta_priorita, (void*)prodcons);

	//Thread consumatore
	pthread_create(&threads[4], &attr, consumatore, (void*) prodcons);
    
	/* TBD: Effettuare la join con i thread */
    //Sono 5 thread
	for (int i=0; i<5; i++)
	pthread_join(threads[i], NULL);

	/* TBD: Richiamare il distrutture dell'oggetto monitor */
	rimuovi_prod_cons(prodcons);
}

