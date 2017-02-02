#include "cltlist.h"

int cltlist_init(cltlist_t * clients, size_t capacity)
{
	sin_t * addrs;
	addrs = malloc(sizeof(sin_t) * capacity);
	if(!addrs)
		PREXIT("[cltlist_init] malloc ");

	clients->addrs = addrs;
	clients->size = 0;
	clients->cap = capacity;

	return 0;
}

void cltlist_free(cltlist_t * clients)
{
	if(!clients)
		return;

	free(clients->addrs);
	clients->size = clients->cap = 0;
}


int cltlist_is_full(cltlist_t * clients)
{
	if(!clients)
		PWEXIT("[cltlist_is_full] : Null parameter.");

	return clients->size >= clients->cap;
}

int cltlist_add(cltlist_t * clients, const sin_t * addr)
{
	if(!clients || !addr)
		PWEXIT("[cltlist_add] : Null parameter.");

	if(clients->size >= clients->cap)
		PWEXIT("[cltlist_add] : Array is full.");

	clients->addrs[clients->size] = *addr;
	clients->size++;
}


int cltlist_sendtoall(int sock, const void * data, cltlist_t * clients,
	int (*usend) (int, const void *, const sin_t *))
{
	if(!clients || !data || !usend)
		PWEXIT("[cltlist_sendtoall_str] : Null parameter.");

	int i; for(i = 0; i < clients->size; i++)
		if(usend(sock, data, &clients->addrs[i]) == -1)
			return -1;

	return 0;
}
