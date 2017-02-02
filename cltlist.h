#ifndef CLTLIST_H
#  define CLTLIST_H

#include "conec.h"
#include <string.h>

typedef struct {
	sin_t * addrs;
	size_t size;
	size_t cap;
} cltlist_t;

int		cltlist_init(cltlist_t *, size_t);
void	cltlist_free(cltlist_t *);

int		cltlist_is_full(cltlist_t *);
int		cltlist_add(cltlist_t *, const sin_t * addr);

int		cltlist_sendtoall(int sock, const void * data, cltlist_t *,
						  int (*) (int, const void *, const sin_t *));

#endif
