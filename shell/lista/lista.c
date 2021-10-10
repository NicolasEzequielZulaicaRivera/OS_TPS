#include "lista.h"
#include <stdlib.h>

const int EXITO =  0;
const int FALLO = -1;


lista_t* lista_crear(){

  lista_t* lista = malloc( sizeof( lista_t ) );
  if( !lista ) return NULL;

  lista->cantidad = 0;
  lista->nodo_inicio = NULL;
  lista->nodo_fin = NULL;

  return lista;
}

/*
 * Inserta un elemento al inicio de la lista.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int lista_insertar_inicio(lista_t* lista, void* elemento){
  if( !lista ) return FALLO;

  nodo_t* nodo = malloc( sizeof(nodo_t) );
  if( !nodo ) return FALLO;

  nodo->elemento = elemento;
  nodo->siguiente = NULL;

  if( !lista->nodo_fin )
    lista->nodo_fin = nodo;

  if( lista->nodo_inicio )
    nodo->siguiente = lista->nodo_inicio;

  lista->nodo_inicio = nodo;

  lista->cantidad ++;

  return EXITO;
}

/*
 * Devuelve el nodo en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
nodo_t* nodo_en_posicion( lista_t* lista, size_t posicion ){
  if(!lista) return NULL;
  if( posicion >= lista->cantidad ) return NULL;

  nodo_t* nodo = lista->nodo_inicio;
  for( size_t i=0 ; i < posicion ; i++ ){
    if(!nodo) return NULL;
    nodo = nodo->siguiente;
  }

  return nodo;
}

int lista_insertar(lista_t* lista, void* elemento){

  if( !lista ) return FALLO;

  nodo_t* nodo = malloc( sizeof(nodo_t) );
  if( !nodo ) return FALLO;

  nodo->elemento = elemento;
  nodo->siguiente = NULL;

  if( !lista->nodo_inicio )
    lista->nodo_inicio = nodo;

  if( lista->nodo_fin )
    lista->nodo_fin->siguiente = nodo;

  lista->nodo_fin = nodo;

  lista->cantidad ++;

  return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){

  if(!lista) return FALLO;
  if( posicion > lista->cantidad ) posicion = lista->cantidad;

  if( posicion == 0 ) return lista_insertar_inicio( lista, elemento );
  if( posicion == lista->cantidad ) return lista_insertar( lista, elemento );

  nodo_t* nodo_anterior = nodo_en_posicion(lista, posicion-1);

  nodo_t* nuevo_nodo = malloc( sizeof(nodo_t) );
  if( !nuevo_nodo ) return FALLO;
  nuevo_nodo->elemento = elemento;
  nuevo_nodo->siguiente = nodo_anterior->siguiente;
  nodo_anterior->siguiente = nuevo_nodo;

  lista->cantidad++;

  return EXITO;
}

int lista_borrar(lista_t* lista){
  if(!lista) return FALLO;
  if( lista->cantidad == 0 ) return FALLO;

  if( lista->cantidad == 1 ){
      free( lista->nodo_inicio );
      lista->nodo_inicio = lista->nodo_fin = NULL;
      lista->cantidad--;
      return EXITO;
  }

  nodo_t* anteultimo = nodo_en_posicion( lista, lista->cantidad-2 );
  if( anteultimo->siguiente != lista->nodo_fin ) return FALLO;

  free( anteultimo->siguiente );
  lista->nodo_fin = anteultimo;
  anteultimo->siguiente = NULL;
  lista->cantidad--;

  return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
  if(!lista) return FALLO;
  if( lista->cantidad == 0 ) return FALLO;
  if( posicion >= lista->cantidad ) posicion = lista->cantidad-1;

  if( posicion == lista->cantidad-1 ) return lista_borrar( lista );
  if( posicion == 0 ){
    nodo_t* aux = lista->nodo_inicio;
    lista->nodo_inicio = lista->nodo_inicio->siguiente;
    free( aux );
    lista->cantidad--;
    return EXITO;
  }

  nodo_t* nodo_anterior = nodo_en_posicion( lista , posicion-1 );
  nodo_t* nodo = nodo_anterior->siguiente;
  nodo_anterior->siguiente = nodo->siguiente;
  free( nodo );
  lista->cantidad--;

  return EXITO;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
  if( !lista ) return NULL;
  if( posicion >= lista->cantidad ) return NULL;
  if( posicion == lista->cantidad-1 ) return lista_ultimo(lista);

  nodo_t* nodo = nodo_en_posicion( lista, posicion );
  if( !nodo ) return NULL;

  return nodo->elemento;
}

void* lista_ultimo(lista_t* lista){
  if( !lista ) return NULL;
  if( !lista->nodo_fin ) return NULL;
  return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
  if( !lista ) return true;

  if( !lista->cantidad ) return true;
  return false;
}

size_t lista_elementos(lista_t* lista){
  if(!lista) return (size_t)EXITO;
  return lista->cantidad;
}

int lista_apilar(lista_t* lista, void* elemento){
  return lista_insertar_inicio( lista, elemento );
}

int lista_desapilar(lista_t* lista){
  return lista_borrar_de_posicion( lista, 0 );
}

void* lista_tope(lista_t* lista){
  return lista_elemento_en_posicion(lista,0);
}

int lista_encolar(lista_t* lista, void* elemento){
  return lista_insertar( lista, elemento );
}

int lista_desencolar(lista_t* lista){
  return lista_borrar_de_posicion(lista,0);
}

void* lista_primero(lista_t* lista){
  return lista_elemento_en_posicion(lista,0);
}

void lista_destruir(lista_t* lista){

  if( !lista ) return;

  nodo_t* nodo_i = lista->nodo_inicio;
  nodo_t* nodo_aux = NULL;

  while ( nodo_i ) {
    nodo_aux = nodo_i;
    nodo_i = nodo_i->siguiente;
    free( nodo_aux );
  }

  free( lista );

  return;
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
  if(!lista) return NULL;

  lista_iterador_t* iterador = malloc( sizeof(lista_iterador_t) );

  if( !iterador ) return NULL;

  iterador->lista = lista;
  iterador->corriente = lista->nodo_inicio;

  return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
  if( !iterador ) return false;
  return iterador->corriente;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
  if( !iterador ) return false;
  if( !iterador->corriente ) return false;

  iterador->corriente = iterador->corriente->siguiente;

  return iterador->corriente;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
  if( !iterador ) return false;
  if( !iterador->corriente ) return false;
  return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
  free( iterador );
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){

  if(!lista) return 0;
  if(!lista->nodo_inicio) return 0;
  if(!funcion) return 0;

  size_t elementos = 0;
  nodo_t* nodo = lista->nodo_inicio;

  while( nodo && funcion( nodo->elemento, contexto ) ){

    nodo = nodo->siguiente;
    elementos++;
  }

  return elementos;
}
