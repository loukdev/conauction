#include "err.h"
#include "conec.h"
#include <string.h>

int main(int argc, char * argv[])
{
	if(argc < 3)
		PWEXIT("Usage: <prog> <server> <port>.");

	printf("Initialisation de la connexion... "); fflush(stdout);

	conec_t con_auct;
	if(conec_init_host(&con_auct, SOCK_DGRAM, argv[1], atoi(argv[2])) == -1)
		return -1;

	printf("Ok\nEnvoi de la requête de connexion... "); fflush(stdout);

	if(conec_udp_connect(&con_auct) == -1)
	{
		conec_close(&con_auct);
		printf("Echec de la connexion. Au revoir.\n");

		return -1;
	}

	printf("Ok\nAttente de réception de la description et du prix...\n");

	char * str;
	const int ssz = prot_recv_str(con_auct.sock, &str, & con_auct.addr);
	if(ssz == -1)
	{
		conec_close(&con_auct);
		return -1;
	}

	char biddesc[ssz];
	float bidpric;
	strcpy(biddesc, str);

	printf("Reçu la description : '%s'.\n", biddesc);

	if(prot_recv_flt(con_auct.sock, &bidpric, & con_auct.addr) == -1)
	{
		conec_close(&con_auct);
		return -1;
	}

	printf("Reçu le prix de départ : %f.\nContinuer ? ", bidpric);

	char c;
	scanf("%c", &c);

	conec_close(&con_auct);

	return 0;
}

