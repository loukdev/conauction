#include "conec.h"

char * strbin(unsigned long int i)
{
    static char buffer [1+sizeof (unsigned long int)*8] = { 0 };
    char *p=buffer-1+sizeof (unsigned long int)*8;
    do { *--p = '0' + (i & 1); i >>= 1; } while (i);
    return p;
}


int conec_init(conec_t * conec, int type)
{
	if(!conec)
		PWEXIT("[conec_init] : A parameter is null.");

	// Création de la socket :
	if((conec->sock = socket(AF_INET, type, 0)) == -1)
		PREXIT("[conec_init] socket ");

	struct sockaddr_in * addr = & conec->addr;

	// Initialisation par défaut de l'adresse et de sa taille :
	conec->addr_size = sizeof(*addr);
	addr->sin_family = AF_INET;
	addr->sin_port = 0;

	return 0;
}

void conec_close(conec_t * conec)
{
	if(!conec)
	{
		fprintf(stderr, "Warning ! [conec_close] : A parameter is null.\n");
		return;
	}

	close(conec->sock);
	conec->addr_size = 0;
}


int conec_bind(conec_t * conec, short port)
{
	if(!conec)
		PWEXIT("[conec_bind] : A parameter is null.");

	// Préparation de l'adresse :
	struct sockaddr_in * addr = & conec->addr;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = htonl(INADDR_ANY);

	// Liage de la socket à l'adresse voulue :
	if(bind(conec->sock, (struct sockaddr *) addr, conec->addr_size) == -1)
		PREXIT("[conec_bind] bind ");

	return 0;
}

int conec_accept(conec_t * conec, conec_t * accepted)
{
	if(!conec || !accepted)
		PWEXIT("[conec_accept] : A parameter is null.");

	// Préparation de l'adresse :
	accepted->addr.sin_family = AF_INET;
	accepted->addr_size = sizeof(accepted->addr);
	// Acceuil d'un client :
	int s = accept(conec->sock, (struct sockaddr *) & accepted->addr, & accepted->addr_size);
	if(s == -1)
		PREXIT("[conec_accept] accept ");

	// Mise à jour du descripteur de socket :
	accepted->sock = s;

	return 0;
}

int conec_udp_accept(conec_t * conec, struct sockaddr_in * addr)
{
	if(!conec || !addr)
		PWEXIT("[conec_udp_accept] : A parameter is null.");

	// Préparation de l'adresse :
	addr->sin_family = AF_INET;
	socklen_t size = sizeof(*addr);

	pcode_t code;

	// Réception du code d'opération :
	if(recvfrom(conec->sock, (void *) &code, sizeof(code), 0, (struct sockaddr *) addr, &size) == -1)
		PREXIT("[conec_udp_accept] recvfrom ");

	// Si ce n'est pas une requête de connexion :
	if(code != PROT_REQ_CON)
	{
		fprintf(stderr, "Warning ! [conec_udp_accept] : Unexpected protocol value.\n");
		code = PROT_ERR_UNVA;
	}	// Si c'est une requête de connexion :
	else
		code = PROT_REQ_OK;

	// Envoi de la réponse :
	int r = sendto(conec->sock, (void *) &code, sizeof(code), 0, (const struct sockaddr *) addr, size);

	// Si la réponse n'a pas été entièrement envoyée :
	if(r != sizeof(code))
	{
		if(r == -1)
			PREXIT("[conec_udp_accept] sendto ");

		PWEXIT("[conec_udp_accept] : Wrong size of sent code.");
	}

	return 0;
}

