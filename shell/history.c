#include "defs.h"
#include "lista/lista.h"

char EMPTY_STR[1] = { 0 };
static char hist_f_dir[PRMTLEN] = { 0 };
static FILE * hist_file = NULL;

static lista_t *history = NULL;
static ssize_t current = 0;

// Libera la memoria reservada por el modulo
void cleanup_history(){
    // Liberar archivo
    if(hist_file) fclose(hist_file);
    // Liberar lista
    if(history){
        while ( !lista_vacia(history) )
        {
            free( lista_primero(history) );
            lista_desapilar(history);
        }
        lista_destruir(history);
    }
}

// Funcion para inicializar el modulo
void init_history() {

    // Se guardan todos los comandos a travez de diferentes sesiones en el archivo HISTFILE
    // Este luego puede ser accedido mediante el comando history
    if( getenv("HISTFILE") )  strcpy( hist_f_dir, getenv("HISTFILE") );
    else {
        strcpy( hist_f_dir, getenv("HOME") );
        strcat( hist_f_dir, "/.fisop_history" );
    }

    // Se utiliza una lista para guardar los comandos de la sesion actual
    // Esta se utilizara para navegar con las flechas
    hist_file = fopen( hist_f_dir , "a" );

    history = lista_crear();

    // Aqui se podria popular la lista con los elementos del HISTFILE
    // zsh pareciera no hacerlo, asi que tampoco lo voy a implementar

    atexit(cleanup_history);
    return;
}

char * hist_get_previous () {
    if(!hist_file) return EMPTY_STR;

    current ++;
    if( current > history->cantidad )
        current = history->cantidad;

    return lista_elemento_en_posicion(history, current-1);
}
char * hist_get_next (){
    if(!hist_file) return EMPTY_STR;

    if( current <= 1 ){
        current = 0;
        return EMPTY_STR;
    }

    return lista_elemento_en_posicion(history, --current -1 );
}
char * hist_get (int n){
    if(!hist_file) return EMPTY_STR;

    if( n<1 ) n=1;
    if( current > history->cantidad ) current = history->cantidad;
    return lista_elemento_en_posicion(history, n-1 );
}

void hist_push (char *str){
    if(!hist_file || !str) return;
    fprintf(hist_file,"%s\n", str);
    lista_apilar( history, strdup(str) );
    return;
}

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
