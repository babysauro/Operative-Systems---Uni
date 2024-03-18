#include "processi.h"
#include "semafori.h"

#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define MESSAGGIO_DISP 0
#define SPAZIO_DISP 1

int inizializza_semafori()
{
    int sem_id = semget (IPC_PRIVATE, 2, IPC_CREAT|0664);/* TBD: usare semget() per allocare un vettore,
                                                          *      con una coppia di semafori */

    if (sem_id < 0)
    {
        perror("Impossibile creare il semaforo per la mutua esclusione");
        exit(1);
    }

    /* Valori iniziali: 0 (messaggio disponibile), 1 (spazio disponibile) */
    
    /* TBD: inizializzare i semafori */
    semctl(sem_id, SPAZIO_DISP, SETVAL, 1);
    semctl(sem_id, MESSAGGIO_DISP, SETVAL, 0);

    return sem_id;
}

void figlio(int *vettore,
            int *buffer,
            int sem_id,
            int elemento_iniziale,
            int qta_elementi)
{

    /* TBD: aggiungere dentro questa funzione delle chiamate a
     *      Wait_Sem() e Signal_Sem() per realizzare lo schema
     *      produttore-consumatore con buffer singolo */
    
    

    int minimo = INT_MAX;

    for (int i = elemento_iniziale; i < elemento_iniziale + qta_elementi; i++)
    {

        if (vettore[i] < minimo)
        {

            minimo = vettore[i];
        }
    }

    printf("Figlio: Il minimo locale è %d\n", minimo);

    Wait_Sem(sem_id, SPAZIO_DISP);
    *buffer = minimo;
    Signal_Sem(sem_id, MESSAGGIO_DISP);

}

void padre(int *buffer,
           int sem_id)
{

     /* TBD: aggiungere dentro questa funzione delle chiamate a
     *      Wait_Sem() e Signal_Sem() per realizzare lo schema
     *      produttore-consumatore con buffer singolo */
    
    

    int minimo = INT_MAX;

    for (int i = 0; i < NUM_PROCESSI; i++)
    {
        Wait_Sem(sem_id, SPAZIO_DISP);
        if( *buffer < minimo ) {

            minimo = *buffer;
        }
        Signal_Sem(sem_id, MESSAGGIO_DISP);
    }

    /* Attesa terminazione processi figli */

    /* TBD: Utilizzare wait() per attendere la terminazione dei 10 figli */
    for (int k=0; k<10; k++)
    {
        wait(NULL);
    }
    

    /* Risultato finale */

    printf("Padre: Il valore minimo assoluto è: %d\n", minimo);
}
