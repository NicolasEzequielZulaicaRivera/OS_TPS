#include "util.h"

int
main(int argc, char *argv[])
{
	if( argc != 2 ){
		errno = EINVAL;
		perror("Interfaz: primes <n> \n");
		_exit(-1);
	}

	const unsigned int num = atoi( argv[1] );
	const ssize_t s_uint = sizeof(unsigned int);

	int fds [2];
	_pipe( fds );

	int i = _fork();
	int j = 2;

	if (i != 0) {
		/// PARENT ------------------------- PARENT
		int writeFd = selectWriteFd( fds );

		for( unsigned int j=2; j<=num; j++ ){
			_write( writeFd, &j, s_uint );
		}

		close( writeFd );
	} else {
		/// CHILD ------------------------- CHILD
		int readFd = selectReadFd( fds );
		int writeFd = 0;

		int number = 2;
		bool firstRead = true;

		while( _read( readFd, &j, s_uint ) ){

			if( firstRead ){
				number = j;
				printf("primo %d \n", number);
				firstRead = false;
				continue;
			}

			if( j%number != 0 ){

				if( !writeFd ){

					_pipe( fds );

					if( _fork() ){ // PARENT
						writeFd = selectWriteFd( fds );
					} else { // CHILD
						firstRead=true;
						close(readFd);
						readFd = selectReadFd( fds );
						continue;
					}

				}
				
				_write( writeFd, &j, s_uint );

			}
		}
		if( writeFd ) close( writeFd );
		close( readFd );
	}

	wait(NULL);
	_exit(0);
}