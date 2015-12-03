#ifndef __REQUEST_H
#define __REQUEST_H

typedef struct request {
    char query_string[1000];
    ssize_t content_length;
    char request_method[100];
    char script_name[1000];
    char remote_host[1000];
    char remote_addr[1000];
    // auth_type, remote_user, remote_ident
} Request;

#endif
