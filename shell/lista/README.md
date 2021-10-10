# [TDA: LISTA , PILA , COLA](https://github.com/NicolasEzequielZulaicaRivera/A2_TDA1)

## Funcionamiento

  Se crea la libreria **lista.h** con el objetivo de implementar los tipos de datos abstractos **Lista simplemente enlazada** , **Pila** y **Cola**
  
  Se define la estructura `lista_t`, capaz de funcionar como cualquiera de los tres tipos. Ademas se incluye la estructura `lista_iterador` como iterador externo y la funcion  `lista_con_cada_elemento` como iterador interno para recorrer la lista.
  
  ### Lista simplemente enlazada
  
  Este tipo de dato nos permite almacenar diferentes elementos de manera ordenada, acceder a ellos y eliminarlos de la estructura sin ninguna restriccion.
  
  ### Pila
  
  Este tipo de dato nos permite almacenar diferentes elementos de manera ordenada, acceder a ellos y eliminarlos de la estructura.
  Se caracteriza por su sistema LIFO (last in, first out), es decir, solo tendremos acceso al elemento insertado con menor antiguedad.
  
  ### Cola
  Este tipo de dato nos permite almacenar diferentes elementos de manera ordenada, acceder a ellos y eliminarlos de la estructura.
  Se caracteriza por su sistema FIFO (first in, first out), es decir, solo tendremos acceso al elemento insertado con mayor antiguedad.

  **Importante :** Las funciones implementadas reservan *memoria dinamica* para la utilizacion de las estructuras, por lo que es imperativo que esta se libere antes de finalizar el programa.
  Esto se realiza mediante el uso de las funciones `lista_destruir`. Ademas, cualquier memoria reservada por el usuario queda a su discrecion y responsabilidad liberar, ya que la estructura no se encargara de liberarla al borrar elementos.

  Se añade el archivo **minipruebas.c** como ejemplo de implementacion de la libreria tanto como para verificar su correcto funcionamiento.

## Compilacion y Ejecution

  Habiendo incluido la libreria en su programa o descargado el main.c proveido, puede compilarlo con la siguiente linea en la terminal :

  ` gcc *.c -Wall -Werror -Wconversion -std=c99 -o nombre_programa `
  > usaremos el compilador gcc nativo a cualquier distribucion linux para compilar todos los archivos c (\*.c)
    en el directorio, empleando diversos *flags* para prevenir errores,
    queda a discrecion del usuario elegir el nombre del programa

  Luego puede ejecutar el programa con la siguiente linea:

  `./nombre_programa`

  Cabe aclarar que la terminal debe encontrarse en el directorio donde se encuentran los archivos del programa para poder llevar su compilacion y ejecucion a cabo.

  Se recomienda  emplear un **makefile** para agilizar estas tareas.
  Se añade en los archivos un makefile que nos permitira usar la linea `make run` que compilara (de ser necesario) y ejecutara el programa.
  
---

### Funcionamiento particular de la implementación

En esta implementacion se opto por aprovechar la similitud entre los tres tipos de datos. Siendo la lista simplemente enlazada la esctructura con mas libertad de las tres, se tomo esta como base y se construyeron las funcionalidades de pila y cola sobre ellas.

Otro punto que cabe destacar es que esta implementacion no hace uso de rutinas recursivas a favor de no producir errores de desbordamiento de pila ( tengase en cuenta que el compilador optado no presenta optimizacion por recursion en la cola ).

---

## ¿Qué es lo que entendés por una lista? ¿Cuáles son las diferencias entre ser simple o doblemente enlazada?

Este tipo de dato nos permite almacenar diferentes elementos de manera ordenada, acceder a ellos y eliminarlos de la estructura sin ninguna restriccion, es decir, podemos insertar, visualizar o eliminar un elemento de cualquier posicion.

Teniendo en cuenta que una lista sera una cadena de nodos, donde un nodo es una estructura que contendra, por lo menos, el elemento o referencia al elemento que se quiere almacenar y una referencia al siguiente nodo.
La diferencia entre una lista simplemente enlazada y una lista doblemente enlazada, es que en la primera cada nodo solo apunta al siguiente y en la segunda tambien apuntan al anterior.
Esto hace que los tiempos de trabajo de una lista doblemente enlazada sean mejores cuando se quiere acceder a los nodos mas profundos( La complejidad de acceder a un elemento en cierta posicion en la primera es O(N) mientras que en la segunda O(N/2) ). Por otro lado la implementacion de una lista doblemente enlazada es mas comleja.
Cabe aclarar que, segun la implementacion, los funcionamientos de pila y cola no se ven afectados por la inhabilidad de recorrer la lista en ambos sentidos.

## ¿Cuáles son las características fundamentales de las Pilas? ¿Y de las Colas?

**Pilas:** El primer elemento en entrar es el ultimo en salir. (LIFO)

**Colas:** El primer elemento en entrar es el primero en salir. (FIFO) 

## ¿Qué es un iterador? ¿Cuál es su función?

Un iterador es una estructura que nos permite recorrer de manera secue un contenedor ( estructura que almacena datos ), de modo que podamos acceder a sus elementos. 
Para el caso de una lista, este recorrido sera secuencial ( de inicio a fin ). 

## ¿En qué se diferencia un iterador interno de uno externo?

- Iterador Intero: El flujo de iteracion sera interno a la estructura, por lo que el usuario tiene un control limitado del proceso pero no necesita controlar el ciclo en el cual se recorre el mismo. Para ello normalmente un iterador interno en una lista es una funcion que recibira: la lista a iterar y una funcion que ejecutar para cada elemento que indicara si continuar la iteracion

- Iterador Externo: es un tda que nos permitira controlar la iteracion mediante sus metodos. 
  Un ejemplo de implementacion de iterador externo:
  ```C
    lista_iterador_t* it = NULL;

    for(it = lista_iterador_crear(lista);
        lista_iterador_tiene_siguiente(it);
        lista_iterador_avanzar(it))
        printf("%c ", *(char*)lista_iterador_elemento_actual(it));
    printf("\n\n");

    lista_iterador_destruir(it);
  ```

