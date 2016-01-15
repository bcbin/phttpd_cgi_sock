#include "unp.h"

int
Socket(int family, int type, int protocol)
{
    int     n;
    if ( (n = socket(family, type, protocol)) < 0) {
        perror("[ERROR] Server - socket error");
        exit(0);
    }
    return (n);
}

void
SetReuseSock(int listenfd)
{
    int optval = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("[ERROR] Server - setsockopt");
    }
}

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0) {
        perror("[ERROR] Server - bind error");
        exit(0);
    }
}

void
Listen(int fd, int backlog)
{
    char    *ptr;

    if ( (ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if (listen(fd, backlog) < 0) {
        perror("[ERROR] Server - listen error");
        exit(0);
    }
}

int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int     n;

again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef  EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
        if (errno == ECONNABORTED)
#endif
            goto again;
        else {
            perror("[ERROR] Server - accept error");
            exit(0);
        }
    }
    return(n);
}

/**
 * Socket error message
 */

void
Socket_Message(int connfd, char *send_buff)
{
    memset(send_buff, 0, (int) sizeof(send_buff));
    snprintf(send_buff, 20, "Invalid Inputs.\n");
    Write(connfd, send_buff, strlen(send_buff));
}

/* Listen and bind a blockinng tcp socket */
int tcp_listen()
{
    int     listenfd;
    struct sockaddr_in servaddr;

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

	return listenfd;
}
