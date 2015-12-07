#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include "parser.h"
#include "list.h"
#include "request.h"

Request requests[5];

void html_init() {
    char *content = "<html> \
        <head> \
        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" /> \
        <title>Network Programming Homework 3</title> \
        </head> \
        <body style=\"font-family: 'Courier New', Courier, monospace;\">\
        <style>\
        td {\
            font-size: small;\
            vertical-align: top;\
        }\
        </style>";
    char *table_html = "<table id=\"result_table\" width=\"800\" border=\"1\">\
                        <tr id=\"res_tr_head\"></tr>\
                        <tr id=\"res_tr_content\"></tr>\
                        </table>";
    printf("Content-Type: text/html\n\n");
    printf("%s%s", content, table_html);

}

void html_end() {
    char *content = "</body></html>";
    printf("%s", content);
}

void print_requests_info()
{
    int i = 0;
    for(i; i < 5; i++) {
        requests[i].socket = 0;
        printf("requests[%d] ip=%s port=%s filename=%s<br>",
            i+1, requests[i].ip, requests[i].port, requests[i].filename);
    }
}

int main(int argc, const char *argv[])
{
    html_init();

    /* init requests */
    int i = 0;
    for(i; i < 5; i++) {
        requests[i].socket = 0;
    }

    // getenv
    char *query = getenv("QUERY_STRING");
    //char query[] = "h1=127.0.0.1&p1=9877&f1=test1.txt&h2=192.168.1.111&p2=9878&f2=test2.txt&h3=&p3=&f3=&h4=&p4=&f4=&h5=127.0.0.1&p5=1111&f5=asdf";

    parse_query_string(query);
    print_requests_info();

    html_end();

    // free requests

    return 0;
}
