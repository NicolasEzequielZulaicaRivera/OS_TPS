#ifndef HISTORY_H
#define HISTORY_H

#include "defs.h"

void init_history();

int history_cmd(int n);

char * hist_get_previous ();
char * hist_get_next ();
char * hist_get (int n);

void hist_push (char *str);

#endif  // HISTORY_H