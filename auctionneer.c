#include "conec.h"

int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Not enough arguments. Usage: %s <port number>\n", argv[0]);
		return 0;
	}

	conec_t con_home, con_auction;

	// Création des sockets d'acceuil et de service :
	if(conec_init(&con_home, SOCK_DGRAM) == -1)
		return -1;
	if(conec_init(&con_auction, SOCK_DGRAM) == -1)
		return -1;

	// Liage de la socket au port donné en argument pour l'accueil
	if(conec_bind(&con_home, atoi(argv[1])) == -1)
		return -1;
	// Liage de la socket à un port automatiquement donné :
	if(conec_bind(&con_auction, 0) == -1)
		return -1;

	conec_close(&con_home);
	conec_close(&con_auction);

	return 0;
}

