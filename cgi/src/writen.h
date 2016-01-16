#ifndef __WRITEN_H
#define __WRITEN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>

ssize_t writen(int fd, const void *vptr, size_t n);
void Writen(int fd, void *ptr, size_t nbytes);

#endif
