#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "registro.h"

void registro(int id_coda_registro_richieste, int id_coda_registro_risposte) {
    
    printf("Registro: Avvio...\n");


    /* Questo vettore verrà usato per memorizzare gli ID delle code dei server */
    int id_code_server[2];
    id_code_server[0] = 0;
    id_code_server[1] = 0;


    while(1) {

        printf("Registro: In attesa di messaggi...\n");

        /* TBD: Prelevare un messaggio dalla coda "id_coda_registro_richieste" */
        messaggio_registro msg;
        int ret;
         
        ret = msgrcv(id_coda_registro_richieste, &msg, sizeof(messaggio_registro)-sizeof(long), 0, 0);
        if (ret < 0) {
            perror("errore receive messaggio registro");
            exit(1);
    }

        if(msg.tipo == bind/* TBD: Messaggio di tipo BIND */) {

            printf("Registro: Ricevuto messaggio BIND (id_server=%d, id_coda=%d)\n", /* TBD */msg.id_server,msg.id_coda /* TBD */);

            if(msg.id_server != 0 && msg.id_server != 1/* TBD: Controllare che lo ID del server sia 0 oppure 1 */) {
                printf("Registro: ID server non valido\n");
                continue;
            }

            printf("Registro: Registrazione server %d\n", msg.id_server/* TBD */);

            id_code_server[msg.id_server/* TBD */] = msg.id_coda/* TBD: Memorizzare lo ID della coda */;

        }
        else if(msg.tipo == query/* TBD: Messaggio di tipo QUERY */) {

            printf("Registro: Ricevuto messaggio QUERY (id_server=%d)\n", msg.id_server/* TBD */);

            if(msg.id_server != 0 && msg.id_server != 1/* TBD: Controllare che lo ID del server sia 0 oppure 1 */) {
                printf("Registro: ID server non valido\n");
                continue;
            }

            // TBD: Controllare che lo ID del server sia stato precedentemente registrato
            if(id_code_server[msg.id_server/* TBD */] == 0) {
                printf("Registro: ID server non registrato\n");
                continue;
            }

            /* TBD: Inviare un messaggio di risposta di tipo RESULT al client,
             *      tramite la coda "id_coda_registro_risposte" */

            messaggio_registro ris;

            ris.pid=msg.pid;

            ris.tipo = result;
            ris.id_coda= id_code_server[msg.id_server];
            ris.id_server = msg.id_server;
            int ret;

            printf("Registro: Invio messaggio RESULT (id_coda=%d)\n",id_code_server[msg.id_server] /* TBD: ID della coda del server */);

            ret = msgsnd(id_coda_registro_risposte, &ris, sizeof(messaggio_registro)-sizeof(long), 0);

                 if (ret < 0) {
                     perror("errore msgsnd result");
                    exit(1);
                 }

        }
        else if(msg.tipo == EXIT/* TBD: Messaggio di tipo EXIT */) {

            printf("Registro: Ricevuto messaggio EXIT\n");


            for(int i = 0; i<2; i++) {

                if(id_code_server[i] == 0) {
                    continue;
                }

                /* TBD: Inviare un messaggio di tipo EXIT al server, 
                 *      tramite la coda "id_code_server[i]"
                 */
                messaggio_server ser;

                ser.tipo= EXIT;
                ser.pid = 1;

                printf("Registro: Invio messaggio EXIT a server %d\n", i);

                ret = msgsnd(id_code_server[i],&ser,sizeof(messaggio_server)-sizeof(long),0);

                if (ret < 0) {
                     perror("errore msgsnd exit");
                    exit(1);
                 }

            }

            printf("Registro: Uscita\n");

            /* TBD: Terminare il processo */
            exit(0);
            
        }
        else {

            printf("Registro: Ricevuto messaggio non riconosciuto\n");
        }
    }
}
