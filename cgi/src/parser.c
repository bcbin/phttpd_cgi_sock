#include "parser.h"

extern Request requests[5];
extern Sock sock_req[5];

void parse_key_value(char *token)
{
    int index = 0;
    char *value_tok,
         *saveptr,
         capital,
         second;

    /* get key */
    value_tok = strtok_r(token, "=", &saveptr);
    //printf("key - %s<br>", value_tok);
    sscanf(value_tok, "%c%d", &capital, &index);
    //printf("capital-%c index-%d<br>", capital, index);
    index--;

    /* store value */
    if (capital == 'h') {

        value_tok = strtok_r(NULL, "\0", &saveptr);

        if (value_tok) {
            requests[index].ip = malloc(REQUEST_HOST_SIZE);
            sock_req[index].request.ip = malloc(REQUEST_FILENAME_SIZE);
            strcpy(requests[index].ip, value_tok);
            strcpy(sock_req[index].request.ip, value_tok);
            //printf("value - %s<br>", requests[index].ip);
        }
    }
    else if (capital == 'p') {

        value_tok = strtok_r(NULL, "\0", &saveptr);

        if (value_tok) {
            requests[index].port = malloc(REQUEST_PORT_SIZE);
            sock_req[index].request.port = malloc(REQUEST_PORT_SIZE);
            strcpy(requests[index].port, value_tok);
            strcpy(sock_req[index].request.port, value_tok);
            //printf("value - %s<br>", requests[index].port);
        }
    }
    else if (capital == 'f') {

        value_tok = strtok_r(NULL, "\0", &saveptr);

        if (value_tok) {
            requests[index].filename = malloc(REQUEST_FILENAME_SIZE);
            sock_req[index].request.filename = malloc(REQUEST_FILENAME_SIZE);
            strcpy(requests[index].filename, value_tok);
            strcpy(sock_req[index].request.filename, value_tok);
            //printf("value - %s<br>", requests[index].filename);
        }
    }
    else if (capital == 's') {

        sscanf(value_tok, "%c%c%d", &capital, &second, &index);
        index--;
        //printf("capital: %c, second: %c\n", capital, second);

        if (second == 'h') {

            value_tok = strtok_r(NULL, "\0", &saveptr);

            if (value_tok) {
                sock_req[index].ip = malloc(REQUEST_HOST_SIZE);
                strcpy(sock_req[index].ip, value_tok);
                //printf("value - %s<br>", sock_req[index].request.ip);
            }
        }
        else if (second == 'p') {

            value_tok = strtok_r(NULL, "\0", &saveptr);

            if (value_tok) {
                sock_req[index].port = malloc(REQUEST_PORT_SIZE);
                strcpy(sock_req[index].port, value_tok);
                //printf("value - %s<br>", sock_req[index].request.port);
            }
        }
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
        parse_key_value(token);
        token = strtok_r(NULL, "&", &saveptr);

        i++;
    }

    return 0;
}
