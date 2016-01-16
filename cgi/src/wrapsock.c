#include "wrapsock.h"

int
Socket(int family, int type, int protocol)
{
    int     n;
    if ( (n = socket(family, type, protocol)) < 0) {
        perror("[ERROR] Server - socket error");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
        }
    }
    return(n);
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
	int		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0) {
		perror("[ERROR] Server - select error");
        exit(EXIT_FAILURE);
    }
	return(n);		/* can return 0 on timeout */
}

/* non thread safe */
struct hostent* Gethostbyname(const char *name)
{
    struct hostent *server;
    server = gethostbyname(name);

    if (server == NULL) {
        log_err("no such host");

        return NULL;
    }

    return server;
}

/* Socket error message */
void
Socket_Message(int connfd, char *send_buff)
{
    memset(send_buff, 0, (int) sizeof(send_buff));
    snprintf(send_buff, 20, "Invalid Inputs.\n");
    Writen(connfd, send_buff, strlen(send_buff));
}
