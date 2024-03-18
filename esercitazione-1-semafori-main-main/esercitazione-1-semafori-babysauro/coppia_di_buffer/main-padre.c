#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "buffer.h"

int main()
{

    int buf1_id;
    int buf2_id;
    buffer *buf1;
    buffer *buf2;
    int sem_id;

    buf1_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0644);/* TBD: Creazione shared memory per il primo buffer (singolo) */

    if (buf1_id < 0)
    {
        perror("Errore creazione SHM buf1");
        exit(1);
    }

    buf2_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0644); /* TBD: Creazione shared memory per il secondo buffer (singolo) */

    if (buf2_id < 0)
    {
        perror("Errore creazione SHM buf2");
        exit(1);
    }

    buf1 = shmat(sem_id, NULL, 0); /* TBD: Attach shared memory del primo buffer (singolo) */

    if (buf1 == (void *)-1)
    {
        perror("Errore attach SHM buf1");
        exit(1);
    }

    buf2 = shmat(sem_id, NULL, 0); /* TBD: Attach shared memory del secondo buffer (singolo) */

    if (buf2 == (void *)-1)
    {
        perror("Errore attach SHM buf2");
        exit(1);
    }

    buf1->stato = LIBERO; /* TBD: inizializzazione stato buf1 */
    buf2->stato = LIBERO; /* TBD: inizializzazione stato buf2 */


    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT|0644); /* TBD: Creazione vettore semafori (spazio disponibile, messaggio disponibile) */

    if (sem_id < 0)
    {
        perror("Errore creazione semafori");
        exit(1);
    }

    /* TBD: inizializzazione semafori */
    semctl(sem_id, SPAZIO_DISP, SETVAL, 1);
    semctl(sem_id, MESSAGGIO_DISP, SETVAL, 0);


    /* TBD: Creare il processo produttore, fargli eseguire l'eseguibile "./main-produttore" */
    pid_t pid;
    pid = fork();

    if (pid==0)
    {
        //PROCESSO FIGLIO
        execlp("./main-produttore", NULL); //Esegue l'eseguibile del main-produttore
    }
    else if (pid<0)
    {
       perror("ERRORE FORK"); 
       exit(1);
    }
     

    /* TBD: Creare il processo consumatore, fargli eseguire l'eseguibile "./main-consumatore" */
    pid = fork();

    if (pid==0)
    {
        //PROCESSO FIGLIO
        execlp("./main-consumatore", NULL); //Esegue l'eseguibile del main-produttore
    }
    else if (pid<0)
    {
       perror("ERRORE FORK"); 
       exit(1);
    }


    for (int i = 0; i < 2; i++)
    {
        wait(NULL);
    }

    shmctl(buf1_id, IPC_RMID, NULL);
    shmctl(buf2_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}
