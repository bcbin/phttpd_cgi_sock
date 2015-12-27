#include "sock.h"

#define BUF_SIZE 10000

typedef struct sock {
    unsigned char VN;
    unsigned char CD;
    unsigned short DST_PORT;
    unsigned int DST_IP;
    // char *USER_ID;
} __attribute__((packed)) Sock;

void sock_request(int connfd)
{
    // client code...

    Sock *sock;
    sock->VN = '4';
    sock->CD = '1';
    //sock.DST_PORT = htons();
    //sock.DST_IP = htonl();

    //printf("");
    Writen(connfd, sock, 8);
}
