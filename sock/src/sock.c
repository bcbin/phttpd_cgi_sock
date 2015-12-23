#include "unp.h"

#define WRITE_BUF_SIZE 8
#define BYTE           0x000000FF
#define IP_LENGTH      20
#define PORT_LENGTH    10

typedef struct sock {
    unsigned char VN;
    unsigned char CD;
    unsigned short DST_PORT;
    unsigned int DST_IP;
    // char* USER_ID;
} __attribute__((packed)) Sock;

/* debug */
void print_packet(char* buf, int size)
{
    int i;
    for(i = 0; i < size; i++) {
        fprintf(stderr, "%x ", buf[i] & BYTE);
    }
    fprintf(stderr, "\n");
}

int deny_access()
{
    return -1;
}

char *get_port_str(char *buf)
{
    char *temp = Malloc(PORT_LENGTH);
    unsigned int port = ((buf[2] & BYTE) << 8) + (buf[3] & BYTE);
    sprintf(temp, "%u", port);
    return temp;
}

char *get_ip_str(char *buf)
{
    char *temp = Malloc(IP_LENGTH);
    sprintf(temp, "%u.%u.%u.%u",
            buf[4] & BYTE,
            buf[5] & BYTE,
            buf[6] & BYTE,
            buf[7] & BYTE);
    return temp;
}

/* return connected socket if success, return 0 if fail */
int connectTCP(char *ip, char *port)
{
    int sockfd;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    /* create the socket */
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    /* get server */
    server = gethostbyname(ip);
    if (server == NULL) {
        log_err("no such server");
        return 0;
    }

    /* setup socket */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(atoi(port));

    if (connect(sockfd, (struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0) {
        log_err("ERROR connecting");
        return 0;
    }

    return sockfd;
}

/* Establish a passive tcp connection using a random port
 * pass port as value result argument and return psock if success */
int passiveTCP(unsigned short *port)
{
    int listenfd;
    socklen_t addrlen = sizeof(struct sockaddr);
    struct sockaddr_in servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(0);      /* important!! */

    SetReuseSock(listenfd);
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    getsockname(listenfd, (struct sockaddr *) &servaddr, &addrlen); // -> network byte order
    *port = servaddr.sin_port;

    Listen(listenfd, BACKLOG);

    return listenfd;
}

void redirect_socket_data(int ssock, int rsock)
{
    fd_set fds, rfds;
    struct timeval timeout;     // second, microsecs
    timeout.tv_sec = 120;
    timeout.tv_usec = 0;

    int max_fd = 10;
    char buf[BUF_SIZE];

    FD_ZERO(&fds);
    FD_SET(ssock, &fds);
    FD_SET(rsock, &fds);

    while(1) {
        rfds = fds;
        Select(max_fd, &rfds, NULL, NULL, &timeout);

        // client pass chunks
        if (FD_ISSET(ssock, &rfds)) {
            ssize_t n = Read(ssock, buf, BUF_SIZE);
            if (n > 0) {
                Writen(rsock, buf, n);
            } else {
                break;
            }
        }
        // remote pass chunks
        else if (FD_ISSET(rsock, &rfds)) {
            ssize_t n = Read(rsock, buf, BUF_SIZE);
            if (n > 0) {
                Writen(ssock, buf, n);
            } else {
                break;
            }
        }
    }

    Close(ssock);
    Close(rsock);
}

void proxy_handler(int sockfd)
{
    int i,
        rsock,      /* connect mode */
        psock;      /* bind mode */
    char read_buf[BUF_SIZE],
         write_buf[BUF_SIZE];

    Read(sockfd, read_buf, BUF_SIZE);

    /* extract info from SOCK request packet */
    unsigned char VN = read_buf[0];
    unsigned char CD = read_buf[1];
    char *port = get_port_str(read_buf);
    char *ip = get_ip_str(read_buf);
    char *USER_ID = read_buf + 8;

    fprintf(stderr, "VN: %d, CD: %d, IP: %s, PORT: %s\n", VN, CD, ip, port);

    /* connect mode */
    if (CD == 1)
    {
        fprintf(stderr, "CD=1 CONNECT\n");

        /* fill sock write packet */
        write_buf[0] = 0;
        write_buf[1] = (unsigned char) 90; // 90 for granted 91 for reject
        for (i = 2; i < 7; i++) {
            /* reference to read_buf */
            write_buf[i] = read_buf[i];
        }

        rsock = connectTCP(ip, port);   /* connect to remote server */
        if (rsock <= 0) {
            perror("connect error\n");
            exit(EXIT_FAILURE);
        }

        /* SOCK4 reply */
        //print_packet(write_buf, WRITE_BUF_SIZE);
        if (write(sockfd, write_buf, WRITE_BUF_SIZE) < 0){
            log_err("sock(connect) reply error");
        }

        redirect_socket_data(sockfd, rsock);
        //fprintf(stderr, "grant/non-grant\n");
    }
    /* bind mode */
    else if (CD == 2)
    {
        fprintf(stderr, "CD=2 BIND\n");

        unsigned short port;
        psock = passiveTCP(&port);
        fprintf(stderr, "passive psock=%d port = %d\n", psock, port);
        port = ntohs(port);

        /* fill sock write packet */
        write_buf[0] = 0;
        write_buf[1] = (unsigned char) 90; // 90 for granted 91 for reject
        write_buf[2] = port / 256;
        write_buf[3] = port % 256;
        for(i = 4; i < WRITE_BUF_SIZE; i++) {
            write_buf[i] = 0;
        }

        print_packet(write_buf, WRITE_BUF_SIZE);
        Writen(sockfd, write_buf, WRITE_BUF_SIZE);

        struct sockaddr_in servaddr;
        socklen_t addrlen = sizeof(servaddr);
        rsock = Accept(psock, (SA *) &servaddr, &addrlen);
        log_info("rsock=%d", rsock);

        Close(psock);

        Writen(sockfd, write_buf, WRITE_BUF_SIZE);

        redirect_socket_data(sockfd, rsock);
        //fprintf(stderr, "grant/non-grant\n");
    }
}

int main(int argc, const char *argv[])
{
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    SetReuseSock(listenfd);
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, BACKLOG);

    log_info("Start listen on port %d", SERV_PORT);

    /* nonblock signal child */
    signal(SIGCHLD, SIG_IGN);

    for(;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        pid_t pid;
        pid = Fork();

        if (pid == 0) {

            proxy_handler(connfd);

            exit(EXIT_SUCCESS);

        } else {

            Close(connfd);
        }
    }

    /* should never go here */
    exit(EXIT_FAILURE);
}
