#include "html.h"

void html_init() {
    char *content = "<html> \
        <head> \
        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" /> \
        <title>Network Programming Homework 3</title> \
        </head> \
        <body bgcolor=#336699> \
        <font face=\"Courier New\" size=2 color=#FFFF99>";

    char *table_html = "<table width=\"800\" border=\"1\">\
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

void write_head_at(int num, char *content) {
    printf("<script>document.all['res_tr_head'].innerHTML += \"<td>%s</td>\";</script>", content);
}

void write_content_at(int num, char *content, int bold) {
    if(bold) {
        printf("<script>document.all('m%d').innerHTML += \"<b>%s</b>\";</script>", num, content);
    }
    else {
        printf("<script>document.all('m%d').innerHTML += \"%s\";</script>", num, content);
    }
    fflush(stdout);
    fflush(stdout);
    fflush(stdout);
}

void write_content_init(int num) {
    printf("<script>\
            document.all('res_tr_content').innerHTML += \"\
                <td id='m%d'></td>\";\
            </script>", num);

    /* hello message */
    printf("<script>document.all['m%d'].innerHTML += \"\
            ****************************************\
            <br>\";</script>", num);
    printf("<script>document.all['m%d'].innerHTML += \"\
            ** Welcome to the information server. **\
            <br>\";</script>", num);
    printf("<script>document.all['m%d'].innerHTML += \"\
            ****************************************\
            <br>\";</script>", num);
    printf("<script>document.all['m%d'].innerHTML += \"\
            ****************************************\
            <br>\";</script>", num);

    fflush(stdout);
    fflush(stdout);
    fflush(stdout);
}

