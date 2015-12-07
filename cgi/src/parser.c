#include "parser.h"

extern Request requests[5];

int parse_key_value(char *token, int index)
{
    char *value_tok,
         *saveptr;

    /* get key */
    value_tok = strtok_r(token, "=", &saveptr);
    printf("key - %s<br>", value_tok);

    if (value_tok[0] == 'h') {
        /* store value */
        value_tok = strtok_r(NULL, " ", &saveptr);
        printf("value - %s<br>", value_tok);
        requests[index].ip = malloc(REQUEST_HOST_SIZE);
        //strcpy(requests[index].ip, token);
    }
    else if (value_tok[0] == 'p') {
        /* store value */
        value_tok = strtok_r(NULL, " ", &saveptr);
        printf("value - %s<br>", value_tok);
        requests[index].port = malloc(REQUEST_PORT_SIZE);
        //strcpy(requests[index].port, token);
    }
    else if (value_tok[0] == 'f') {
        /* store value */
        value_tok = strtok_r(NULL, " ", &saveptr);
        printf("value - %s<br>", value_tok);
        requests[index].filename = malloc(REQUEST_FILENAME_SIZE);
        //strcpy(requests[index].filename, token);
    }

}

int parse_query_string(char *qs)
{
    char *token,
         *saveptr;

    if (!qs) return -1;

    token = strtok_r(qs, "&", &saveptr);

    int  i = 0;
    while(token) {
        parse_key_value(token, i);
        printf("<hr>");
        token = strtok_r(NULL, "&", &saveptr);

        i++;
    }

    return 0;
}

/**
 * Parse given line to seperate requests
 */

int parse_split_query(char *query, Request **request)
{
    printf("\n[BEFORE] - parse_query_string=%s\n", query);
}
