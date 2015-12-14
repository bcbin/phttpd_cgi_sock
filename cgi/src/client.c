#include "client.h"

int gSc = 0;
int recv_msg(int from);
int readline(int fd, char *ptr, int maxlen);

int recv_msg(int from)
{
    char buf[3000];
    int len;
    if((len=readline(from,buf,sizeof(buf)-1)) <0) return -1;
    buf[len] = 0;
    printf("%s",buf);   //echo input
    fflush(stdout);
    return len;
}

int readline(int fd,char *ptr,int maxlen)
{
    int n,rc;
    char c;
    *ptr = 0;
    for(n=1;n<maxlen;n++)
    {
        if((rc=read(fd,&c,1)) == 1)
        {
            *ptr++ = c;
            if(c==' '&& *(ptr-2) =='%'){ gSc = 1; break; }
            if(c=='\n')  break;
        }
        else if(rc==0)
        {
            if(n==1)     return(0);
            else         break;
        }
        else
            return(-1);
    }
    return(n);
}

void new_connection(int index)
{
    fd_set readfds;
    int client_fd;
    struct sockaddr_in client_sin;
    char msg_buf[30000];
    int len, SERVER_PORT, end;
    FILE *fp;
    struct hostent *he;
    gSc = 0;

    /* open file */
    char filepath[1000];
    strcpy(filepath, "test/");
    strcat(filepath, requests[index].filename);
    log_info("filepath=%s", filepath);
    fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "open file");
        exit(EXIT_FAILURE);
    }

    /* get host info */
    if((he=gethostbyname(requests[index].ip)) == NULL)
    {
        fprintf(stderr,"host error");
        exit(EXIT_FAILURE);
    }

    /* bind and connect */
    SERVER_PORT = (u_short) atoi(requests[index].port);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&client_sin, sizeof(client_sin));
    client_sin.sin_family = AF_INET;
    client_sin.sin_addr = *((struct in_addr *)he->h_addr);
    client_sin.sin_port = htons(SERVER_PORT);
    if(connect(client_fd, (struct sockaddr *)&client_sin, sizeof(client_sin)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    end = 0;
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(client_fd, &readfds);
        if (end == 0) {
            FD_SET(fileno(fp), &readfds);
        }
        if (select(client_fd+1, &readfds, NULL, NULL, NULL) < 0) {
            exit(EXIT_FAILURE);
        }

        /* readline */
        if (gSc == 1) {

            len = readline(fileno(fp), msg_buf, sizeof(msg_buf));
            if (len < 0) exit(1);

            msg_buf[len - 1] = 13;
            msg_buf[len] = 10;

            msg_buf[len + 1] = '\0';
            printf("%s", msg_buf);
            fflush(stdout);
            if (write(client_fd, msg_buf, len+1) == -1) {
                return;
            }

            gSc = 0;
        }

        /* */
        if (FD_ISSET(client_fd, &readfds)) {
            int errnum;
            errnum = recv_msg(client_fd);
            if (errnum < 0) {
                shutdown(client_fd, 2);
                close(client_fd);
                exit(1);
            } else if (errnum == 0) {
                shutdown(client_fd, 2);
                close(client_fd);
                exit(0);
            }
        }
    } // end of while
}

void clients_handler()
{
    int i = 0;
    for(i = 0; i < REQUEST_MAX_NUM; i++) {
        if (!(requests[i].ip && requests[i].port)) continue;
        /* connect */
        new_connection(i);
    }
}
