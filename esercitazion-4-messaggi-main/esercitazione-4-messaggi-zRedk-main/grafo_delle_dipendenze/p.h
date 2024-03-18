#ifndef _P_H_
#define _P_H_

/* TBD: Definire delle macro da usare per il campo "type" dei messaggi */

#define P1 1/*TBD*/
#define P2 2/*TBD*/
#define P3 3/*TBD*/
#define P4 4/*TBD*/
#define P5 5/*TBD*/
#define P6 6/*TBD*/


/* TBD: Definire una struct per i messaggi con gli operandi (vettore di interi) */
struct messaggio
{
    long type;
    int operandi[4];
};

/* TBD: Definire una struct per i messaggi con le risposte (un intero) */
struct risposte
{
    long type;
    int risposta;
};


#endif // _P_H_
