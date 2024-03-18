#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

void client(int id_c, int id_s){

	int k;
	int ret;

	srand(getpid());



	for(k=0;k<RICHIESTE;k++){

		/* TBD: Inviare un messaggio di richiesta */
		Client cl;
		Server sv;

		int v1 = rand()%101;
		int v2 = rand()%101;

		/*Fatto da me*/
		cl.val1 = v1; 
		cl.val2 = v2;
		cl.typeClient = 1;
		cl.typeClient = getpid();

		printf("Richiesta %d Inviata (%d, %d) [PID=%d]\n\n", k, v1, v2, getpid());

		ret = msgsnd(id_c, &cl, sizeof(Client)-sizeof(long), 0); /* TBD */

		if(ret < 0) {
			perror("Errore invio richiesta client");
			exit(1);
		}


		/* TBD: Ricevere un messaggio di risposta */
		size_t size_server = sizeof(Server) - sizeof(long);
		ret = msgrcv(id_s, &sv, size_server, getpid(), 0); /* TBD */

		if(ret < 0) {
			perror("Errore ricezione risposta client");
			exit(1);
		}

		int v3 = sv.risposta; /* TBD */

		printf("Risposta %d Ricevuta (%d) [PID=%d]\n\n", k, v3, getpid());
	}

}

