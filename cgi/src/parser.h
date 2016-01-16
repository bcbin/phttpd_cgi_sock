#ifndef __PARSE_H
#define __PARSE_H

#include <stdlib.h>
#include "dbg.h"
#include "parser.h"
#include "request.h"

int parse_query_string(char *qs);
int parse_split_query(char *query, Request **request);
int parse_params();

#endif
