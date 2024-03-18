#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prodcons_msg.h"
#include "prodcons_client.h"

static int id_coda_richieste;
static int id_coda_risposte;

void init_client(int id_coda_richieste_parametro, int id_coda_risposte_parametro) {

    id_coda_richieste = id_coda_richieste_parametro;
    id_coda_risposte = id_coda_risposte_parametro;
}

void produci_con_somma(int val1, int val2, int val3) {

    richiesta_rpc req;
    risposta_rpc ris;
    int ret;

    req.tipo = SOMMA;
    req.pid = getpid();

    req.var1 = val1;
    req.var2 = val2;
    req.var3 = val3;
    /* TBD: Inviare un messaggio di tipo "PRODUCI CON SOMMA" */

    printf("[Client] Invio richiesta PRODUCI_CON_SOMMA(%d, %d, %d)\n", val1, val2, val3);

    ret = msgsnd(id_coda_richieste, &req, sizeof(richiesta_rpc) - sizeof(long), 0);

    if(ret < 0){
        perror("errore msgsnd somma");
        exit(1);
    }

    /* TBD: Ricevere un messaggio di risposta */
    ret = msgrcv(id_coda_risposte, &ris, sizeof(risposta_rpc) - sizeof(long), getpid(), 0);

    int risultato = ris.somma /* TBD */;
    int errore = ris.errore /* TBD */;

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);
}

void produci(int val) {

    richiesta_rpc req;
    risposta_rpc ris;
    int ret;

    req.var1 = val;
    
    req.tipo = PRODUCI;
    req.pid = getpid();
    /* TBD: Inviare un messaggio di tipo "PRODUCI" */

    printf("[Client] Invio richiesta PRODUCI(%d)\n", val);

    ret = msgsnd(id_coda_richieste, &req, sizeof(richiesta_rpc) - sizeof(long), 0);

    if(ret < 0){
        perror("errore msgsnd produzione");
        exit(1);
    }
    /* TBD: Ricevere un messaggio di risposta */
    ret = msgrcv(id_coda_risposte, &ris, sizeof(risposta_rpc)-sizeof(long), getpid(), 0); 

    int risultato = ris.somma; /* TBD */
    int errore = ris.errore; /* TBD */

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);
}

int consuma() {
    richiesta_rpc req;
    risposta_rpc ris;
    int ret;
    
    req.tipo = CONSUMA;
    req.pid = getpid();

    /* TBD: Inviare un messaggio di tipo "CONSUMA" */

    printf("[Client] Invio richiesta CONSUMA(nessun parametro)\n");

    ret = msgsnd(id_coda_richieste, &req, sizeof(richiesta_rpc)-sizeof(long), 0);

    if(ret < 0){
        perror("errore msgsnd consumazione");
        exit(1);
    }


    /* TBD: Ricevere un messaggio di risposta */
    ret = msgrcv(id_coda_risposte, &ris, sizeof(risposta_rpc)-sizeof(long), getpid(), 0);

    int risultato = ris.somma; /* TBD */
    int errore = ris.errore; /* TBD */

    printf("[Client] Ricevuto risposta: risultato=%d, errore=%d\n", risultato, errore);

    return risultato;
}