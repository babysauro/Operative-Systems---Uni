#include "monitor_hoare.h"

#define NUM_UTENTI 10

typedef struct {
	int temperatura;
	unsigned int umidita;
	unsigned short pioggia;
} meteo;

typedef struct {
	meteo meteo;

	/* TBD: Aggiungere un sotto-oggetto Monitor e altre variabili di stato
	 * per la sincronizzazione */
	int buffer;
	int num_lettori;
	int num_scrittori;

	Monitor m;

} MonitorMeteo;

#define CV_LETT 0
#define CV_SCRITT 1
#define NUM_CONDITIONS 2

void Servizio(MonitorMeteo* p);
void Utente(MonitorMeteo* p);
