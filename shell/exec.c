#include "exec.h"

// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	// Your code here
	for( int i=0; i<eargc; i++ ){
		char *name = strdup( eargv[i] ) ;
		char *value = split_line(name, '=');
		setenv(name,value,0);
		free(name);
	}
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int
open_redir_fd(char *file, int flags)
{
	// Your code here
	int fd = open(file,flags,S_IWUSR | S_IRUSR);

	if( fd < 0 ){
		printf_debug("Error creating fd");
		_exit(-1);
	}

	return fd;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	switch (cmd->type) {
	case EXEC:
		// spawns a command
		//
		// Your code here
		e = (struct execcmd *)cmd;
		set_environ_vars( e->eargv, e->eargc );
		execvp( e->argv[0], e->argv);
		printf_debug("Command not found\n");
		fflush(stdout);
		_exit(-1);

	case BACK: {
		// runs a command in background
		//
		// Your code here
		b = (struct backcmd *)cmd;
		e = (struct execcmd *)b->c;
		exec_cmd(e);
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		//
		// Your code here
		r = (struct execcmd *)cmd;
		set_environ_vars( r->eargv, r->eargc );

		// replace stdin
		fflush(stdout);
		if( strlen(r->in_file) > 0 ){
			int fd = open_redir_fd( r->in_file, O_RDONLY );
			dup2(fd,0);
			close(fd);
		}
		fflush(stdout);

		// replace stdout
		if( strlen(r->out_file) > 0 ){
			fflush(stdout);
			int fd = open_redir_fd( r->out_file, O_WRONLY | O_CREAT );
			dup2(fd,1);
			fflush(stdout);
			close(fd);
		}
		// replace stderr
		if( strlen(r->err_file) > 0 ){
			if( (r->err_file)[0] == '&' ){
				dup2(1,2);
			}else{
				int fd = open_redir_fd( r->err_file, O_WRONLY | O_CREAT );
				dup2(fd,2);
				close(fd);
			}	
		}

		execvp( r->argv[0], r->argv);
		printf_debug("Command not found\n");
		_exit(-1);
		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		p = (struct pipecmd *)cmd;
		int fds [2];
		if( pipe(fds)<0 ) _exit(-1);

		if( !_fork() ){ // LEFT CHILD
			dup2(fds[1],1);
			close( fds[0] );close( fds[1] );
			e = (struct execcmd *)p->leftcmd;
			exec_cmd(e);
			_exit(-1);
		}
		
		if( !_fork() ){ // RIGHT CHILD
			dup2(fds[0],0);
			close( fds[0] );close( fds[1] );
			e = (struct execcmd *)p->rightcmd;
			exec_cmd(e);
			_exit(-1);
		}

		close( fds[0] );close( fds[1] );

		wait(NULL);
		wait(NULL);

		// free the memory allocated
		// for the pipe tree structure
		free_command(parsed_pipe);
		_exit(0);
		break;
	}
	}
}
