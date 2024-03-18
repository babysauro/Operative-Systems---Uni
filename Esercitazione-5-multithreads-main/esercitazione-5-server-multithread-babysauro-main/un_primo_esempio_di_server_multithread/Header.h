#include <sys/types.h>

#define RICHIESTE 5
#define CLIENT 3

/* TBD: Definire una struct per i messaggi dal client al server */
typedef struct {
    long typeClient;
    int val1;
    int val2;
    
}Client;

/* TBD: Definire una struct per i messaggi dal server al client */
typedef struct {
    long typeServer;
    int risposta;
}Server;


void client(int id_c, int id_s);
void server(int id_c, int id_s);
void* Prodotto(void*);
