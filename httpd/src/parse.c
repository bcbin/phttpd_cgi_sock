#include "unp.h"

/* parse request method into request struct */

void parse_request_method(char *buf, Request *request)
{
    char *r;
    r = strtok(buf, " ");
    strncpy(request->request_method, r, strlen(r));
    request->request_method[strlen(r)] = '\0';
}

/* parse request paramaters */

void parse_params(char *params, Request *request)
{
    char *r;
    r = strtok(NULL, " ");
    //fprintf(stderr, "r=%s", r);
    /* r+1 cutoff '/' */
    strncpy(params, r+1, strlen(r)-1);
    params[strlen(r)-1] = '\0';

    /* cutoff query_string */
    params = strtok(params, "?");

    /* get query_string */
    char *qs;
    qs = strtok(NULL, " ");
    strncpy(request->query_string, qs, strlen(qs));
    request->query_string[strlen(qs)] = '\0';
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
