#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prodcons_msg.h"
#include "prodcons_server.h"

#define TOTALE_WORKER 3
#define RICHIESTE_PER_WORKER 2

void * worker(void *);

struct param {

    int id_coda_richieste;
    int id_coda_risposte;
};

int main() {

    key_t chiave_coda_richieste = ftok(".", 'a'); /* TBD: Scegliere una chiave per la coda richieste */

    int id_coda_richieste = msgget(chiave_coda_richieste, IPC_CREAT|0644); /* TBD: Creare la coda richieste */

    /*Fatto da me*/
    if (id_coda_richieste<0)
    {
        perror("ERRORE msgget richieste (Server)");
        exit(1);
    }



    key_t chiave_coda_risposte = ftok(".", 'b'); /* TBD: Scegliere una chiave per la coda risposte */

    int id_coda_risposte = msgget(chiave_coda_risposte, IPC_CREAT|0644); /* TBD: Creare la coda risposte */

    /*Fatto da me*/
    if (id_coda_risposte<0)
    {
        perror("ERRORE msgget risposte (Server)");
        exit(1);
    }



    init_monitor();

    /*Fatto da me*/
    pthread_t thread_worker[TOTALE_WORKER]; 


    for(int i=0; i<TOTALE_WORKER; i++) {

        /* TBD: Creare il gruppo di thread "Worker", 
                facendogli eseguire la funzione worker(),
                e passandogli gli id delle code in ingresso
                usando "struct param" (definita sopra).
         */
        struct param *w = (struct param *) malloc (sizeof(struct param));

        pthread_create(&thread_worker[i], NULL, worker, w); //Al posto dell'attributo, ho messo NULL
        w->id_coda_richieste = id_coda_richieste;
        w->id_coda_risposte = id_coda_risposte;
    }


    for(int i=0; i<TOTALE_WORKER; i++) {

        /* TBD: Attendere la terminazione dei thread Worker */
        pthread_join(thread_worker[i], NULL);
    }

    remove_monitor();

    return 0;
}


void * worker(void * x) {

    /*Fatto da me*/
    struct param * w = (struct param *) x;

    int id_coda_richieste = w->id_coda_richieste; /* TBD: Prendere in ingresso lo ID della coda richieste */
    int id_coda_risposte = w->id_coda_risposte; /* TBD: Prendere in ingresso lo ID della coda risposte */

    int ret;
    int risultato;
    int errore;

    printf("[Worker] In attesa di richieste...\n");


    for(int i=0; i<RICHIESTE_PER_WORKER; i++) {

        /* TBD: Ricevere un messaggio di richiesta RPC */
        richiesta_rpc req;
        ret = msgrcv(id_coda_richieste, &req, sizeof(richiesta_rpc)-sizeof(long), 0, 0);

        if (ret<0)
        {
            perror("ERRORE msgrcv richiesta (Server)");
            exit(1);
        }

        
        if( req.tipo == SOMMA/* TBD: Il tipo di richiesta è "PRODUCI CON SOMMA" */) {

            int val1 = req.var1; /* TBD */
            int val2 = req.var2; /* TBD */
            int val3 = req.var3; /* TBD */

            printf("[Worker] Ricevuta richiesta di tipo PRODUCI CON SOMMA(%d, %d, %d)\n", val1, val2, val3);

            produci_con_somma(val1, val2, val3);

            risultato = 0;
            errore = 0;

        }
        else if(req.tipo==PRODUCI /* TBD: Il tipo di richiesta è "PRODUCI" */) {

            int val1 = req.var1; /* TBD */

            printf("[Worker] Ricevuta richiesta di tipo PRODUCI(%d)\n", val1);

            produci(val1);

            risultato = 0;
            errore = 0;
        }
        else if(req.tipo==CONSUMA /* TBD: Il tipo di richiesta è "CONSUMA" */) {

            printf("[Worker] Ricevuta richiesta di tipo CONSUMA(nessun parametro)\n");

            risultato = consuma();
            errore = 0;
        }
        else {

            printf("[Worker] Errore, tipo di richiesta sconosciuta");

            risultato = -1;
            errore = 1;
        }

        /* TBD: Inviare un messaggio di risposta RPC,
                in cui inserire "risultato" ed "errore"
         */
        risposta_rpc ris;
        int pid_client = req.pid; 

        ris.somma = risultato;
        ris.errore = errore;
        ris.pid = pid_client;   
        ret = msgsnd(id_coda_risposte, &ris, sizeof(risposta_rpc)-sizeof(long), 0);

        if (ret<0)
        {
            perror("ERRORE msgrcv");
            exit(1);
        }



        printf("[Worker] Inviato risposta: risultato=%d, errore=%d\n", risultato, errore);

    }




    printf("[Worker] Terminazione\n");

    return NULL;
}
