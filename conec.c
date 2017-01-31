#include "conec.h"

int conec_init(conec_t * conec, int type)
{
	if(!conec)
		return 1;

	// Création socket :
	if((conec->sock = socket(AF_INET, type, 0)) == -1)
		PREXIT("[conec_init] socket ");

	struct sockaddr_in * addr = & conec->addr;

	// Initialisation par défaut de l'adresse et de sa taille :
	conec->addr_size = sizeof(*addr);
	addr->sin_family = AF_INET;
	addr->sin_port = 0;

	return 0;
}

int conec_bind(conec_t * conec, short port)
{
	if(!conec)
		return 1;

	struct sockaddr_in * addr = & conec->addr;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(conec->sock, (struct sockaddr *) addr, conec->addr_size) == -1)
		PREXIT("[conec_bind] bind ");

	return 0;
}

void conec_close(conec_t * conec)
{
	if(!conec)
		return;

	close(conec->sock);
	conec->addr_size = 0;
}

