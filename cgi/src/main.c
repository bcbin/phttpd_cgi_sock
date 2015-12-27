#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include "parser.h"
#include "request.h"
#include "html.h"
#include "client.h"

Request requests[REQUEST_MAX_NUM];

/* print debug message */
void print_requests_info()
{
    int i = 0;
    for(i = 0; i < REQUEST_MAX_NUM; i++) {
        requests[i].socket = 0;

        printf("requests[%d] ip=%s port=%s filename=%s sock_ip=%s sock_port=%s<br>",
            i+1, requests[i].ip, requests[i].port, requests[i].filename,
            requests[i].sock_ip, requests[i].sock_port);
    }
}

void request_iterator()
{
    int i = 0;
    for(i = 0; i < REQUEST_MAX_NUM; i++) {
        if (!(requests[i].ip && requests[i].port)) continue;
        serve_request_at(i, 'm');
        save_request_fileptr(i);
    }
}

int main(int argc, const char *argv[])
{
    html_init();

    // getenv
    char *query = getenv("QUERY_STRING");
    // char query[] = "h1=127.0.0.1&p1=9878&f1=t1.txt&h2=192.168.1.111&p2=9878&f2=t2.txt&h3=&p3=&f3=&h4=&p4=&f4=&h5=127.0.0.1&p5=1111&f5=asdf";

    /* init requests */
    int i = 0;
    for(i = 0; i < REQUEST_MAX_NUM; i++) {
        requests[i].socket = 0;
    }

    if (query)
    {
        parse_query_string(query);
        if (DEBUG)
            print_requests_info();

        request_iterator();
        clients_handler();
    } else {
        log_err("Empty query string.");
    }

    html_end();

    return 0;
}
