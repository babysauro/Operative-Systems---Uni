#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "Header.h"


int main(){

	pid_t pidc, pids;
	int id_c, id_s,i;
	int ret;

	/*Fatto da me*/
	key_t chiave_client = ftok(".", 'a');
	id_c = msgget(chiave_client, IPC_CREAT|0644); /* Ottenere una coda per le richieste dal client al server */

	if(id_c < 0) {
		perror("Errore allocazione coda");
		exit(1);
	}


	/*Fatto da me*/
	key_t chiave_server = ftok(".", 'b');
	id_s = msgget(chiave_server, IPC_CREAT|0644); /* Ottenere una coda per le risposte dal server al client */

	if(id_s < 0) {
		perror("Errore allocazione coda");
		exit(1);
	}



	for(i=0;i<CLIENT;i++){

		/* TBD: Avviare i processi client, chiamando client() */
		pidc = fork();

		if (pidc==0)
		{
			//PROCESSO FIGLIO
			client(id_c, id_s);
			exit(0);
		}
		else if (pidc<0)
		{
			perror("ERRORE fork() client");
			exit(1);
		}
	}


	pids = fork();

	if(pids<0) {
		perror("Errore fork server");
		exit(1);
	}
	/* TBD: Avviare il processo server, chiamando server() */
	else if (pids==0)
	{
		//PROCESSO FIGLIO
		server(id_c, id_s);
		exit(0);
	}


	/* TBD: Attendere la terminazione dei client */
	for (i=0; i<CLIENT; i++)
		wait(0);


	/* TBD: Inviare al server il messaggio di terminazione, con i valori {-1, -1} */
	Client uscita;
	uscita.val1 = -1;
	uscita.val2 = -1;
	uscita.typeClient = getpid();
	ret = msgsnd(id_c, &uscita, sizeof(Client)-sizeof(long), 0); /* TBD */


	if(ret < 0) {
		perror("Errore invio messaggio di terminazione");
		exit(1);
	}

	wait(0);

	/* TBD: Deallocazione code */
	msgctl(id_c, 0, IPC_RMID); //Coda client
	msgctl(id_s, 0, IPC_RMID); //Coda server

	return 0;

}

