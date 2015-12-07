#ifndef __REQUEST_H
#define __REQUEST_H

#include "dbg.h"

#define REQUEST_HOST_SIZE 128
#define REQUEST_PORT_SIZE 64
#define REQUEST_FILENAME_SIZE 1000

typedef struct {
    char *ip;
    char *port;
    char *filename;
    char *socket;
    FILE *fp;
} Request;

#endif
