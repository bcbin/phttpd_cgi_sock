#include "unp.h"

typedef struct _conn {
    int connfd;
    struct sockaddr_in cliaddr;
} Connection;

static void *
doit(void *arg)
{
    int connfd;
    int read_size;
    struct sockaddr_in cliaddr;
    Connection conn;
    char *message, client_message[2000];

    conn = *((Connection *) arg);
    connfd = conn.connfd;
    cliaddr = conn.cliaddr;
    log_info("connfd=%d, cliaddr=%d", connfd, cliaddr.sin_port);

    free(arg);

    Request *request = Malloc(sizeof(request));
    strcpy(request->remote_addr, inet_ntoa(cliaddr.sin_addr));
    cgi_handler(connfd, request);

//     message = "Now type something and i shall repeat what you type \n";
//     write(connfd , message , strlen(message));
// 
//     while( (read_size = recv(connfd, client_message, 2000, 0)) > 0)
//     {
//     	client_message[read_size] = '\0';
// 
// 		//Send the message back to client
//         write(connfd , client_message , strlen(client_message));
// 
// 		//clear the message buffer
// 		memset(client_message, 0, 2000);
//     }

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
    struct sockaddr_in cliaddr, servaddr;

    /* all init work */
    signal_init();

	listenfd = tcp_listen();

    log_info("Start listen on port %d", SERV_PORT);

    for(;;) {
        conn = Malloc(sizeof(Connection));
        clilen = sizeof(cliaddr);
        conn->connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
        conn->cliaddr = cliaddr;
        pthread_create(&tid, NULL, &doit, conn);
// 
//         /* Fork to handle new connections */
//         pid_t pid;
//         pid = Fork();
// 
//         if (pid == 0) {             /* child */
// 
//             Request *request = Malloc(sizeof(request));
//             strcpy(request->remote_addr, inet_ntoa(cliaddr.sin_addr));
//             cgi_handler(connfd, request);
// 
//             exit(EXIT_SUCCESS);
// 
//         } else {                    /* parend */
// 
//             Close(connfd);
//         }
    }

    /* should never go here */
    exit(EXIT_FAILURE);
}
