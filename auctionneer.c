#include "conec.h"
#include "cltlist.h"

#include <ctype.h>
#include <signal.h>

#define NCLI	32

#define STEP_EXIT		-1
#define STEP_ACCEPT		0
#define STEP_START		1
#define STEP_AUCTION	2


char step = 0;

void	ctrl_c(int);
int		ask_continue(void);

int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Not enough arguments. Usage: %s <port number>\n", argv[0]);
		return 0;
	}

	struct sigaction sa;
	sa.sa_handler = ctrl_c;
	if(sigaction(SIGINT, &sa, NULL) == -1)
		PREXIT("[main] sigaction ");

	conec_t con_home, con_auction;
	cltlist_t clients;

	printf("Initialistaion des connexions... "); fflush(stdout);

	// Création des sockets d'acceuil et de service :
	if(conec_init(&con_home, SOCK_DGRAM) == -1)
		return -1;
	if(conec_init(&con_auction, SOCK_DGRAM) == -1)
		return -1;

	// Liage de la socket au port donné en argument pour l'accueil :
	if(conec_bind(&con_home, atoi(argv[1])) == -1)
		return -1;
	// Liage de la socket à un port automatiquement donné :
	if(conec_bind(&con_auction, 0) == -1)
		return -1;

	printf("OK.\n");

	// Demande de la description et du prix au commissaire :
	char biddescr[] = "Crayon cure-dent";
	float bidprice = 1.0;

	// Création de la liste pour 32 clients :
	if(cltlist_init(&clients, NCLI) == -1)
		return -1;

	if(step == STEP_ACCEPT)
		printf("Etape : accueil des clients.\n");

	char said = 0;
	// Acceuil des clients :
	while(step != STEP_EXIT)
	{
		switch(step)
		{
			case STEP_ACCEPT:
			{
				struct sockaddr_in clt_addr;
				if(conec_udp_accept(&con_home, &clt_addr) == -1)
					{step = STEP_EXIT; break;}

				printf("Un client a été accepté.\n");

				// Ajout du client dans la liste :
				if(cltlist_add(&clients, &clt_addr) == -1)
					{step = STEP_EXIT; break;}

				if(cltlist_is_full(&clients))
				{
					printf("Salle pleine.\n");
					fflush(stdout);
					step = STEP_START;

					break;
				}

				// On demande au commissaire s'il veut continuer :
				char b = ask_continue();
				if(b == -1)
					step = STEP_EXIT;
				else if(b == 0)
					step = STEP_START;
			} break;
			case STEP_START:
			{
				printf("Etape : démarrage de l'enchère.\n");

				// Fermeture de la connexion d'accueil :
				conec_close(&con_home);

				// Envoi de la description et du prix de départ à tout le monde :
				if(cltlist_sendtoall(con_auction.sock, biddescr, &clients, clt_sendstrto) == -1)
					step = STEP_EXIT;
				else if(cltlist_sendtoall(con_auction.sock, (puint_t *) &bidprice, &clients, clt_sendu32to) == -1)
					step = STEP_EXIT;
				else
					step = STEP_AUCTION;
			} break;
			case STEP_AUCTION:
			{
				if(!said)
					{printf("Etape : enchères.\n"); said = 1;}

				// Réception d'une offre :

				// Calcul de la meilleure offre et affichage :

				// On demande au commissaire s'il faut continuer :

				// Si non, on envoie la fin de l'enchère :
				// ...
				// step = STEP_EXIT;

				// Envoi de la nouvelle meilleure offre :

			} break;
			default:
				break;
		}
	}

	printf("Fermeture des connexions... "); fflush(stdout);

	cltlist_free(&clients);
	conec_close(&con_home);
	conec_close(&con_auction);

	printf(" Au revoir.\n");

	return 0;
}

void ctrl_c(int signum)
{
	// Évite le warning :
	(void) signum;

	printf("\nReçu signal d'arrêt.\n");

	step = STEP_EXIT;
}

int ask_continue(void)
{
	printf("Continuer l'accueil ? [O]ui/[N]on : ");
	char c;
	if(scanf("%c", &c) == EOF)
		PREXIT("[ask_continue] scanf");

	while(c != '\n' && getchar() != '\n');

	return toupper(c) == 'O';
}

