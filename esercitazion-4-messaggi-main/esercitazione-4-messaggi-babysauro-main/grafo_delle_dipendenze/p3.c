#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "p.h"


int main() {

	printf("Processo P3 avviato\n");



	key_t queue_operandi_1  = ftok(".",1);/* TBD: Definire la chiave per la coda 1 per gli operandi */
	key_t queue_risultati_1 = ftok(".",2);/* TBD: Definire la chiave per la coda 1 per le risposte */
	key_t queue_operandi_2  = ftok(".",3);/* TBD: Definire la chiave per la coda 2 per gli operandi */
	key_t queue_risultati_2 = ftok(".",4);/* TBD: Definire la chiave per la coda 2 per le risposte */



	int id_operandi_1 = msgget(queue_operandi_1, 0644);/* TBD: Ottenere la coda 1 per gli operandi */

	if(id_operandi_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	int id_risultati_1 = msgget(queue_risultati_1,0644);/* TBD: Ottenere la coda 1 per le risposte */

	if(id_risultati_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	int id_operandi_2 = msgget(queue_operandi_2, 0644); /* TBD: Ottenere la coda 2 per gli operandi */

	if(id_operandi_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	int id_risultati_2 = msgget(queue_risultati_2, 0644);/* TBD: Ottenere la coda 2 per le risposte */

	if(id_risultati_2 < 0) {
		perror("Msgget fallita");
		exit(1);
	}




	int i;

	for(i=0; i<3; i++) {


		int ret;

		int c, d, e, f, r2, r4, r5;
		struct messaggio msg;
		struct risposte ris;


		/* TBD: Ricevere gli operandi "c", "d", "e" e "f" dal processo P1 sulla coda 1 degli operandi */

		printf("[P3] RECEIVE P1\n");

		ret = msgrcv(id_operandi_1, &msg, sizeof(struct messaggio)-sizeof(long), P3, 0);/* TBD */

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		c = msg.operandi[0];/* TBD */
		d = msg.operandi[1];/* TBD */
		e = msg.operandi[2];/* TBD */
		f = msg.operandi[3];/* TBD */

		printf("[P3] OPERANDI: c=%d, d=%d, e=%d, f=%d\n", c, d, e, f);




		/* TBD: Inviare gli operandi "c" e "d" al processo P5 sulla coda 2 degli operandi */
		struct messaggio msg2;
		msg2.type=P5;
		msg2.operandi[0]=c;
		msg2.operandi[1]=d;

		printf("[P3] SEND P5\n");

		ret = msgsnd(id_operandi_2,&msg2,sizeof(struct messaggio)-sizeof(long),0);/* TBD */

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}




		/* TBD: Inviare gli operandi "e" e "f" al processo P6 sulla coda 2 degli operandi */
		msg2.type=P6;
		msg2.operandi[0]=e;
		msg2.operandi[1]=f;

		printf("[P3] SEND P6\n");

		ret = msgsnd(id_operandi_2,&msg2,sizeof(struct messaggio)-sizeof(long),0);/* TBD */

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}




		/* TBD: Ricevere la risposta "r4" dal processo P5 sulla coda 2 delle risposte */

		printf("[P3] RECEIVE P5\n");

		ret = msgrcv(id_risultati_2, &ris, sizeof(struct risposte)-sizeof(long), P5, 0);/* TBD */

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r4 = ris.risposta; /* TBD */




		/* TBD: Ricevere la risposta "r5" dal processo P6 sulla coda 2 delle risposte */

		printf("[P3] RECEIVE P6\n");

		ret = msgrcv(id_risultati_2, &ris, sizeof(struct risposte)-sizeof(long), P6, 0);/* TBD */

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r5 =  ris.risposta;/* TBD */





		printf("[P3] RISULTATI INTERMEDI: r4=%d, r5=%d\n", r4, r5);





		r2 = r4 * r5;

		printf("[P3] RISULTATO: %d\n", r2);






		/* TBD: Inviare la risposta "r2" al processo P1 sulla coda 1 dei risultati */


		printf("[P3] SEND P1\n");
		ris.risposta=r2;
		ris.type=P3;

		ret = msgsnd(id_risultati_1,&ris,sizeof(struct risposte)-sizeof(long),0);/* TBD */

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}


	}


	return 0;
}


