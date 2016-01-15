#include "unp.h"

int main(int argc, const char *argv[])
{
    int     listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    /* all init work */
    signal_init();

	listenfd = tcp_listen();

    /* Listen */
    Listen(listenfd, LISTENQ);

    log_info("Start listen on port %d", SERV_PORT);

    for( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        /* Fork to handle new connections */
        pid_t pid;
        pid = Fork();

        if (pid == 0) {             /* child */

            Request *request = Malloc(sizeof(request));
            strcpy(request->remote_addr, inet_ntoa(cliaddr.sin_addr));
            cgi_handler(connfd, request);

            exit(EXIT_SUCCESS);

        } else {                    /* parend */

            Close(connfd);
        }
    }

    /* should never go here */
    exit(EXIT_FAILURE);
}
