#include "html.h"

/* print html head and init body */
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

/* end of html body */
void html_end() {
    char *content = "</body></html>";
    printf("%s", content);
}

/* write head at a given section of server# */
void write_head_at(int num, char *content) {
    printf("<script>document.all['res_tr_head'].innerHTML += \"<td>%s</td>\";</script>", content);
}

/* write html content at given section of server# */
void write_content_at(int num, char *content, int bold) {
    if(bold) {
        printf("<script>document.all('m%d').innerHTML += \"<b>%s</b>\";</script>", num, content);
    }
    else {
        printf("<script>document.all('m%d').innerHTML += \"%s\";</script>", num, content);
    }
    fflush(stdout);
}

void write_content_init(int num) {
    printf("<script>\
            document.all('res_tr_content').innerHTML += \"\
                <td id='m%d'></td>\";\
            </script>", num);

    fflush(stdout);
}

/* replace the replacements substring of orig with a given string */
char *str_replace(char *orig, char *rep, char *with)
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)) != NULL; ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

/* html wrapper */
char *wrap_html(char *source)
{
    char *result;
    result = source;
    result = str_replace(result, "&", "&amp;");
    result = str_replace(result, "\"", "&quot;");
    result = str_replace(result, "<", "&lt;");
    result = str_replace(result, ">", "&gt;");
    result = str_replace(result, "\r\n", "\n");
    result = str_replace(result, "\n", "<br>");

    return result;
}
