#include "sock.h"
#include "client.h"

Request requests[5];

/* send SOCK4 connection mode request
 * return 0 if success, -1 if fail */
int send_sock_request(int connfd, int index)
{
    char package[8];
    struct hostent *sh;

    package[0] = 4;
    package[1] = 1;
    package[2] = atoi(requests[index].port) / 256;
    package[3] = atoi(requests[index].port) % 256;

    sh = gethostbyname(requests[index].sock_ip);
    if (DEBUG) {
        fprintf(stderr, "sh->> %u\n", (unsigned char)sh->h_addr_list[0][0]);
        fprintf(stderr, "sh->> %u\n", (unsigned char)sh->h_addr_list[0][1]);
        fprintf(stderr, "sh->> %u\n", (unsigned char)sh->h_addr_list[0][2]);
        fprintf(stderr, "sh->> %u\n", (unsigned char)sh->h_addr_list[0][3]);
    }

    package[4] = (unsigned char) sh->h_addr_list[0][0];
    package[5] = (unsigned char) sh->h_addr_list[0][1];
    package[6] = (unsigned char) sh->h_addr_list[0][2];
    package[7] = (unsigned char) sh->h_addr_list[0][3];

    Writen(connfd, package, sizeof(package));

    if(read(connfd, package, sizeof(package)) < 0) {
        log_err("Read sock reply");
        return -1;
    }

    if( package[1] == 91 ) {
        log_err("Sock reject");
        return -1;
    }

    return 0;
}
