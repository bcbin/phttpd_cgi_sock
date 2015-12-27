#include "unp.h"

/*-------------------------------------------------------------*/
/*--------------------- response helper -----------------------*/
/*-------------------------------------------------------------*/

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

void write_bad_request(int connfd)
{
    char *response = "HTTP/1.1 400 Bad Request\nContent-Type: text/html\n\n";
    Write(connfd, response, strlen(response));

    /* print bad request body */
    char *content = "400 Bad Request\n";
    Write(connfd, content, strlen(content));
}

/**
 * Read whole file into a given buffer
 */

int readfile_into_buf(char *filename, char *buf)
{
    FILE *file_ptr = fopen(filename, "r");

    if (file_ptr == NULL) {
        log_err("fopen failed");
        return -1;
    }

    fseek(file_ptr, 0, SEEK_END);
    int fsize = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    memset(buf, 0, BUF_SIZE);
    if (fsize != fread(buf, fsize, 1, file_ptr)) {
        log_err("fread failed");
    }

    return 0;
}

/**
 * execute cgi program
 */

void cgi_exec(int connfd, char *path, Request *request)
{
    pid_t pid;
    pid = Fork();

    if (pid == 0) {                 /* child */

        write_cgi_header(connfd);
        Dup2(connfd, STDIN_FILENO);
        Dup2(connfd, STDOUT_FILENO);

        if (execl(path, "", NULL) < 0) {
            write_bad_request(connfd);
        }

        exit(EXIT_SUCCESS);

    } else if (pid > 0) {           /* parent */

        int return_val;
        waitpid(pid, &return_val, 0);
        if (WEXITSTATUS(return_val) == 1) return;
    }
}

int cgi_handler(int connfd, Request *request)
{
    char buf[BUF_SIZE], params[BUF_SIZE];
    Read(connfd, buf, BUF_SIZE);

    /* set content_length */
    char *content_length = Malloc(sizeof(ssize_t));
    sprintf(content_length, "%ld", strlen(buf));
    setenv2("CONTENT_LENGTH", content_length);

    /* parse request_method and remote_addr */
    parse_request_method(buf, request);
    setenv2("REMOTE_ADDR", request->remote_addr);

    /* set arbitrary env for cgi program */
    setenv2("SCRIPT_NAME", "/~0453411/public_html/");
    setenv2("REMOTE_HOST", "nplinux2.cs.nctu.edu.tw");
    setenv2("AUTH_TYPE", "normal");
    setenv2("REMOTE_USER", "guest");
    setenv2("REMOTE_IDENT", "unknown");

    /* parse program path */
    parse_params(params, request);
    char root[50] = "../www/";

    if (DEBUG) {
        //fprintf(stderr, "%s\n", buf);
        fprintf(stderr, "\n==================== PARSE RESULT ======================\n");
        fprintf(stderr, "Content-Length=%s\n", content_length);
        // fprintf(stderr, "request_method=%s\n", request->request_method);
        fprintf(stderr, "params=%s\n", params);
        fprintf(stderr, "============================= END ========================\n");
    }


    switch (parse_extension(params)) {
        case CGI:
            strcat(root, params);
            cgi_exec(connfd, root, request);

            break;

        case HTML:
            strcat(root, params);
            //fprintf(stderr, "html root=%s", root);

            /* read file into buf */
            if (readfile_into_buf(root, buf) == -1) {
                log_warn("html not found");
                write_bad_request(connfd);
                break;
            }

            write_html_header(connfd);
            Write(connfd, buf, strlen(buf));

            break;

        default:
            /* request with other extentions */
            strcat(root, params);
            //fprintf(stderr, "html root=%s", root);

            /* read file into buf */
            if (readfile_into_buf(root, buf) == -1) {
                log_warn("not found");
                write_bad_request(connfd);
                break;
            }

            write_html_header(connfd);
            Write(connfd, buf, strlen(buf));
    }

    Close(connfd);
    return 0;
}
