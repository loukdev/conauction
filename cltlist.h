#ifndef CLTLIST_H
#  define CLTLIST_H

#include "conec.h"
#include <string.h>

typedef struct {
	struct sockaddr_in * addrs;
	socklen_t addr_sz;
	size_t size;
	size_t cap;
} cltlist_t;

int		cltlist_init(cltlist_t *, size_t);
void	cltlist_free(cltlist_t *);

int		cltlist_is_full(cltlist_t *);
int		cltlist_add(cltlist_t *, const struct sockaddr_in * addr);

int		clt_sendstrto(int sock, void *, const struct sockaddr_in *);
int		clt_sendu32to(int sock, void *, const struct sockaddr_in *);
int		cltlist_sendtoall(int sock, void * data, cltlist_t *,
						  int (*) (int, void *, const struct sockaddr_in *));

#endif
