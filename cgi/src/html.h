#include <stdlib.h>
#include "dbg.h"
#include "request.h"

void html_init();
void html_end();
void serve_request_at(int num, char id);
void write_head_at(int num, char *content);
void write_content_at(int num, char id, char *content, int bold);
void write_content_init(int num, char id);
char *str_replace(char *orig, char *rep, char *with);
char* wrap_html(char *source);
