#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

/* Il thread padre condivide l'id della coda 
   delle risposte con i figli, tramite una 
   variabile globale */

int id_coda_risposte;

pthread_mutex_t mutex;

void server(int id_c, int id_s){

	int v;
	int ret;

	id_coda_risposte = id_s;


	/*Fatto da me*/
	Client cl;
	Server sv;
	sv.typeServer = getpid();
	pthread_t thread_id;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_mutex_init(&mutex, NULL);
	


	while(1){

		/* TBD: Ricevere un messaggio di richiesta dal client */
		Client *v = (Client *) malloc (sizeof(Client));


		ret = msgrcv(id_c, &cl, sizeof(Client)-sizeof(long), 0, 0); /* TBD */
        
		if(ret < 0) {
			perror("Errore ricezione richiesta server");
			exit(1);
		}

		if(cl.val1 /* TBD */ == -1 && cl.val2 /* TBD */ == -1){

			/* Il processo esce quando si riceve un messaggio di terminazione,
			   con i valori {-1, -1}
			 */
			exit(0);
		}

		/* TBD: Avviare un thread figlio per l'elaborazione del messaggio,
				passandogli una **copia sullo heap** del messaggio ricevuto.
				(ogni thread figlio deve elaborare un messaggio diverso)
		 */
		
        
		pthread_create(&thread_id, &attr, Prodotto, (void*)v); 
		v->val1 = cl.val1; 
		v->val2 = cl.val2;
		v->typeClient = cl.typeClient;
	}
	pthread_mutex_destroy(&mutex);
}



void* Prodotto(void* v){
	
	int ret;

	/*Fatto da me*/
	Client* dati = (Client*)v;

	int v3 = (dati->val1)*(dati->val2); /* TBD: Calcolare il prodotto */


	/* TBD: Inviare il messaggio di risposta al client.
	        Si chiami la funzione msgsnd() all'interno di una
			sezione critica.
	 */

	printf("\nSono Prodotto di Server. Invio del calcolo: %d\n\n", v3);  

	/*Fatto da me*/
	Server risp;
	risp.risposta = v3;
	risp.typeServer = dati->typeClient;

	/*Fatto da me*/
  	pthread_mutex_lock(&(mutex));


	ret = msgsnd(id_coda_risposte, &risp, sizeof(Server)-sizeof(long), 0); /* TBD */

	/*Fatto da me*/
	pthread_mutex_unlock(&(mutex));


	if(ret < 0) {
		perror("Errore invio risposta server");
		exit(1);
	}

	free(v);

	pthread_exit(NULL);
}

