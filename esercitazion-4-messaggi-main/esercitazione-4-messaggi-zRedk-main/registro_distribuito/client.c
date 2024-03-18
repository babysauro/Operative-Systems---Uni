#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "registro.h"

void client(int id_coda_registro_richieste, int id_coda_registro_risposte) {

    printf("Client: Avvio...\n");

    srand(getpid());

    int id_server = rand() % 2; // Sceglie un server a caso

    printf("Client: Invio messaggio QUERY (id_server=%d)\n", id_server);

    /* TBD: Inviare un messaggio di tipo QUERY al registro,
     *      tramite la coda "id_coda_registro_richieste"
     */

    messaggio_registro msg;
    msg.pid = getpid();
    msg.tipo=query;
    msg.id_server= id_server;
    int ret;

    ret = msgsnd(id_coda_registro_richieste, &msg, sizeof(messaggio_registro)-sizeof(long), 0);

    if (ret < 0) {
            perror("errore msgsnd query");
            exit(1);
    }

    printf("Client: Attesa messaggio RESULT...\n");

    /* TBD: Ricevere il messaggio di risposta di tipo RESULT dal registro,
     *      tramite la coda "id_coda_registro_risposte"
     */

    messaggio_registro ris;

    ret = msgrcv(id_coda_registro_risposte, &ris, sizeof(messaggio_registro)-sizeof(long), getpid(), 0);

    if (ret < 0) {
            perror("errore msrcv result");
            exit(1);
    }

    int id_coda_server = ris.id_coda;/* TBD: prelevare lo ID della coda dal messaggio RESULT */

    printf("Client: Ricevuto messaggio RESULT (id_coda=%d)\n", id_coda_server);



    for(int i = 0; i<3; i++) {

        int valore = rand() % 11;

        printf("Client: Invio messaggio SERVICE (id_server=%d, id_coda=%d, valore=%d)\n", id_server, id_coda_server, valore);
        
        /* TBD: Inviare un messaggio di tipo SERVICE al server */

        messaggio_server ser;

        int ret;
        ser.pid = getpid();
        ser.valore = valore;
        ser.tipo = service;

        ret = msgsnd(id_coda_server, &ser, sizeof(messaggio_server)-sizeof(long), 0);

        if (ret < 0) {
            perror("errore msgsnd service");
            exit(1);
    }

        sleep(1);
    }

    printf("Client: Uscita\n");
}
