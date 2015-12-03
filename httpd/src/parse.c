#include "unp.h"

/* parse request paramaters */

void parse_params(char *buf, char *params)
{
    char *r;
    strtok(buf, " ");
    r = strtok(NULL, " ");
    strncpy(params, r+1, strlen(r)-1);
}

void parse_query_string()
{
    char *query_string = strtok(NULL, "");
}

/* parse the extension with the given filename
 * return the enum{CGI, HTML, OTHER} as result */

int parse_extension(char *filename)
{
    fprintf(stderr, "parse_extension, %s\n", filename);
    const char *last_dot = strrchr(filename, '.');

    if (strcmp(last_dot+1, "cgi") == 0) {
        fprintf(stderr, "cgi\n");
        return CGI;
    } else if (strcmp(last_dot+1, "html") == 0) {
        fprintf(stderr, "html\n");
        return HTML;
    }

    return OTHER;
}
