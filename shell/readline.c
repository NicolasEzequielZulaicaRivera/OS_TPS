#include "defs.h"
#include "readline.h"
#include "history.h"
#include "utils.h"
#include <sys/ioctl.h>

static char buffer[BUFLEN];

// Handle character that start w/ escape code
// Might change buffer ( and thus i )
void handleEsc( int *i, size_t *row, size_t *col, int MAX_COL );

// Handle backspace (delete char)
void handleBackspace( int *i, size_t *row, size_t *col, int MAX_COL );

// Writes character/word to terminal
// by controling how characters are written,
// problems when exceding width can be controled
void writeChar( char c, size_t *row, size_t *col, int MAX_COL );
void writeWord( char *str, size_t *row, size_t *col, int MAX_COL );

void clearLine( size_t *row, size_t *col );

// reads a line from the standard input
// and prints the prompt
char *
read_line(const char *promt)
{
	int i = 0, c = 0;

	// Track last char col & row 
	// Thus control placement on char input
	size_t col = 0, row = 0;

	// Set terminal max columns
	struct winsize w; ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Load winsize obj
	int MAX_COL = w.ws_col-1; // Get width
	if( !MAX_COL || MAX_COL<1 ) MAX_COL = 50; // Fallback

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
			handleEsc(&i,&row,&col,MAX_COL);
			break;
		case 127: // BACKSPACE
			handleBackspace(&i,&row,&col,MAX_COL);
			break;
		case END_LINE: // END LINE
			buffer[i] = END_STRING;
			putchar_debug(END_LINE);
			hist_push(buffer);
			return buffer;
		default: // NORMAL CHAR
			buffer[i++] = c;
			writeChar(c, &row, &col, MAX_COL);
			break;
		}
#ifndef SHELL_NO_INTERACTIVE
		fflush(stdout);
#endif
    }
	return NULL;
}

void handleEsc(int *i, size_t *row, size_t *col, int MAX_COL){
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
		clearLine(row,col);
		writeWord(buffer,row,col,MAX_COL);
		*i = strlen(buffer);
	break;
	}

}

void handleBackspace(int *i, size_t *row, size_t *col, int MAX_COL){
	if ( *i <= 0 ) return;
	
	buffer[--(*i)] = 0;

	if( (*col)-- == 0 ){	// DELETING INTO PREVIOUS LINE
		(*row)--;					// adjust row/col tracking
		(*col)=MAX_COL-1;
		printf_debug("\b \b\b \b"); //delete line & jump to prevline
		printf_debug("\b \b\b \b\033[A\033[%iC",MAX_COL+2);
	}
	printf_debug("\b \b");
}

void writeChar( char c, size_t *row, size_t *col, int MAX_COL ){
	putchar_debug(c);
	if( ++(*col) >= (size_t)MAX_COL ){
		printf_debug("\n> "); 	// artificial breakline in terminal 
		*col = 0; 				// to keep strict track of char placement
		(*row) ++;
	}
}
void writeWord( char *str, size_t *row, size_t *col, int MAX_COL ){
	for( size_t i = 0; i < strlen(str); i++ ) 	// words are written char by char
		writeChar( str[i] , row, col, MAX_COL );// to control placement
}
void clearLine( size_t *row, size_t *col ){
	*col = 0;
	while( *row > 0 ){	// deleting all rows of multiline command
		(*row) --;		// is easy as rows are tracked
		printf_debug("\33[2K\r\033[A");
	};
	printf_debug("\33[2K\r$ ");
}
