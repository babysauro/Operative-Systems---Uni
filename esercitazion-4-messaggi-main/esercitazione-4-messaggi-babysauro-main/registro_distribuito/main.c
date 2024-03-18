#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "registro.h"

int main() {
pid_t pid;

    /* TBD: Creare le code di messaggi, e avviare i processi server e registro,
     *      facendogli chiamare le funzioni server() e registro().
     */

    int id_coda_registro_richieste = msgget(IPC_PRIVATE, IPC_CREAT|0644)/* TBD */;

    int id_coda_registro_risposte = msgget(IPC_PRIVATE, IPC_CREAT|0644)/* TBD */;

    /* TBD: Avviare un processo figlio, e chiamare la funzione:
     *      registro(id_coda_registro_richieste,id_coda_registro_risposte);
     */

        pid = fork();

        if (pid == 0) {
            registro(id_coda_registro_richieste, id_coda_registro_risposte);
            exit(0);
        } else if (pid<0) {
            perror("errore fork registro");
            exit(1);
        }

    for (int i = 0; i < 2; i++)
    {
        pid = fork();
        /* TBD: Avviare un processo figlio, e chiamare la funzione:
         *      server(id_coda_registro_richieste, id_coda_registro_risposte, i);
         */
         if (pid == 0) {
            server(id_coda_registro_richieste, id_coda_registro_risposte,i);
            exit(0);
        } else if (pid<0) {
            perror("errore fork registro");
            exit(1);
        }
    }


    /* NOTA: Lasciare questa sleep per avviare i client con un ritardo,
     *       in modo da dare il tempo ai server di registrarsi sul processo registro.
     */

    sleep(2);

    for (int i = 0; i < 3; i++)
    {
        /* TBD: Avviare un processo figlio, e chiamare la funzione:
         *      client(id_coda_registro_richieste, id_coda_registro_risposte);
         */
         pid = fork();
         if (pid == 0) {
            client(id_coda_registro_richieste, id_coda_registro_risposte);
            exit(0);
        } else if (pid<0) {
            perror("errore fork registro");
            exit(1);
        }
    }


    /* TBD: Attendere la terminazione dei processi client */
    for (int i = 0; i<3; i++) {
        wait(NULL);
    }

    printf("Padre: Invio messaggio EXIT\n");

    /* TBD: Inviare un messaggio di tipo EXIT al registro,
     *      tramite "id_coda_registro_richieste"
     */

        messaggio_registro ext;
        int ret;

        ext.pid=getpid();
        ext.tipo=EXIT;

        ret = msgsnd(id_coda_registro_richieste, &ext, sizeof(messaggio_registro)-sizeof(long), 0);
    /* TBD: Attendere la terminazione dei processi server e del processo registro */

    for (int i=0; i<3; i++) {
        wait(NULL);
    }


    /* TBD: Rimuovere le code */
    msgctl(id_coda_registro_richieste, IPC_RMID, 0);
    msgctl(id_coda_registro_risposte, IPC_RMID, 0); 
}