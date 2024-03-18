#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#include "header.h"

int main(){

	/* TBD: Creare in memoria condivisa un'area per contenere un oggetto "MonitorMeteo" */

	key_t id_meteo = ftok(".", 'm')/* TBD */;
	MonitorMeteo * p = shmget(id_meteo, sizeof(MonitorMeteo), IPC_CREAT |0644) /* TBD */;

	/* TBD: inizializzare il MonitorMeteo, incluso il sotto-oggetto Monitor con init_monitor() */
	init_monitor(&p->m, NUM_CONDITIONS);
	p->num_lettori = 0;
	p->num_scrittori = 0;


	pid_t pid;

	int k;
	for (k=0; k<NUM_UTENTI; k++) {

		pid=fork();
		if (pid==0) {
			Utente(p);
			exit(0);
     	} else if(pid<0) {
			perror("fork");
		}
	}


	pid=fork();
	if (pid==0) {
		Servizio(p);
		exit(0);
	} else if(pid<0) {
		perror("fork");
	}


	int status;
	for (k=0; k<NUM_UTENTI+1; k++) {
		pid=wait(&status);
		if (pid==-1)
			perror("errore");
	}

	/* TBD: Deallocare il sotto-oggetto Monitor con remove_monitor() */
	remove_monitor(&p->m);

	/* TBD: Deallocare la memoria condivisa */
	shmctl(id_meteo, IPC_RMID, 0);

	return 0;
}
