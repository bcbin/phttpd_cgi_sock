#include "unp.h"

int main(int argc, const char *argv[])
{
    int     listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    /* all init work */
    signal_init();

    /* Listen */
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    /* Bind and set socket option */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    SetReuseSock(listenfd);
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    /* Listen */
    Listen(listenfd, LISTENQ);

    /* nonblock signal child */
    signal(SIGCHLD, SIG_IGN);

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
