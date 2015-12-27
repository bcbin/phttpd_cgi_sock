#ifndef __WRAPSOCK_H
#define __WRAPSOCK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "dbg.h"
#include "html.h"
#include "writen.h"

#define SA  struct sockaddr

int Socket(int family, int type, int protocol);
void SetReuseSock(int listenfd);
void Bind(int, const SA *, socklen_t);
void Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
int Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
struct hostent* Gethostbyname(const char *name);
void Socket_Message(int connfd, char *send_buff);

#endif
