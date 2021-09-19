
#include "util.h"
#include <time.h>

int
main()
{
	srandom(time(NULL));

	printf( "Hola, soy PID %d: \n", getpid() );

	int fds1 [2];
	_pipe( fds1 );
	printf( "  - primer pipe me devuelve: [%d, %d] \n", fds1[0], fds1[1] );

	int fds2 [2];
	_pipe( fds2 );
	printf( "  - segundo pipe me devuelve: [%d, %d] \n", fds2[0], fds2[1] );

	printf("\n");

	int readFd;
	int writeFd;
	int msg = 0;

	int i = _fork();

	if (i == 0) {
		/// CHILD ------------------------- CHILD
		readFd = selectReadFd( fds1 );
		writeFd = selectWriteFd( fds2 );
		
		_read( readFd, &msg, sizeof(msg) );
		
		printf( "Donde fork me devuelve %d \n", i );
		printf( "  - getpid me devuelve: %d \n", getpid() );
		printf( "  - getppid me devuelve: %d \n", getppid() );
		printf( "  - recibo el valor %d a travez de fd=%d \n", msg, readFd );
		printf( "  - reenvio el valor en fd=%d \n", writeFd );
		printf("\n");
		
		_write( writeFd, &msg, sizeof(msg) );

		
	} else {
		/// PARENT ------------------------- PARENT
		readFd = selectReadFd( fds2 );
		writeFd = selectWriteFd( fds1 );

		msg = random();

		printf( "Donde fork me devuelve %d \n", i );
		printf( "  - getpid me devuelve: %d \n", getpid() );
		printf( "  - getppid me devuelve: %d \n", getppid() );
		printf( "  - random me devuelve: %d \n", msg );
		printf( "  - envio el valor %d a travez de fd=%d \n", msg, writeFd );
		printf("\n");

		_write( writeFd, &msg, sizeof(msg) );
		

		_read( readFd, &msg, sizeof(msg) );

		printf( "Hola, de nuevo PID %d \n", getpid() );
		printf( "  - recibí valor %d vía fd=%d \n", msg, readFd );

	}

	close( writeFd );
	close( readFd );

	_exit(0);
}
