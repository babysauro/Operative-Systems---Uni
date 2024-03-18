#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prodcons_msg.h"
#include "prodcons_client.h"

void Produttore();
void Consumatore();

int main() {

    key_t chiave_coda_richieste = ftok(".", 'a');/* TBD: Scegliere una chiave per la coda richieste */

    int id_coda_richieste = msgget(chiave_coda_richieste, IPC_CREAT | 0644);/* TBD: Creare la coda richieste */

    if (id_coda_richieste<0)
    {
        perror("ERRORE msgget richieste (Client)");
        exit(1);
    }



    key_t chiave_coda_risposte = ftok(".", 'b');/* TBD: Scegliere una chiave per la coda risposte */

    int id_coda_risposte = msgget(chiave_coda_risposte, IPC_CREAT | 0644);/* TBD: Creare la coda risposte */

    if (id_coda_richieste<0)
    {
        perror("ERRORE msgget risposte (Client)");
        exit(1);
    }


    init_client(id_coda_richieste, id_coda_risposte);


    pid_t pid_prod = fork();

    if(pid_prod == 0) {
        Produttore();
        exit(0);
    }
    else if(pid_prod < 0) {
        perror("Errore fork");
        exit(1);
    }


    pid_t pid_cons = fork();

    if(pid_cons == 0) {
        Consumatore();
        exit(0);
    }
    else if(pid_prod < 0) {
        perror("Errore fork");
        exit(1);
    }

    wait(NULL);
    wait(NULL);

    
    /* TBD: Rimozione coda richieste */
    /* TBD: Rimozione coda risposte */
    msgctl(id_coda_richieste, IPC_RMID, 0);
    msgctl(id_coda_risposte, IPC_RMID, 0);
}


void Produttore() {

    srand(getpid());

    int val1 = rand() % 10;
    int val2 = rand() % 10;
    int val3 = rand() % 10;

    printf("[Produttore] Chiamo PRODUCI CON SOMMA(%d, %d, %d)\n", val1, val2, val3);

    produci_con_somma(val1, val2, val3);


    int val = rand() % 10;

    printf("[Produttore] Chiamo PRODUCI(%d)\n", val);

    produci(val);


    val = rand() % 10;

    printf("[Produttore] Chiamo PRODUCI(%d)\n", val);

    produci(val);
}

void Consumatore() {

    printf("[Consumatore] Chiamo CONSUMA()\n");

    int val = consuma();

    printf("[Consumatore] Ricevuto risultato=%d\n", val);



    printf("[Consumatore] Chiamo CONSUMA()\n");

    val = consuma();

    printf("[Consumatore] Ricevuto risultato=%d\n", val);



    printf("[Consumatore] Chiamo CONSUMA()\n");

    val = consuma();

    printf("[Consumatore] Ricevuto risultato=%d\n", val);
}