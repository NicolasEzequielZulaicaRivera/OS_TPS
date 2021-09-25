#include "util.h"
#include <signal.h>
#include <time.h>

int
main(int argc, char *argv[])
{

    if( argc < 3 ){
        errno = EINVAL;
		perror("Interfaz: timeout <n> <comando> \n");
		_exit(-1);
    }

    // Hacer Fork
    // y guardar el pid hijo
    int cpid = _fork();

    // El proceso hijo ejecutara el comando
    if( !cpid ) execvp( argv[2], argv+2 );

    // Esperar timer
    sleep( atoi( argv[1] ) );

    // si el hijo no termino lo termina
    // mandandole una senal SIGKILL
    kill(cpid,SIGKILL);
    wait(NULL);

    _exit(0);
}