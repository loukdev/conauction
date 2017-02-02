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
		return -1;

/*
	pcode_t code = PROT_REQ_CON;

	if(sendto(con_auct.sock, (void *) &code, sizeof(code), 0,
		(sa_t *) & con_auct.addr, con_auct.addr_size) != sizeof(code))
		PREXIT("sendto ");

	printf("OK.\nRéception de la réponse... "); fflush(stdout);

	conec_t temp;
	if(recvfrom(con_auct.sock, (void *) &code, sizeof(code), 0,
		(sa_t *) & temp.addr, & temp.addr_size) == -1)
		PREXIT("recvfrom ");
*/
	if(code != PROT_REQ_OK)
		printf("Connexion refusée.\n");
	else
		printf("Connexion acceptée.\n");

	conec_close(&con_auct);

	return 0;
}

