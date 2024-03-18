#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *Inserisci(void * s)
{

	int i;
	Elem v;


	for(i=0; i<4; i++) {

		v = rand() % 11;

		/* TBD: Effettuare un inserimento con StackPush() */
		StackPush(s, v);

		sleep(1);
	}

	pthread_exit(NULL);
}


void *Preleva(void * s)
{

	int i;
	Elem v1, v2;


	for(i=0; i<10; i++) {

		/* TBD: Prelevare con StackPop() in v1 */
		v1=StackPop(s);

		/* TBD: Prelevare con StackPop() in v2 */
		v2=StackPop(s);

		printf("Somma: %d\n", v1+v2);

		sleep(3);
	}

	pthread_exit(NULL);

}


int main(int argc, char *argv[])
{

	pthread_t threads[6];
	pthread_attr_t attr;
	int rc;
	int i;

	srand(time(NULL));


	Stack * stack = (Stack*) malloc(sizeof(Stack));/* TBD: Creare un oggetto stack */

	/* TBD: Inizializzare lo stack con StackInit(),
	 *      indicando "4" come dimensione massima */
	StackInit(stack, 4);


	for(i=0; i<5; i++) {

		/* TBD: Creare 5 thread tramite pthread_create(), facendogli
		 *      eseguire la funzione Inserisci(), e passandogli
		 *      l'oggetto stack come parametro puntatore */
		pthread_create(&threads[i], &attr, Inserisci, (void*)stack);
	}


	/* TBD: Creare un thread, facendogli eseguire la funzione Preleva(),
	 *      e passandogli l'oggetto stack come parametro puntatore */
    pthread_create(&threads[i], &attr, Preleva, (void*)stack);


	for(i=0; i<5; i++) {

		/* TBD: Effettuare la join con i thread figli
		 *      che hanno eseguito Inserisci() */
		pthread_join(threads[i], NULL);

	}


	/* TBD: Effettuare la join con il thread figlio
	 *      che ha eseguito Preleva() */
	pthread_join(threads[5], NULL);


	/* TBD: Disattivare l'oggetto stack con StackRemove() */
	StackRemove(stack);
}

