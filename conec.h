#ifndef CONEC_H
#  define CONEC_H

#include "err.h"
#include "protocol.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
	struct sockaddr_in addr;
	socklen_t addr_size;
	int sock;

} conec_t;

int		conec_init(conec_t *, int type);
void	conec_close(conec_t *);

int		conec_bind(conec_t *, short port);
int		conec_accept(conec_t *, conec_t *);
int		conec_udp_accept(conec_t *, struct sockaddr_in *);

int		conec_sendto_str(conec_t *, uint8_t * data);
int		conec_sendto_u32(conec_t *, uint8_t * data);

char * strbin(unsigned long int i);

#endif

