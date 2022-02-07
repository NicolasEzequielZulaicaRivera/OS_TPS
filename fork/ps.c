#define _GNU_SOURCE
#include "util.h"
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

typedef char string[64];


void
print_process(string line);

char *
format_state(string state);

int
main()
{
    // El directorio /procs contiene todos los procesos
    DIR *procs = opendir("/proc");

	struct dirent* entry;
    while ( ( entry = readdir(procs) ) ) {
    
        // Los procesos seran accesibles en forma de directorio dentro de /procs
        // Los directoris de los procesos se llaman por su PID, 
        // entonces filtramos los directorios con nombre numerico
		if (entry->d_type == DT_DIR && atoi(entry->d_name) ) {
            string line;

            // El archivo stat dentro del directorio de cada proceso 
            // contiene informacion del proceso
            string stats = "/proc/";
            strcat(stats, entry->d_name);
            strcat(stats, "/stat");

            int fd = open( stats, DT_REG );

            _read( fd, &line, sizeof(line) );

            print_process(line);

            close(fd);
        }
    }
    printf( "\n" );
    closedir( procs );
    _exit(0);
}


void
print_process(string line)
{
    char *token;
    printf( "\e[4m                                                            \e[0m\n\n" );

    token = strtok(line, " ");
    printf( "PID:\t\e[1m%s\e[0m\n", token );
        
    token = strtok(NULL, " ");
    printf( "COMM:\t%s (filename of the executable)\n", token );

    token = strtok(NULL, " ");
    printf( "STATE:\t%s (%s)\n", token, format_state(token) );

    token = strtok(NULL, " ");
    printf( "PPID:\t%s (PID of the parent)\n", token );

    token = strtok(NULL, " ");
    printf( "PGRP:\t%s (group ID)\n", token );

    // Ignore the rest of the line
    // The value of further tokens is not important for the purpose of this program
    // > Can be seen on `man 5 proc` : `/proc/[pid]/stat`
}

char *
format_state(string state){
    if( !strcmp(state,"R") )
        return "Running";
    if( !strcmp(state,"S") )
        return "Sleeping in an interruptible wait";
    if( !strcmp(state,"D") )
        return "Waiting in uninterruptible disk sleep";
    if( !strcmp(state,"Z") )
        return "Zombie";
    if( !strcmp(state,"T") )
        return "Stopped (on a signal) or (before Linux 2.6.33) trace stopped";
    if( !strcmp(state,"t") )
        return "Tracing stop (Linux 2.6.33 onward)";
    if( !strcmp(state,"W") )
        return "Paging (only before Linux 2.6.0)";
    if( !strcmp(state,"X") )
        return "Dead (from Linux 2.6.0 onward)";
    if( !strcmp(state,"x") )
        return "Dead (Linux 2.6.33 to 3.13 only)";
    if( !strcmp(state,"K") )
        return "Wakekill (Linux 2.6.33 to 3.13 only)";
    if( !strcmp(state,"W") )
        return "Waking (Linux 2.6.33 to 3.13 only)";
    if( !strcmp(state,"P") )
        return "Parked (Linux 3.9 to 3.13 only)";
    return "UNKNOWN";
}