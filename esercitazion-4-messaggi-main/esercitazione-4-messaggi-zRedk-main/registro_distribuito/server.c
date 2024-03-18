#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "registro.h"

void server(int id_coda_registro_richieste, int id_coda_registro_risposte, int id_server) {

    printf("Registro: Server...\n");

    int risorsa = 0;

    int id_coda_server = msgget(IPC_PRIVATE, IPC_CREAT|0644);/* TBD */

    printf("Server: Invio messaggio BIND (id_server=%d, id_coda=%d)\n", id_server, id_coda_server);

    /* TBD: Inviare messaggio di BIND al registro,
     *      tramite la coda "id_coda_registro_richieste",
     *      e includere nel messaggio lo ID della
     *      coda privata del server "id_coda_server"
     */
    int ret;

    messaggio_registro msg;

    msg.id_coda = id_coda_server;
    msg.id_server = id_server;
    msg.tipo = bind;
    msg.pid = 1;

    ret = msgsnd(id_coda_registro_richieste, &msg, sizeof(messaggio_registro)-sizeof(long), 0);

    if (ret < 0) {
            perror("errore msgsnd bind");
            exit(1);
    }

    while(1) {

        printf("Server: In attesa di messaggi...\n");

        /* TBD: Prelevare un messaggio dalla coda del server */
        messaggio_server ser;
        int ret = msgrcv(id_coda_server, &ser, sizeof(messaggio_server)-sizeof(long), 0, 0);

        if (ret < 0) {
            perror("errore msgrcv messaggio server");
            exit(1);
    }


        if(ser.tipo == service/* TBD: Messaggio di tipo SERVICE */) {

            printf("Server: Ricevuto messaggio SERVICE (id_server=%d, valore=%d)\n", id_server, ser.valore/* TBD */);

            risorsa = ser.valore/* TBD: Assegnare il valore ricevuto alla risorsa */;
        }
        else if(ser.tipo == EXIT/* TBD: Messaggio di tipo EXIT */) {

            printf("Server: Ricevuto messaggio EXIT (id_server=%d)\n", id_server);

            /* TBD: De-allocare la coda del server */
            msgctl(id_coda_server, IPC_RMID, 0);

            printf("Server: Uscita\n");

            /* TBD: Terminare il processo */
            exit(0);
        }
        else {

            printf("Server: Ricevuto messaggio non riconosciuto\n");
        }

    }
}
