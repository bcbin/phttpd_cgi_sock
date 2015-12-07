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

    parse_query_string(query);

    html_end();

    // free requests

    return 0;
}
