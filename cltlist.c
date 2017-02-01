#include "cltlist.h"

int cltlist_init(cltlist_t * clients, size_t capacity)
{
	struct sockaddr_in * addrs;
	addrs = malloc(sizeof(struct sockaddr_in) * capacity);
	if(!addrs)
		PREXIT("[cltlist_init] malloc ");

	clients->addrs = addrs;
	clients->addr_sz = sizeof(*addrs);
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

int cltlist_add(cltlist_t * clients, const struct sockaddr_in * addr)
{
	if(!clients || !addr)
		PWEXIT("[cltlist_add] : Null parameter.");

	if(clients->size >= clients->cap)
		PWEXIT("[cltlist_add] : Array is full.");

	clients->addrs[clients->size] = *addr;
	clients->size++;
}


int clt_sendstrto(int sock, void * data, const struct sockaddr_in * addr)
{
	if(!data || !addr)
		PWEXIT("[clt_sendstrto] : Null parameter.");

	char * str = (char *) data;
	size_t ssz = strlen(str);
	socklen_t addr_sz = sizeof(*addr);

	int r = sendto(sock, str, ssz, 0, (const struct sockaddr *) addr, addr_sz); 
	if(r != ssz)
	{
		if(r < 0)
			PREXIT("[clt_sendstrto] sendto ");
		else
			PWEXIT("[clt_sendstrto] sendto : String not fully sent.");
	}

	pcode_t code;
	if(read(sock, (void *) &code, sizeof(code)) == -1)
		PREXIT("[clt_sendstrto] read ");

	if(code != PROT_ANS_OK)
		PWEXIT("[clt_sendstrto] : Unexpected protocal value.");

	return 0;
}

int clt_sendu32to(int sock, void * data, const struct sockaddr_in * addr)
{
	if(!data || !addr)
		PWEXIT("[clt_sendu32to] : Null parameter.");

	puint_t * val = (puint_t *) data;
	socklen_t addr_sz = sizeof(*addr);

	int r = sendto(sock, val, sizeof(*val), 0, (const struct sockaddr *) addr, addr_sz); 
	if(r != sizeof(*val))
	{
		if(r < 0)
			PREXIT("[clt_sendu32to] sendto ");
		else
			PWEXIT("[clt_sendu32to] sendto : String not fully sent.");
	}

	pcode_t code;
	if(read(sock, (void *) &code, sizeof(code)) == -1)
		PREXIT("[clt_sentu32to] read ");

	if(code != PROT_ANS_OK)
		PWEXIT("[clt_sendu32to] : Unexpected protocal value.");

	return 0;
}

int cltlist_sendtoall(int sock, void * data, cltlist_t * clients,
	int (*usend) (int, void *, const struct sockaddr_in *))
{
	if(!clients || !data || !usend)
		PWEXIT("[cltlist_sendtoall_str] : Null parameter.");

	int i; for(i = 0; i < clients->size; i++)
		if(usend(sock, data, &clients->addrs[i]) == -1)
			return -1;

	return 0;
}
