#ifndef CONEC_H
#  define CONEC_H

#include "err.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
	struct sockaddr_in addr;
	size_t addr_size;
	int sock;

} conec_t;

int		conec_init(conec_t *, int type);
int		conec_bind(conec_t *, short port);
void	conec_close(conec_t *);

#endif

