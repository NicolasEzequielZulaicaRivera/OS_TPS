#ifndef TP_FORK_UTIL_H
#define TP_FORK_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>

int
_fork(void);

int
_pipe( int *fds  );

int
selectReadFd( int *fds );

int
selectWriteFd( int *fds );

ssize_t
_read( int fd, void *buf, size_t count);

ssize_t
_write( int fd, const void *buf, size_t count);

#endif