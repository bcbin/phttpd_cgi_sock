#include "unp.h"

/** helper **/

void write_cgi_header(int connfd)
{
    char *content = "HTTP/1.1 200 OK\n";
    Write(connfd, content, strlen(content));
}

void write_html_header(int connfd)
{
    char *content = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
    Write(connfd, content, strlen(content));
}

/**
 * Read whole file into a given buffer
 */

void readfile_into_buf(char *filename, char *buf)
{
    FILE *file_ptr = fopen(filename, "r");

    if (file_ptr == NULL) {
        log_err("fopen failed");
    }

    fseek(file_ptr, 0, SEEK_END);
    int fsize = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    memset(buf, 0, BUF_SIZE);
    if (fsize != fread(buf, fsize, 1, file_ptr)) {
        log_err("fread failed");
    }
}

int cgi_handler(int connfd, Request *request)
{
    pid_t pid;
    pid = Fork();

    if (pid == 0) {                 /* child */

        char buf[BUF_SIZE], params[BUF_SIZE];
        Read(connfd, buf, BUF_SIZE);
        sprintf(request->content_length, "%ld", strlen(buf));
        fprintf(stderr, "%s\n", buf);
        fprintf(stderr, "\n==================== PARSE RESULT ======================\n");
        fprintf(stderr, "Content-Length=%s\n", request->content_length);
        parse_request_method(buf, request);
        fprintf(stderr, "request_method=%s\n", request->request_method);
        parse_params(params, request);
        fprintf(stderr, "params=%s\n", params);
        fprintf(stderr, "============================= END ========================\n");

        char root[50] = "../www/";

        switch (parse_extension(params)) {
            case CGI:
                write_cgi_header(connfd);
                Dup2(connfd, STDIN_FILENO);
                Dup2(connfd, STDOUT_FILENO);

                strcat(root, params);
                fprintf(stderr, "root=%s", root);

                /* set cgi env */
                //setenv2("QUERY_STRING", request->query_string);
                setenv2("CONTENT_LENGTH", request->content_length);
                setenv2("REQUEST_METHOD", request->request_method);
                //setenv2("SCRIPT_NAME", request->script_name);
                //setenv2("REMOTE_HOST", request->remote_host);
                setenv2("REMOTE_ADDR", request->remote_addr);

                if (execl(root, "", NULL) < 0) {
                    perror("execl");
                }

                break;

            case HTML:
                strcat(root, params);
                fprintf(stderr, "html root=%s", root);

                /* read file into buf */
                readfile_into_buf(root, buf);

                write_html_header(connfd);
                Write(connfd, buf, strlen(buf));

                break;

            default:
                /* request with other extentions */
                strcat(root, params);
                fprintf(stderr, "html root=%s", root);

                /* read file into buf */
                readfile_into_buf(root, buf);

                write_html_header(connfd);
                Write(connfd, buf, strlen(buf));
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
