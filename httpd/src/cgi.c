#include "unp.h"

int cgi_handler(int connfd)
{
    // cgi env variables
    ssize_t content_length;
    char *query_string,
         *request_method,
         *script_name,
         *remote_host,
         *remote_addr;

    pid_t pid;
    pid = Fork();

    if (pid == 0) {                 /* child */

        char buf[BUF_SIZE];
        Read(connfd, buf, BUF_SIZE);
        content_length = strlen(buf);
        parse_requset(buf);

        fprintf(stderr, "%s\n", buf);
        fprintf(stderr, "\n==========================================================\n");
        fprintf(stderr, "Content-Length=%ld\n", content_length);
        fprintf(stderr, "==========================================================\n");

        // exec cgi
        char *content = "HTTP/1.1 200 OK\n";
        Write(connfd, content, content_length);
        Dup2(connfd, STDIN_FILENO);
        Dup2(connfd, STDOUT_FILENO);
        if (execl("../www/hello.cgi", "", NULL) < 0) {
            perror("execl");
        }

        // normal file open

        exit(EXIT_SUCCESS);

    } else if (pid > 0) {           /* parent */

        int return_val;
        waitpid(pid, &return_val, 0);
        if (WEXITSTATUS(return_val) == 1) return 1;
    }

    Close(connfd);
    return 0;
}
