#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{

    /* Coda "OK TO SEND" */

    key_t ok_key = ftok(".", 1);/* TBD: Definire una chiave per "OK TO SEND" */

    int ok_id = msgget(ok_key, IPC_CREAT|0644); /* TBD: Ottenere la coda per i messaggi "OK TO SEND" */

    if (ok_id < 0)
    {
        perror("Errore creazione coda messaggi");
        exit(1);
    }

    /* Coda "REQUEST TO SEND" */

    key_t req_key = ftok(".",2);/* TBD: Definire una chiave per "REQUEST TO SEND" */

    int req_id = msgget(req_key, IPC_CREAT|0644);/* TBD: Ottenere la coda per i messaggi "REQUEST TO SEND" */

    if (req_id < 0)
    {
        perror("Errore creazione coda messaggi");
        exit(1);
    }


    for (int i = 0; i < 2; i++)
    {
        /* TBD: Avviare 2 processi server */
        pid_t pid=fork();
        if (pid==0)
        {
        execlp("./server", "server",NULL);

        perror("Errore fork server");
        exit(1);
        }
    }


    for (int i = 0; i < 4; i++)
    {
        /* TBD: Avviare 4 processi client */
        pid_t pid=fork();
        if (pid==0)
        {
        execlp("./client", "client", NULL);

        perror("Errore fork client");
        exit(1);
        }
    }




    printf("[%d] In attesa di terminazione...\n", getpid());

    /* TBD: Attendere la terminazione di tutti i processi */
    for (int i=0; i<6; i++) {
        wait(NULL);
    }



    printf("[%d] Deallocazione code\n", getpid());

    /* TBD: Deallocazione delle code */
    msgctl(ok_id, IPC_RMID, NULL);
    msgctl(req_id, IPC_RMID, NULL);
    return 0;
}