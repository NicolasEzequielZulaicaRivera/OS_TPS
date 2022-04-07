#include "util.h"
#include <string.h>

#ifndef NARGS
#define NARGS 4
#endif

const int STRING_SIZE = 60;

int
main(int argc, char *argv[])
{
	if( argc != 2 ){
		errno = EINVAL;
		perror("Interfaz: xargs <comando> \n");
		_exit(-1);
	}

	const int maxnargs = NARGS+2;

	char **args= malloc(sizeof(char*) * (maxnargs));
	for( int i=0; i<maxnargs-1; i++ ) args[i]= malloc(sizeof(char)*STRING_SIZE);
	strcpy(args[0],argv[1]);
	args[maxnargs]=NULL;

	int nargs = 1;

	char* line = NULL;
	size_t size;


	while( getline(&line, &size, stdin ) != -1 ){
		
		if(line[strlen(line)-1]=='\n')line[strlen(line)-1] = '\0';
		strcpy(args[nargs++],line);

		if(nargs > NARGS){
			args[nargs]=NULL;

			if( !_fork() ){ // CHILD
				execvp(argv[1],args);
			}
			wait(NULL);
			nargs=1;
		}
	};
	if( nargs > 1 ){
		args[nargs]=NULL;

		if( !_fork() ){ // CHILD
			execvp(argv[1],args);
		}
		wait(NULL);
	}

	free(line);
	for( int i=0; i<NARGS; i++ ) free(args[i]);
	free(args);

	_exit(0);
}