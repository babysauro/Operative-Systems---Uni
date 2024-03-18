#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "serversync.h"



void send_sinc(int ok_id, int req_id, messaggio * msg)
{

    int ret;
    struct Request r;
    struct SendOk s;
    r.type=getpid();
    
    /* TBD: Inviare il messaggio REQUEST TO SEND */

    printf("[%d] Client: invio request-to-send, type=%ld\n", getpid(), r.type/* TBD */);

    ret = msgsnd(req_id, &r, sizeof(struct Request)-sizeof(long),0); /* TBD */

    if (ret < 0)
    {
        perror("Errore msgsnd (request-to-send)");
        exit(1);
    }


    /* TBD: Ricevere il messaggio OK TO SEND */

    printf("[%d] Client: in attesa di ok-to-send...\n", getpid());

    ret = msgrcv(ok_id, &s, sizeof(struct SendOk)-sizeof(long), 0, 0);/* TBD */

    if (ret < 0)
    {
        perror("Errore msgrcv (ok-to-send)");
        exit(1);
    }

    printf("[%d] Client: ricevuto ok-to-send... type=%ld, id_coda=%d\n", getpid(), s.type/* TBD */, s.ID/* TBD */);




    /* TBD: Inviare il messaggio al server */

    int id_coda = s.ID; /* TBD */


    printf("[%d] Client: invio messaggio, coda=%d, type=%ld, valore=%d\n", getpid(), id_coda, msg->type, msg->val);

    ret = msgsnd(id_coda, msg, sizeof(messaggio)-sizeof(long),0);/* TBD */

    if (ret < 0)
    {
        perror("Errore msgsnd (coda messaggi)");
        exit(1);
    }

}

void receive_sinc(int msg_id, int ok_id, int req_id, messaggio * msg)
{

    int ret;
    struct Request r;
    struct SendOk s;
    
    /* TBD: Ricevere il messaggio REQUEST TO SEND */

    printf("[%d] Server: in attesa di request-to-send...\n", getpid());

    ret = msgrcv(req_id, &r, sizeof(struct Request)-sizeof(long), 0, 0); /* TBD */

    printf("[%d] Server: ricevuto request-to-send, type=%ld\n", getpid(), r.type/* TBD */);

    if (ret < 0)
    {
        perror("Errore msgrcv (request-to-send)");
        exit(1);
    }



    /* TBD: Inviare il messaggio OK TO SEND */
    s.type = r.type;
    s.ID = msg_id;
    
    printf("[%d] Server: invio ok-to-send, type=%ld, id_coda=%d\n", getpid(), s.type/* TBD */,s.ID /* TBD */);

    ret = msgsnd(ok_id,&s,sizeof(struct SendOk)-sizeof(long),0);/* TBD */

    if (ret < 0)
    {
        perror("Errore msgsnd (ok-to-send)");
        exit(1);
    }



    /* TBD: Ricevere il messaggio destinato al server, usando la struct indicata
            dal puntatore "msg" (passato dal chiamante della funzione) 
     */

    printf("[%d] Server: in attesa del messaggio...\n", getpid());

    ret = msgrcv(msg_id, msg, sizeof(messaggio)-sizeof(long), 0, 0); /* TBD */

    if (ret < 0)
    {
        perror("Errore msgrcv (coda messaggi)");
        exit(1);
    }

    printf("[%d] Server: ricevuto messaggio, type=%ld, valore=%d\n", getpid(), msg->type, msg->val);

}
