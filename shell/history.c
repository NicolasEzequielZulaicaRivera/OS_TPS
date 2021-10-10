#include "defs.h"
#include <stdio.h>

char hist_promt[PRMTLEN] = { 0 };
static char hist_f_dir[PRMTLEN] = { 0 };
static FILE * hist_file = NULL;

int history_cmd(int n){
    if(!hist_file) return -1;
    fclose(hist_file);

    hist_file = fopen( hist_f_dir, "r" );
    if(!hist_file){ 
        hist_file = fopen( hist_f_dir, "a" );
        return -1;
    }

    char *line;
    size_t len = 0;

    if( n != 0 ){
        int lines_count = 0;
        while ( getline(&line, &len, hist_file) != -1 ) 
            lines_count++;

        fseek(hist_file, 0, SEEK_SET);

        if( lines_count > n )
            for( int i=0; i<n; i++ ) 
                getline(&line, &len, hist_file);
        
    }

    while ( getline(&line, &len, hist_file) != -1 )
        printf("%s", line);

    fclose(hist_file);
    hist_file = fopen( hist_f_dir, "a" );
    return 0;
}

void cleanup_history(){
    if(!hist_file) return;
    fclose(hist_file);
}

void init_history() {

    if( getenv("HISTFILE") )  strcpy( hist_f_dir, getenv("HISTFILE") );
    else {
        strcpy( hist_f_dir, getenv("HOME") );
        strcat( hist_f_dir, "/.fisop_history" );
    }

    hist_file = fopen( hist_f_dir , "a" );

    atexit(cleanup_history);
    return;
}

char * hist_get_previous () {
    if(!hist_file) return hist_promt;

    return hist_promt;
}
char * hist_get_next (){
    if(!hist_file) return hist_promt;

    return hist_promt;
}
char * hist_get (int n){
    return hist_promt;
}

void hist_push (char *str){
    if(!hist_file || !str) return;
    fprintf(hist_file,"%s\n", str);
    return;
}