#include <stdio.h>
#include <unistd.h>
#include "util.h"

int
_fork(void){
    int res = fork();
    
    if( res<0 ){
        perror("Error en FORK");
        _exit(-1);
    }
    return res;
}

int
_pipe( int *fds  ){
    int res = pipe(fds);

    if( res<0 ){
        perror("Error en PIPE");
        _exit(-1);
    }
    return res;
}

int
selectReadFd( int *fds ){
    close( fds[1] );
    return fds[0];
}

int
selectWriteFd( int *fds ){
    close( fds[0] );
    return fds[1];
}

ssize_t
_read( int fd, void *buf, size_t count){
    ssize_t res = read(fd, buf, count);

    if( res<0 ){
        perror("Error en READ");
        _exit(-1);
    }
    return res;
}

ssize_t
_write( int fd, const void *buf, size_t count){
    ssize_t res = write(fd, buf, count);

    if( res<0 ){
        perror("Error en WRITE");
        _exit(-1);
    }
    return res;
}
