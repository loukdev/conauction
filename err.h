#ifndef ERR_H
#  define ERR_H

#include <stdio.h>
#include <stdlib.h>

#define PREXIT(str) do { perror(str); return -1; } while(0)

#endif
