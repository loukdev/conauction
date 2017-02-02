#include "conec.h"


int conec_init(conec_t * conec, int type)
{
	if(!conec)
		PWEXIT("[conec_init] : Null parameter.");

	// Création de la socket :
	if((conec->sock = socket(AF_INET, type, 0)) == -1)
		PREXIT("[conec_init] socket ");

	sin_t * addr = & conec->addr;

	// Initialisation par défaut de l'adresse et de sa taille :
	conec->addr_size = sizeof(*addr);
	addr->sin_family = AF_INET;
	addr->sin_port = 0;

	return 0;
}

int conec_init_host(conec_t * conec, int type, const char * hname, int port)
{
	if(!conec || !hname)
		PWEXIT("[conec_init_host] : Null parameter.");

	if((conec->sock = socket(AF_INET, type, 0)) == -1)
		PREXIT("[conec_init_host] socket ");

	struct hostent * host = gethostbyname(hname);
	if(!host)
	{
		herror("[conec_init_host] gethostbyname ");
		return -1;
	}

	sin_t * addr = & conec->addr;
	conec->addr_size = sizeof(*addr);
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	memcpy(&addr->sin_addr, host->h_addr, host->h_length);

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
	sin_t * addr = & conec->addr;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = htonl(INADDR_ANY);

	// Liage de la socket à l'adresse voulue :
	if(bind(conec->sock, (sa_t *) addr, conec->addr_size) == -1)
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
	int s = accept(conec->sock, (sa_t *) & accepted->addr, & accepted->addr_size);
	if(s == -1)
		PREXIT("[conec_accept] accept ");

	// Mise à jour du descripteur de socket :
	accepted->sock = s;

	return 0;
}

int conec_udp_accept(conec_t * conec, sin_t * addr)
{
	if(!conec || !addr)
		PWEXIT("[conec_udp_accept] : A parameter is null.");

	// Préparation de l'adresse :
	addr->sin_family = AF_INET;
	socklen_t size = sizeof(*addr);

	pcode_t code;

	// Réception du code d'opération :
	if(recvfrom(conec->sock, (void *) &code, sizeof(code), 0, (sa_t *) addr, &size) == -1)
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
	int r = sendto(conec->sock, (void *) &code, sizeof(code), 0, (const sa_t *) addr, size);

	// Si la réponse n'a pas été entièrement envoyée :
	if(r != sizeof(code))
	{
		if(r == -1)
			PREXIT("[conec_udp_accept] sendto ");

		PWEXIT("[conec_udp_accept] : Wrong size of sent code.");
	}

	return 0;
}

int conec_udp_connect(conec_t * conec)
{
	if(!conec)
		PWEXIT("[conec_udp_connect] : Null parameter.");

	pcode_t code = PROT_REQ_CON;
	sin_t * addr = &conec->addr;
	socklen_t * adsz = &conec->addr_size;

	int r = sendto(conec->sock, (void *) &code, sizeof(code), 0, (sa_t *) addr, *adsz);
	if(r != sizeof(code))
	{
		if(r == -1)
			PREXIT("[conec_udp_connect] sendto ");
		else
			PWEXIT("[conec_udp_connect] sendto : Protocol value not fully sent.");
	}

	if(recvfrom(con_auct.sock, (void *) &code, sizeof(code), 0, (sa_t *) addr, adsz) == -1)
		PREXIT("recvfrom ");

	if(code != PROT_REQ_OK)
	{
		if(code & PROT_RFS)
			PWEXIT2("[conec_udp_connect] : Connection refused by server.", -1);
		PWEXIT("[conec_udp_connect] : Unexpected protocol value.");
	}

	return 0;
}

