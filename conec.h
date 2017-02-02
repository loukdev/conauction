#ifndef CONEC_H
#  define CONEC_H

#include "err.h"
#include "protocol.h"

typedef struct {
	struct sockaddr_in addr;
	socklen_t addr_size;
	int sock;

} conec_t;

int		conec_init(conec_t *, int type);
int		conec_init_host(conec_t *, int, const char *, int);
void	conec_close(conec_t *);

int		conec_bind(conec_t *, short port);
int		conec_accept(conec_t *, conec_t *);
int		conec_udp_accept(conec_t *, sin_t *);
int		conec_udp_connect(conec_t *);

int		conec_sendto_str(conec_t *, uint8_t * data);
int		conec_sendto_u32(conec_t *, uint8_t * data);

#endif

