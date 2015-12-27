#ifndef __SOCK_H
#define __SOCK_H

#include <stdlib.h>
#include <netinet/in.h>
#include "writen.h"
#include "dbg.h"

#define BUF_SIZE 10000
#define SOCK_REQUEST_SIZE

// typedef struct sock {
//     unsigned char VN;
//     unsigned char CD;
//     unsigned short DST_PORT;
//     unsigned int DST_IP;
//     // char *USER_ID;
// } __attribute__((packed)) Sock;

int send_sock_request(int connfd, int index);

#endif
