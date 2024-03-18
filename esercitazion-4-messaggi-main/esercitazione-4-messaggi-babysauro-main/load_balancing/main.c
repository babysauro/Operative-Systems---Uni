#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main() {

	int msg_id_balancer;
	int msg_id_server[TOTALE_SERVER];
	int i;
	pid_t p;
	int status;
	int ret;
	pid_t pid;



	/*
	  TBD: Creare una coda per i messaggi dai client
	  verso il balancer
	*/
	key_t key = ftok(".",1);

	msg_id_balancer = msgget(key, IPC_CREAT|0644);/* TBD */

	if(msg_id_balancer < 0) {

		perror("Errore msgget() coda balancer");
		exit(1);
	}



	/*
	  TBD: Creare 3 code per i messaggi dal balancer verso
	  i server, una coda per ogni server
	*/

	for(i=0; i<TOTALE_SERVER; i++) {

		key = ftok(".",i+2); 

		/*Utilizzo la stessa chiave precedente poichè essa verrà sovrascritta;
		 Itero l'id poichè il ciclo for verrà ripetuto 3 volte,
		 così facendo otterrò 4 chiavi diverse (considerando anche quella precedente)
		*/

		msg_id_server[i] = msgget(key, IPC_CREAT|0644);/* TBD */

		if(msg_id_server[i] < 0) {

			perror("Errore msgget() coda server");
			exit(1);
		}
	}



	/*
	  TBD: Creare i processi client, passando alla funzione Client()
	  in ingresso lo ID della coda del balancer
	*/

	for(i=0; i<TOTALE_CLIENT; i++) {

		/* TBD */
		pid = fork();
		if(pid>0){
			//PADRE
		}
		else if(pid==0){
			//FIGLIO
			Client(msg_id_balancer);
			exit(0);
		}
		else{
			perror("errore fork");
			exit(1);
		}
	}



	/*
	  TBD: Creare i processi server, passando alla funzione Server()
	  in ingresso lo ID della coda di quel server
	*/

	for(i=0; i<TOTALE_SERVER; i++) {
		pid = fork();
		if(pid>0){
			//PADRE
		}
		else if(pid==0){
			//FIGLIO
			Server(msg_id_server[i]);
			exit(0);
		}
		else{
			perror("errore fork");
			exit(1);
		}
	}



	/*
	  TBD: Creare il processo balancer, passando alla funzione Balancer()
	in ingresso gli ID di tutte le code
	*/
	Balancer(msg_id_balancer, msg_id_server);



	/*
	  TBD: Il processo padre si pone in attesa della terminazione
	  di tutti i processi (client, server e balancer)
	*/

	for(i=0; i<TOTALE_CLIENT+TOTALE_SERVER+1; i++) {

		/* TBD */
		wait(NULL);
	}



	/*
	  TBD: Rimozione delle code dal sistema
	*/
	msgctl(msg_id_balancer, IPC_RMID, NULL);

	for (i=0; i<TOTALE_SERVER; i++) {
	msgctl(msg_id_server[i], IPC_RMID, NULL);
	}


	return 0;

}

  
