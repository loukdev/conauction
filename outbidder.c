#include "err.h"
#include "conec.h"
#include <string.h>

int main(int argc, char * argv[])
{
	if(argc < 3)
		PWEXIT("Usage: <prog> <server> <port>.");

	printf("Création socket.\n");

	conec_t con_auct;
	if(conec_init(&con_auct, SOCK_DGRAM) == -1)
		return -1;

	struct hostent * host;
	if(!(host = gethostbyname(argv[1])))
		{herror("gethostbyname "); return -1;}

	struct sockaddr_in * addr = & con_auct.addr;
	addr->sin_port = htons(atoi(argv[2]));
	memcpy(& addr->sin_addr, host->h_addr, host->h_length);

	printf("Envoi de la requête de connexion... "); fflush(stdout);

	prot_t code = PROT_REQ_CON;

	if(sendto(con_auct.sock, (void *) &code, sizeof(code), 0,
		(struct sockaddr *) & con_auct.addr, con_auct.addr_size) != sizeof(code))
		PREXIT("sendto ");

	printf("OK.\n"
		   "Réception de la réponse... "); fflush(stdout);

	conec_t temp;
	if(recvfrom(con_auct.sock, (void *) &code, sizeof(code), 0,
		(struct sockaddr *) & temp.addr, & temp.addr_size) == -1)
		PREXIT("recvfrom ");

	if(code != PROT_REQ_OK)
		printf("Connexion refusée.\n");
	else
		printf("Connexion acceptée.\n");

	conec_close(&con_auct);

	return 0;
}

