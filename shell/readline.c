#include "defs.h"
#include "readline.h"
#include "history.h"

static char buffer[BUFLEN];

// Handle character that start w/ escape code
// Might change buffer ( and thus i )
void handleEsc(int *i);

// reads a line from the standard input
// and prints the prompt
char *
read_line(const char *promt)
{
	int i = 0, c = 0;

#ifndef SHELL_NO_INTERACTIVE
	fprintf(stdout, "%s %s %s\n", COLOR_RED, promt, COLOR_RESET);
	fprintf(stdout, "%s", "$ ");
	fflush(stdout);
#endif

	memset(buffer, 0, BUFLEN);

	while (1)
    {
      read (STDIN_FILENO, &c, 1);

		switch (c)
		{
		case EOF: // END OF FILE
		case 4: // EOF | CTRL+D
			return NULL;
		case 27: // ESC
			handleEsc(&i);
			break;
		case 127: // BACKSPACE
			buffer[--i] = 0;
			putchar_debug('\b'); putchar_debug(' '); putchar_debug('\b');
			break;
		case END_LINE:
			buffer[i] = END_STRING;
			putchar_debug(END_LINE);
			hist_push(buffer);
			return buffer;
		default:
			buffer[i++] = c;
			putchar_debug(c);
			break;
		}
#ifndef SHELL_NO_INTERACTIVE
		fflush(stdout);
#endif
    }
	return NULL;
}

void handleEsc(int *i){
	int c = 0;
	read (STDIN_FILENO, &c, 1);
	switch (c)
	{
	case '[': // ARROWS
		read (STDIN_FILENO, &c, 1);
		switch (c)
		{
		case 'A': // UP-ARROW
			strcpy(buffer, hist_get_previous() );
			break;
		case 'B': // DOWN-ARROW
			strcpy(buffer, hist_get_next() );
			break;
		default : return;
		}
		printf("\33[2K\r$ %s", buffer);
		*i = strlen(buffer);
	break;
	}

}
