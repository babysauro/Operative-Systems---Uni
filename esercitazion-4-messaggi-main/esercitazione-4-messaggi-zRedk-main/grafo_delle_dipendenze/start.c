#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	pid_t pid;
	int i;



    key_t queue_operandi_1  = ftok(".", 1);/* TBD: Definire una chiave */
	key_t queue_risultati_1 = ftok(".", 2);/* TBD: Definire una chiave */
	key_t queue_operandi_2  = ftok(".", 3);/* TBD: Definire una chiave */
	key_t queue_risultati_2 = ftok(".", 4);/* TBD: Definire una chiave */



    int id_operandi_1 = msgget(queue_operandi_1, IPC_CREAT|0644);/* TBD: Creare una nuova coda */

	if(id_operandi_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



    int id_risultati_1 = msgget(queue_risultati_1, IPC_CREAT|0644);/* TBD: Creare una nuova coda */

	if(id_risultati_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int id_operandi_2 = msgget(queue_operandi_2, IPC_CREAT|0644); /* TBD: Creare una nuova coda */

	if(id_operandi_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int id_risultati_2 = msgget(queue_risultati_2, IPC_CREAT|0644);/* TBD: Creare una nuova coda */

	if(id_risultati_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	/* TBD: Creare 6 processi figli, e fare in modo che ognuno di loro
	   esegua un degli eseguibili "p1", "p2", etc.

	   NOTA: È consentito anche non usare un ciclo.
	   (poiché il numero dei processi è fisso, si può replicare il
		codice per 6 volte, cambiando solo il percorso dell'eseguibile)
	 */

	pid=fork();
		
	if (pid==0) {
		execl("./p1", "p1", NULL);

		exit(0);

	}else if (pid<0) {

			perror("Errore fork");
			exit(1);

	}

	pid=fork();
		
	if (pid==0) {
		execl("./p2", "p2", NULL);

		exit(0);

	}else if (pid<0) {

			perror("Errore fork");
			exit(1);
			
	}

	pid=fork();
		
	if (pid==0) {
		execl("./p3", "p3", NULL);

		exit(0);

	}else if (pid<0) {

			perror("Errore fork");
			exit(1);
			
	}

	pid=fork();
		
	if (pid==0) {
		execl("./p4", "p4", NULL);

		exit(0);

	}else if (pid<0) {

			perror("Errore fork");
			exit(1);
			
	}

	pid=fork();
		
	if (pid==0) {
		execl("./p5", "p5", NULL);

		exit(0);

	}else if (pid<0) {

			perror("Errore fork");
			exit(1);
			
	}

	pid=fork();
		
	if (pid==0) {
		execl("./p6", "p6", NULL);

		exit(0);

	}else if (pid<0) {

			perror("Errore fork");
			exit(1);
			
	}

	/* TBD: Attendere la terminazione dei 6 processi figli */
	for (i=0; i<6; i++) {
		wait(NULL);
	}


    /* TBD: Rimuovere le code */

	msgctl(id_operandi_1, IPC_RMID,0);
	msgctl(id_operandi_2, IPC_RMID,0);
	msgctl(id_risultati_1, IPC_RMID,0);
	msgctl(id_risultati_2, IPC_RMID,0);

	return 0;
}

