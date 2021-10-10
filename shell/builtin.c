#include "builtin.h"
#include "history.h"

// returns true if the 'exit' call
// should be performed
//
// (It must not be called from here)
int
exit_shell(char *cmd)
{
	// Your code here
	char * _cmd = strdup(cmd);
	split_line(_cmd,SPACE);
	if( !strcmp(_cmd,"exit") ){
		free(_cmd);
		status = 0;
		return 1;
	}
	free(_cmd);
	return 0;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to $HOME)
//  it has to be executed and then return true
//
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int
cd(char *cmd)
{
	// Your code here
	char * _cmd = strdup(cmd);
	split_line(_cmd,SPACE);
	char * _dir = _cmd + 3;

	if( !strcmp(_cmd,"cd") ){

		while( _dir[0] == SPACE ) _dir ++;

		if( strlen(_dir) == 0 ){
			chdir( getenv("HOME") );
			getcwd(promt, PRMTLEN);
		} else {
			if( _dir[ strlen(_dir) ] != '/' ) {
				_cmd = realloc( _cmd, sizeof(char) * ( strlen(cmd) + 2 ) );
				strcat(_dir,"/");
			}
			if( chdir(_dir) ) status = -1;
			else getcwd(promt, PRMTLEN);
		}

		free(_cmd);
		return 1;
	}
	free(_cmd);
	return 0;
}

// returns true if 'pwd' was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
pwd(char *cmd)
{
	// Your code here
	char * _cmd = strdup(cmd);
	split_line(_cmd,SPACE);
	if( !strcmp(_cmd,"pwd") ){
		if( getcwd(promt, PRMTLEN) == NULL){
			status = -1;
		} else {
			printf("%s",promt); // Not a debug_print
			status = 0;
		}
		return 1;
	}
	free(_cmd);
	return 0;
}

int
history(char *cmd)
{
	// Your code here
	char * _cmd = strdup(cmd);
	split_line(_cmd,SPACE);

	if( !strcmp(_cmd,"history") ){
		int n = atoi(_cmd+8);
		status = history_cmd(n);
		return 1;
	}
	free(_cmd);
	return 0;
}
