#include "src/unp.h"

void client_handler(int connfd)
{
    dprintf(connfd, "connfd=%d", connfd);
    Close(connfd);
}

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

    for( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        /* Fork to handle new connections */
        pid_t pid;
        pid = Fork();

        if (pid < 0) {

            log_err("Fork failed");
            exit(EXIT_FAILURE);

        } else if (pid == 0) {      /* Child */

            client_handler(connfd);

            exit(EXIT_SUCCESS);
        } else {                    /* Parend */

            Close(connfd);

        }
    }
}
