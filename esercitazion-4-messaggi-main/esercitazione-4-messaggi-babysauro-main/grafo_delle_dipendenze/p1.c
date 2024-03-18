#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "p.h"


int main() {

	printf("Processo P1 avviato\n");



	key_t queue_operandi_1  = ftok(".", 1);/* TBD: Definire la chiave per la coda 1 per gli operandi */
	key_t queue_risultati_1 = ftok(".", 2);/* TBD: Definire la chiave per la coda 1 per le risposte */



	int id_operandi_1 = msgget(queue_operandi_1, 0644);/* TBD: Ottenere la coda 1 per gli operandi */

	if(id_operandi_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	int id_risultati_1 = msgget(queue_risultati_1, 0644);/* TBD: Ottenere la coda 1 per le risposte */

	if(id_risultati_1 < 0) {
		perror("Msgget fallita");
		exit(1);
	}



	srand(time(NULL));


	int i;

	for(i=0; i<3; i++) {


		int ret;

		int a = rand() % 10;
		int b = rand() % 10;
		int c = rand() % 10;
		int d = rand() % 10;
		int e = rand() % 10;
		int f = rand() % 10;
		int g = rand() % 10;
		int h = rand() % 10;

		int r1, r2, r3, risultato;



		printf("[P1] OPERANDI: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d, h=%d\n", a, b, c, d, e, f, g, h);


		/* TBD: Inviare gli operandi "a" e "b" al processo P2 sulla coda 1 degli operandi */
		struct messaggio msg;
		msg.type=P2;
		msg.operandi[0] = a;
		msg.operandi[1] = b;

		printf("[P1] SEND P2\n");
		

		ret = msgsnd(id_operandi_1, &msg, sizeof(struct messaggio)-sizeof(long), 0);/* TBD */

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}



		/* TBD: Inviare gli operandi "c", "d", "e", "f" al processo P3 sulla coda 1 degli operandi */
		msg.type=P3;
		msg.operandi[0]= c;
		msg.operandi[1]= d;
		msg.operandi[2]= e;
		msg.operandi[3]= f;

		printf("[P1] SEND P3\n");

		ret = msgsnd(id_operandi_1, &msg, sizeof(struct messaggio)-sizeof(long), 0);/* TBD */

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}




		/* TBD: Inviare gli operandi "g" e "h" al processo P4 sulla coda 1 degli operandi */


		printf("[P1] SEND P4\n");
		msg.type=P4;
		msg.operandi[0]= g;
		msg.operandi[1]= h;

		ret = msgsnd(id_operandi_1, &msg, sizeof(struct messaggio)-sizeof(long), 0);/* TBD */

		if(ret<0) {
			perror("Msgsnd fallita");
			exit(1);
		}



		/* TBD: Ricevere la risposta "r1" dal processo P2 sulla coda 1 delle risposte */
		struct risposte ris;

		printf("[P1] RECEIVE P2\n");

		ret = msgrcv(id_risultati_1, &ris, sizeof(struct risposte)-sizeof(long), P2, 0);/* TBD */

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r1 = ris.risposta;/* TBD */




		/* TBD: Ricevere la risposta "r2" dal processo P3 sulla coda 1 delle risposte */

		printf("[P1] RECEIVE P3\n");

		ret = msgrcv(id_risultati_1, &ris, sizeof(struct risposte)-sizeof(long), P3, 0);/* TBD */

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r2 = ris.risposta;/* TBD */




		/* TBD: Ricevere la risposta "r3" dal processo P4 sulla coda 1 delle risposte */

		printf("[P1] RECEIVE P4\n");

		ret = msgrcv(id_risultati_1, &ris, sizeof(struct risposte)-sizeof(long), P4, 0);/* TBD */

		if(ret<0) {
			perror("Msgrcv fallita");
			exit(1);
		}

		r3 = ris.risposta;/* TBD */





		printf("[P1] RISULTATI INTERMEDI: r1=%d, r2=%d, r3=%d\n", r1, r2, r3);




		risultato = r1 + r2 + r3;


		printf("[P1] RISULTATO: %d\n", risultato);

	}


	return 0;
}
