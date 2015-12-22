#include "unp.h"

int main(int argc, const char *argv[])
{
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    SetReuseSock(listenfd);
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, BACKLOG);

    log_info("Start listen on port %d", SERV_PORT);

    for(;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        pid_t pid;
        pid = Fork();

        if (pid == 0) {

            exit(EXIT_SUCCESS);

        } else {

            Close(connfd);
        }
    }

    /* should never go here */
    exit(EXIT_FAILURE);
}
