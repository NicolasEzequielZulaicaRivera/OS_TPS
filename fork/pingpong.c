#include <stdio.h>
#include <unistd.h>
#include "util.h"


int
main()
{

	int fds1 [2];
	_pipe( fds1 );

	int fds2 [2];
	_pipe( fds2 );

	int readFd;
	int writeFd;

	int msg = 0;
	int re_msg = 0;

	int i = _fork();

	if (i == 0) {
		/// CHILD
		printf("Soy el proceso hijo y mi pid es: %d\n", getpid());
		readFd = selectReadFd( fds1 );
		writeFd = selectWriteFd( fds2 );
		
		msg = 101;

		printf("[Child] WRITE: %i\n", msg );
		_write( writeFd, &msg, sizeof(msg) );

		_read( readFd, &re_msg, sizeof(re_msg) );
		printf("[Child] READ: %i\n", re_msg );
		
	} else {
		/// PARENT
		printf("Soy el proceso padre y mi pid es: %d\n", getpid());
		readFd = selectReadFd( fds2 );
		writeFd = selectWriteFd( fds1 );

		msg = 123;

		_read( readFd, &re_msg, sizeof(re_msg) );
		printf("[Parent] READ: %i\n", re_msg );

		printf("[Parent] WRITE: %i\n", msg );
		_write( writeFd, &msg, sizeof(msg) );

	}

	close( writeFd );
	close( readFd );
	printf("Terminando\n");
	_exit(0);
}
