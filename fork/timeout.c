#include "util.h"
#include <signal.h>
#include <time.h>

int
timeout(int argc, char *argv[])
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

int
main()
{
   // PROBANDO TIMER
   struct sigevent event;
   event.sigev_notify = SIGEV_SIGNAL;
   event.sigev_signo = SIGKILL;

   struct itimerspec itime;
   itime.it_value.tv_sec  = 5; 
   itime.it_value.tv_nsec  = 0; 
   itime.it_interval.tv_sec = 0;
   itime.it_interval.tv_nsec  = 0; 

   timer_t timerId;

   timer_create( CLOCK_REALTIME , &event, &timerId);

   timer_settime(timerId, 0, &itime, NULL);


   for( int i=0; i<20; i++ ){
       printf("%i ",i);
       fflush(stdout);
       sleep(1);
   }
    return 0;
}