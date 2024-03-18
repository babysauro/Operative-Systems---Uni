#include "sensore.h"
#include "aggregatore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {


    /* TBD: Creare le code di messaggi, 
     * e avviare i processi sensore, aggregatore, e collettore 
     */

    //Sensore
    int id_coda_sensore = msgget(IPC_PRIVATE, IPC_CREAT|0644);

    if (id_coda_sensore<0)
    {
        perror("ERRORE msgget sensore");
        exit(1);
    }

    //Collettore
    int id_coda_collettore[3];

    for (int i=0; i<3; i++)
    {
        id_coda_collettore[i] = msgget(IPC_PRIVATE, IPC_CREAT|0644);

        if (id_coda_collettore[i]<0)
        {
            perror("ERRORE msgget collettore ");
            exit(1);
        }
    }

    pid_t pid = fork();

    //Sensore
    if (pid==0)
    {
        sensore(id_coda_sensore);
        exit(0);
    }
    else if(pid<0)
    {
        perror("ERRORE sensore");
        exit(1);
    }

    //Aggregatore
    pid = fork();
     if (pid==0)
    {
        aggregatore(id_coda_sensore, id_coda_collettore);
        exit(0);
    }
    else if(pid<0)
    {
        perror("ERRORE aggregatore");
        exit(1);
    }


    //Collettori
    for (int i=0; i<3;i++)
    {
        pid = fork();
       if (pid==0)
       {
         collettore(id_coda_collettore[i]);
         exit(0);
       }
       else if (pid<0)
       {
        perror("ERRORE fork() collettori");
        exit(1);
       }
    }


    //Aspettare i processi
    for(int i=0; i<5; i++)
        wait(NULL);
    
    //Deallocare le code
    msgctl(id_coda_sensore,IPC_RMID, NULL);

    for (int i=0; i<0; i++)
        msgctl(id_coda_collettore[i], IPC_RMID, NULL);

 
}