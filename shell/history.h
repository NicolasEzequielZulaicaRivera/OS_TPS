#ifndef HISTORY_H
#define HISTORY_H

#include "defs.h"

// Initialize module
void init_history();

// Get prev/next command
char * hist_get_previous ();
char * hist_get_next ();

// Get command on history at pos n ( 1 indexed )
char * hist_get (int n);

// Add command to history
void hist_push (char *str);

// CMD Command `history`
// shows command history (from HISTFILE)
// n : number of entries
// > n=0 : all entries
int history_cmd(int n);

#endif  // HISTORY_H