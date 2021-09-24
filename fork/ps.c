#define _GNU_SOURCE
#include "util.h"
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

typedef char string[64];

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

            // Mostramos la linea, que contendra:
            // PID nombre estado PPID ...etc
            // el significado del resto de los datos se puede encontrar en:
            // man 5 proc , /proc/[pid]/stat
            printf( "%s\n", line );

            close(fd);
        }
    }

    closedir( procs );
    _exit(0);
}