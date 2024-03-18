#ifndef __HEADER__
#define __HEADER__

#include <pthread.h>

typedef int Elem;

typedef struct Stack {
	Elem * dati;	// puntatore ad un vettore dinamico di elementi
	int dim;		// dimensione dello Stack
	int testa;

	/* TBD: Aggiungere variabili per la sincronizzazione */
	pthread_mutex_t mutex;
	pthread_cond_t push;
	pthread_cond_t pop;
	

} Stack;

void StackInit(Stack * s, int dim);	// alloca dinamicamente "dim" elementi
void StackRemove(Stack * s);		// dealloca gli elementi
void StackPush(Stack * s, Elem e);	// inserimento nello Stack
Elem StackPop(Stack * s);		// estrazione dallo Stack
int StackSize(Stack * s);		// ritorna il numero attuale di elementi


#endif
