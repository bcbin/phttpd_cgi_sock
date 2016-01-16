
#ifndef __unp_h
#define __unp_h

#include <sys/socket.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <netdb.h>
#include <arpa/inet.h>  /* inet(3) functions */
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "dbg.h"

#define TRUE        1
#define FALSE       0

/* shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define BACKLOG             1024
#define SERV_PORT           9878
#define BUF_SIZE            10000

#define DEBUG               0

/* color */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

// socket
int Socket(int family, int type, int protocol);
void SetReuseSock(int listenfd);
void Bind(int, const SA *, socklen_t);
void Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Socket_Message(int connfd, char *send_buff);

// unix
void Write(int fd, void *ptr, size_t nbytes);
int Open(const char *pathname, int oflag, mode_t mode);
void Close(int fd);
void Unlink(const char *pathname);
void Execl(const char *path, char *arg);
ssize_t Read(int fd, void *ptr, size_t nbytes);
void Writen(int fd, void *ptr, size_t nbytes);
void *Malloc(size_t size);
void Pipe(int *fds);
pid_t Fork(void);
void Dup2(int fd1, int fd2);
void Log(char *);
void Chdir(const char *);
void setenv2(const char *, const char *);
void Select(int nfds, fd_set *readfds, fd_set *writefds,
                          fd_set *exceptfds, struct timeval *timeout);
int Shmget(key_t key, size_t size, int flags);
void * Shmat(int id, const void *shmaddr, int flags);
void Shmdt(const void *shmaddr);
void Shmctl(int shmid, int cmd, struct shmid_ds *buf);
void Mknod(const char *pathname, mode_t mode, dev_t dev);

#endif
