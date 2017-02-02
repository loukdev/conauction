#ifndef PROTOCOL_H
#  define PROTOCOL_H

#include <stdint.h>
#include <string.h>

#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "err.h"

typedef uint16_t pcode_t;
typedef uint32_t puint_t;

typedef struct sockaddr_in sin_t;
typedef struct sockaddr sa_t;

#define PROT_ERR	0b0000000100000000
#define	PROT_REQ	0b0000001000000000
#define	PROT_ANS	0b0000010000000000
#define PROT_BID	0b0000100000000000

#define PROT_RFS	0b0000000000000000
#define PROT_OK		0b0000000000000001
#define PROT_CON	0b0000000000000010
#define PROT_UNVA	0b0000000000000100	// UNexpected VAlue
#define PROT_PRC	0b0000000000001000
#define PROT_DSC	0b0000000000010000

#define PROT_ERR_UNVA	(PROT_ERR | PROT_UNVA)

#define	PROT_REQ_CON	(PROT_REQ | PROT_CON)
#define PROT_REQ_OK		(PROT_REQ | PROT_OK)
#define PROT_REQ_RFS	(PROT_REQ | PROT_RFS)

#define PROT_ANS_OK		(PROT_ANS | PROT_OK)

#define PROT_BID_PRC	(PROT_BID | PROT_PRC)
#define PROT_BID_DSC	(PROT_BID | PROT_DSC)

char * strbin(unsigned long int);

int	prot_send_str(int, const void *, const sin_t *);
int	prot_send_flt(int, const void *, const sin_t *);

int prot_recv_str(int, char **, sin_t *);
int prot_recv_flt(int, float *, sin_t *);

#endif
