#include "protocol.h"

char * strbin(unsigned long int i)
{
    static char buffer [1+sizeof (unsigned long int)*8] = { 0 };
    char * p = buffer-1 + sizeof (unsigned long int)*8;
    do {
		* --p = '0' + (i & 1);
		i >>= 1;
	} while (i);
    return p;
}


int prot_send_str(int sock, const void * data, const sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_send_str] : Null parameter.");

	char * str = (char *) data;
	pint_t ssz = strlen(str) + 1;
	socklen_t adsz = sizeof(*addr);

	int r = sendto(sock, &ssz, sizeof(ssz), 0, (const sa_t *) addr, adsz);
	if(r != sizeof(ssz))
	{
		if(r == -1)
			PREXIT("[prot_send_str] sendto (string size) ");
		else
			PWEXIT("[prot_send_str] sendto : String size not fully sent.");
	}

	r = sendto(sock, str, ssz, 0, (const sa_t *) addr, adsz); 
	if(r != ssz)
	{
		if(r == -1)
			PREXIT("[prot_send_str] sendto (string) ");
		else
			PWEXIT("[prot_send_str] sendto : String not fully sent.");
	}

	return 0;
}

int prot_send_flt(int sock, const void * data, const sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_send_str] : Null parameter.");

	float * flt = (float *) data;
	pint_t fsz = sizeof(*flt);
	socklen_t adsz = sizeof(*addr);

	int r = sendto(sock, (void *) flt, fsz, 0, (const sa_t *) addr, adsz); 
	if(r != fsz)
	{
		if(r < 0)
			PREXIT("[prot_send_str] sendto ");
		else
			PWEXIT("[prot_send_str] sendto : String not fully sent.");
	}

	return 0;
}


int prot_recv_str(int sock, char ** data, sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_recv_str] : Null parameter.");

	pint_t ssz;
	socklen_t adsz = sizeof(*addr);

	int r = recvfrom(sock, (void *) &ssz, sizeof(ssz), 0, (sa_t *) addr, &adsz);
	if(r == -1)
		PREXIT("[prot_recv_str] recvfrom (string size) ");

	static char * str = NULL;
	if(str)	free(str);
	str = malloc(ssz);

	r = recvfrom(sock, (void *) str, ssz, 0, (sa_t *) addr, &adsz);
	if(r == -1)
		PREXIT("[prot_recv_str) recvfrom (string) ");

	*data = str;

	return ssz;
}

int prot_recv_flt(int sock, float * data, sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_recv_flt] : Null parameter.");

	pint_t fsz = sizeof(*data);
	socklen_t adsz = sizeof(*addr);

	int r = recvfrom(sock, (void *) data, fsz, 0, (sa_t *) addr, &adsz);
	if(r == -1)
		PREXIT("[prot_recv_flt] recvfrom ");

	return 0;
}

