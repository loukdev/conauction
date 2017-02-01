#ifndef ERR_H
#  define ERR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define PREXIT(str)\
do {\
	if(errno != EINTR) \
	{\
		fprintf(stderr, "Error ! ");\
		perror(str);\
	}\
	return -1;\
} while(0)

#define PWEXIT(str) do { fprintf(stderr, "Warning ! %s\n", (str)); return 1; } while(0)

#endif
