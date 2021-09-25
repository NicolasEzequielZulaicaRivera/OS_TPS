#include "util.h"
#include <signal.h>
#include <time.h>

// --------------- TIMEOUT CON SLEEP ---------------
// Utiliza fork, sleep y kill
/*
int
main (int argc, char *argv[])
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
*/

// --------------- TIMEOUT CON TIMER ---------------
// Utiliza timer_create y timer_settime
// Ademas de fork y kill

// PID HIJO
// Es global para poder pasarselo al handler
int child_pid = 0; 

// HANDLER PARA TERMINAR PROCESO HIJO
void handle_killChild( int sig ){
    kill(child_pid,SIGKILL);
}

int
main(int argc, char *argv[])
{
    // COMPROBAR PARAMETROS
    if( argc < 3 ){
        errno = EINVAL;
		perror("Interfaz: timeout <n> <comando> \n");
		_exit(-1);
    }

    // DEFINIR ACCION PARA TERMINAR PROCESO HIJO
    // Ligamos el handler que termina al proceso hijo a la senal SIGUSR1
    // No hay problema en usar esta senal, esta para ser usada por los usuarios.
    struct sigaction killChild;
    killChild.sa_handler = &handle_killChild;
    sigaction(SIGUSR1, &killChild, NULL);

    // CREAR EVENTO
    // El evento manda una senal SIGUSR1
    // Esta senal esta definida para matar al proceso hijo
    struct sigevent event;
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGUSR1;

    // CREAR OBJETO DE TIEMPO
    // Setear duracion
    struct itimerspec itime;
    itime.it_value.tv_sec  = atoi( argv[1] ); 
    itime.it_value.tv_nsec  = 0;
    // Setear repeticion - 0 Indica que no se repite
    itime.it_interval.tv_sec = 0;
    itime.it_interval.tv_nsec  = 0; 

    // CREAR TIMER
    timer_t timerId;
    timer_create( CLOCK_REALTIME , &event, &timerId);

    // INICIAR TIMER
    timer_settime(timerId, 0, &itime, NULL);

    // FORK
    child_pid = _fork();
    // HIJO : Ejecuta el comando
    if( !child_pid ) execvp( argv[2], argv+2 );
    // PADRE : Espera a que el hijo termine ( incluido haberlo terminado )
    wait(NULL);

    return 0;
}