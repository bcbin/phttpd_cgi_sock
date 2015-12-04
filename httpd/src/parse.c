#include "unp.h"

/* parse request method into request struct */

void parse_request_method(char *buf, Request *request)
{
    char *r;
    r = strtok(buf, " ");
    strncpy(request->request_method, r, strlen(r));
}

/* parse request paramaters */

void parse_params(char *params, Request *request)
{
    char *r;
    r = strtok(NULL, " ");
    //fprintf(stderr, "r=%s", r);
    /* r+1 cutoff '/' */
    strncpy(params, r+1, strlen(r)-1);

// TODO has bug
//     /* save query_string into request struct */
//     strcpy(request->query_string, strchr(params, '?')+1);
//     fprintf(stderr, "qs=%s", request->query_string);

    /* cutoff query_string */
    params = strtok(params, "?");
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
