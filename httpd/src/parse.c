#include "unp.h"

/* parse request method into request struct */

void parse_request_method(char *buf, Request *request)
{
    char *request_method;
    request_method = strtok(buf, " ");

    if (DEBUG)  fprintf(stderr, "request_method=%s\n", request_method);
    setenv2("REQUEST_METHOD", request_method);
}

/* parse request paramaters */

void parse_params(char *params, Request *request)
{
    char *r;
    r = strtok(NULL, " ");

    /* r+1 cutoff '/' */
    strncpy(params, r+1, strlen(r)-1);
    params[strlen(r)-1] = '\0';

    /* cutoff query_string */
    params = strtok(params, "?");

    /* get query_string */
    char *qs;
    qs = strtok(NULL, " ");
    if (DEBUG)
        fprintf(stderr, "request qs=%s\n", qs);

    if (!qs) return;
    setenv2("QUERY_STRING", qs);
// BUG
//     strncpy(request->query_string, qs, strlen(qs));
//     request->query_string[1000] = '\0';
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
