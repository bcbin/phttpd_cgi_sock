#include "unp.h"

/* parse buf into request object and setenv */
void parse_request(char *buf, Request *request, char *params)
{
    char *request_method;
    char *rs;
    char *save_ptr;
    request_method = strtok_r(buf, " ", &save_ptr);

    setenv2("REQUEST_METHOD", request_method);
    setenv2("REMOTE_ADDR", request->remote_addr);

    rs = strtok_r(NULL, " ", &save_ptr);

    /* r+1 cutoff '/' */
    strncpy(params, rs+1, strlen(rs)-1);
    params[strlen(rs)-1] = '\0';

    /* cutoff query_string */
    params = strtok_r(params, "?", &save_ptr);

    /* get query_string */
    char *qs;
    qs = strtok_r(NULL, " ", &save_ptr);

    if (!qs) {
        setenv2("QUERY_STRING", "");
        return;
    }
    setenv2("QUERY_STRING", qs);

    if (DEBUG) {
        log_info("params=%s", params);
        log_info("request_method=%s", request_method);
        log_info("remote_addr=%s", request->remote_addr);
        log_info("request qs=%s\n", qs);
    }
}

/* parse the extension with the given filename
 * return the enum{CGI, HTML, OTHER} as result */
int parse_extension(char *filename)
{
    const char *last_dot = strrchr(filename, '.');

    if (strcmp(last_dot+1, "cgi") == 0) {
        if (DEBUG) fprintf(stderr, "cgi\n");
        return CGI;
    } else if (strcmp(last_dot+1, "html") == 0) {
        if (DEBUG) fprintf(stderr, "html\n");
        return HTML;
    }

    return OTHER;
}
