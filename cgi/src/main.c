#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

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

    // query string

    html_end();

    return 0;
}
