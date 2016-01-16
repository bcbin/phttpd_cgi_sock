#include "unp.h"

int     listenfd;

typedef struct _conn {
    pthread_t thread_tid;
    int connfd;
    struct sockaddr_in cliaddr;
} Connection;
Connection *conns;

pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

static void * thread_main(void *arg)
{
    int i = *((int *) arg);
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    printf("thread %d starting\n", i);
    for (;;) {
        clilen = sizeof(cliaddr);

        pthread_mutex_lock(&mlock);
        conns[i].connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
        pthread_mutex_unlock(&mlock);

        /* print client ip and port */
        log_info("thread_id=%d, connfd=%d, ip=%u.%u.%u.%u (%d)",
                i, conns[i].connfd,
                cliaddr.sin_addr.s_addr & 0xFF,
                (cliaddr.sin_addr.s_addr & 0xFF00) >> 8,
                (cliaddr.sin_addr.s_addr & 0xFF0000) >> 16,
                (cliaddr.sin_addr.s_addr & 0xFF000000) >> 24,
                cliaddr.sin_port);

        conns[i].cliaddr = cliaddr;

        Request *request = Malloc(sizeof(request));
        strcpy(request->remote_addr, inet_ntoa(cliaddr.sin_addr));
        cgi_handler(conns[i].connfd, request);

        Close(conns[i].connfd);
    }

    free(arg);
}

void thread_make(int i)
{
    int *iptr = Malloc(sizeof(int));
    *iptr = i;

    pthread_create(&conns[i].thread_tid, NULL, &thread_main, iptr);
    return;
}

int main(int argc, const char *argv[])
{
    int     i;
    int     nthreads = 20;

    /* all init work */
    signal_init();

	listenfd = tcp_listen();

    log_info("Start listen on port %d", SERV_PORT);

    conns = calloc(nthreads, sizeof(Connection));

    log_info("Create a thread pool with %d threads", nthreads);
    for (i = 0; i < nthreads; i++) {
        thread_make(i);
    }

    //signal(SIGINT, sig_int);
    for(;;) {
        pause();
    }

    /* should never go here */
    exit(EXIT_FAILURE);
}
