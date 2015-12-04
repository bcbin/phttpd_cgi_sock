#include "parser.h"

int parse_query_string(char *qs, List *list)
{
    char *token,
         *saveptr;

    if (!qs) return -1;

    token = strtok_r(qs, "&", &saveptr);

    while(token) {
        List_push(list, token);
        token = strtok_r(NULL, "&", &saveptr);
    }

    return 0;
}
