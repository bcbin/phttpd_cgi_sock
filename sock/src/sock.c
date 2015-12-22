#include "unp.h"

#define WRITE_BUF_SIZE 8
#define BYTE        0x000000FF
#define IP_LENGTH      20
#define PORT_LENGTH    10

char buf[BUF_SIZE];

/* debug */
void print_packet(char* buf, int size)
{
    for(int i = 0; i < size; i++) {
        printf("%x ", buf[i]);
    }
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

int connect_tcp()
{

    return 0;
}

int passive_tcp()
{
    return 0;
}

void proxy_handler(int sockfd)
{
    int i;
    char read_buf[BUF_SIZE],
         write_buf[BUF_SIZE];

    ssize_t n = Read(sockfd, read_buf, BUF_SIZE);

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

        write_buf[0] = 0;
        write_buf[1] = (unsigned char) 90; // 90 for granted 91 for reject
        for (i = 0; i < 7; i++) {
            /* reference to read_buf */
            write_buf[i] = read_buf[i];
        }

        //print_packet(write_buf, strlen(write_buf));
        Write(sockfd, write_buf, WRITE_BUF_SIZE);

        connect_tcp();

        //fprintf(stderr, "grant/non-grant\n");
    }
    /* bind mode */
    else if (CD == 2)
    {
        log_info("SOCKS_BIND");

        fprintf(stderr, "grant/non-grant\n");
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
