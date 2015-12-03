#include "unp.h"

int cgi_handler(int connfd)
{
    // cgi env variables
    ssize_t content_length;
    char *param,
         *query_string,
         *request_method,
         *script_name,
         *remote_host,
         *remote_addr;

    pid_t pid;
    pid = Fork();

    if (pid == 0) {                 /* child */

        char buf[BUF_SIZE], params[BUF_SIZE];
        Read(connfd, buf, BUF_SIZE);
        content_length = strlen(buf);
        fprintf(stderr, "%s\n", buf);
        fprintf(stderr, "\n==========================================================\n");
        fprintf(stderr, "Content-Length=%ld\n", content_length);
        parse_params(buf, params);
        fprintf(stderr, "parse params=%s\n", params);
        fprintf(stderr, "==========================================================\n");

        char *response_content,
             root[50] = "../www/";

        switch (parse_extension(params)) {
            case CGI:
                // exec cgi
                response_content= "HTTP/1.1 200 OK\n";
                Write(connfd, response_content, content_length);
                Dup2(connfd, STDIN_FILENO);
                Dup2(connfd, STDOUT_FILENO);

                strcat(root, params);
                fprintf(stderr, "root=%s", root);

                if (execl(root, "", NULL) < 0) {
                    perror("execl");
                }

                break;
            default:
                fprintf(stderr, "TODO print html/txt\n" );
                // normal file open and write it
        }

        exit(EXIT_SUCCESS);

    } else if (pid > 0) {           /* parent */

        int return_val;
        waitpid(pid, &return_val, 0);
        if (WEXITSTATUS(return_val) == 1) return 1;
    }

    Close(connfd);
    return 0;
}
