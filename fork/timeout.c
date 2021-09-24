#include "util.h"

int
main(int argc, char *argv[])
{
    // Crear un timer

    // Fork
    // El hijo ejecuta el segundo proceso
    int cpid = _fork();

    if( !cpid ){
        execvp( argv[2], argv+2 );
    }

    // Esperar timer
    // si el hijo no termino lo maa
    _exit(0);
}