#include "protocol.h"
#include "err.h"

int prot_send_str(int sock, const void * data, const sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[pbid_send_str] : Null parameter.");

	char * str = (char *) data;
	const size_t ssz = strlen(str);
	socklen_t addr_sz = sizeof(*addr);

	int r = sendto(sock, &ssz, sizeof(ssz), 0, (const sa_t *) addr, addr_sz);
	if(r != ssz)
	{
		if(r < 0)
			PREXIT("[pbid_send_str] sendto (string size) ");
		else
			PWEXIT("[pbid_send_str] sendto : String size not fully sent.");
	}

	r = sendto(sock, str, ssz, 0, (const sa_t *) addr, addr_sz); 
	if(r != ssz)
	{
		if(r < 0)
			PREXIT("[pbid_send_str] sendto (string) ");
		else
			PWEXIT("[pbid_send_str] sendto : String not fully sent.");
	}

	pcode_t code;
	if(read(sock, (void *) &code, sizeof(code)) == -1)
		PREXIT("[pbid_send_str] read ");

	if(code != PROT_ANS_OK)
	{
		if(code & PROT_ERR)
			PWEXIT("[pbid_send_str] : Error from client.");
		PWEXIT("[pbid_send_str] : Unexpected protocal value.");
	}

	return 0;
}

int prot_send_flt(int sock, const void * data, const sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_send_str] : Null parameter.");

	float * flt = (float *) data;
	size_t fsz = sizeof(*flt);
	socklen_t adsz = sizeof(*addr);

	int r = sendto(sock, (void *) flt, fsz, 0, (const sa_t *) addr, adsz); 
	if(r != fsz)
	{
		if(r < 0)
			PREXIT("[prot_send_str] sendto ");
		else
			PWEXIT("[prot_send_str] sendto : String not fully sent.");
	}

	pcode_t code;
	if(read(sock, (void *) &code, sizeof(code)) == -1)
		PREXIT("[prot_send_str] read ");

	if(code != PROT_ANS_OK)
	{
		if(code & PROT_ERR)
			PWEXIT("[prot_send_str] : Error from client.");
		PWEXIT("[prot_send_str] : Unexpected protocal value.");
	}

	return 0;
}


int prot_recv_str(int sock, char ** data, sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_recv_str] : Null parameter.");

	size_t ssz;
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

	pcode_t code = PROT_ANS_OK;
	ssz = sizeof(code);

	*data = str;

	r = sendto(sock, (void *) &code, ssz, 0, (sa_t *) addr, adsz);
	if(r != ssz)
	{
		if(r == -1)
			PREXIT("[prot_recv_str] sendto (OK) ");
		else
			PWEXIT("[prot_recv_str] sendto : Protocol value not fully sent.");
	}

	return ssz;
}

int prot_recv_flt(int sock, float * data, sin_t * addr)
{
	if(!data || !addr)
		PWEXIT("[prot_recv_flt] : Null parameter.");

	size_t fsz = sizeof(*data);
	socklen_t adsz = sizeof(*addr);

	int r = recvfrom(sock, (void *) data, fsz, 0, (sa_t *) addr, &adsz);
	if(r == -1)
		PREXIT("[prot_recv_flt] recvfrom ");

	pcode_t code = PROT_ANS_OK;
	fsz = sizeof(code);

	r = sendto(sock, (void *) &code, fsz, 0, (sa_t *) addr, adsz);
	if(r != fsz)
	{
		if(r == -1)
			PREXIT("[prot_recv_flt] sendto (OK) ");
		else
			PWEXIT("[prot_recv_flt] sendto : Protocol value not fully sent.");
	}

	return 0;
}

