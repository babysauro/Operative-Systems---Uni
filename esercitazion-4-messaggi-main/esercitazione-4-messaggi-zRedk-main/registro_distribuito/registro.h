#ifndef _REGISTRO_H_
#define _REGISTRO_H_

// Messaggi da e verso il registro
// (per semplicità, usare questa struttura
// sia per le richieste sia per le risposte)
typedef struct {
    /* TBD: includere lo ID del server (0 oppure 1),
     *      e lo ID della coda creata dal server
     */
     long pid;
     int id_server;
     int id_coda;
     int tipo;
} messaggio_registro;

// Messaggi verso il server
typedef struct {
    /* TBD: includere il valore intero che il client invia al server */
    long pid;
    int tipo;
    int valore;
} messaggio_server;

/* TBD: Definire i tipi di messaggio */

#define bind 1
#define query 2
#define result 3
#define service 4
#define EXIT 5

void client(int id_coda_registro_richieste, int id_coda_registro_risposte);
void registro(int id_coda_registro, int id_coda_registro_risposte);
void server(int id_coda_registro, int id_coda_registro_risposte, int id_server);

#endif