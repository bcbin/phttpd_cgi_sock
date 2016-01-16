#include "unp.h"

typedef struct _conn {
    int connfd;
    struct sockaddr_in cliaddr;
} Connection;

static void *
run(void *arg)
{
    int connfd;
    struct sockaddr_in cliaddr;
    Connection conn;

    conn = *((Connection *) arg);
    connfd = conn.connfd;
    cliaddr = conn.cliaddr;

    /* print client ip and port */
    log_info("connfd=%d, ip=%u.%u.%u.%u(port=%d)",
            connfd,
            cliaddr.sin_addr.s_addr & 0xFF,
            (cliaddr.sin_addr.s_addr & 0xFF00) >> 8,
            (cliaddr.sin_addr.s_addr & 0xFF0000) >> 16,
            (cliaddr.sin_addr.s_addr & 0xFF000000) >> 24,
            cliaddr.sin_port);

    free(arg);

    Request *request = Malloc(sizeof(request));
    strcpy(request->remote_addr, inet_ntoa(cliaddr.sin_addr));
    cgi_handler(connfd, request);

    pthread_detach(pthread_self());
    Close(connfd);
    return (NULL);
}

int main(int argc, const char *argv[])
{
    int     listenfd;
    Connection  *conn;
    pthread_t tid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    /* all init work */
    signal_init();

	listenfd = tcp_listen();

    log_info("Start listen on port %d", SERV_PORT);

    for(;;) {
        conn = Malloc(sizeof(Connection));
        clilen = sizeof(cliaddr);
        conn->connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
        conn->cliaddr = cliaddr;
        pthread_create(&tid, NULL, &run, conn);
    }

    /* should never go here */
    exit(EXIT_FAILURE);
}
