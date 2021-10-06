# Lab: shell

### Búsqueda en $PATH

#### ¿cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?

Los wrappers de la funcion execve proporcionan las siguientes funcionalidades:

- l : se pasan los argumentos en los parametros de la funcion ( argumento variatico )
- v : se pasan los argumentos en un arreglo terminado con NULL
- e : se pasan los argumentos de ambiente en un arreglo terminado en NULL
- p : encuentra el archivo a ejectuar si este se encuentra en algun directorio del PATH

#### ¿Puede la llamada a exec(3) fallar? ¿Cómo se comporta la implementación de la shell en ese caso?

Si. La implementacion llama a `_exit`, finalizando el proceso hijo si falla en ejecutar el comando.

---

### Comandos built-in

---

### Variables de entorno temporarias

#### ¿Por qué es necesario hacerlo luego de la llamada a fork(2)?

Esto es necesario para no cambiar el ambiente del proceso principal.

Si lo hicieramos antes no serian variables temporarias, ya que el porceso principal guardaria la variable y esta se clonaria a cada proceso hijo al hacer fork.

#### En algunos de los wrappers de la familia de funciones de exec(3) (las que finalizan con la letra e), se les puede pasar un tercer argumento (o una lista de argumentos dependiendo del caso), con nuevas variables de entorno para la ejecución de ese proceso. Supongamos, entonces, que en vez de utilizar setenv(3) por cada una de las variables, se guardan en un array y se lo coloca en el tercer argumento de una de las funciones de exec(3). 

##### ¿El comportamiento resultante es el mismo que en el primer caso? Explicar qué sucede y por qué. 

El comportamiento no es el mismo. 
setenv agrega variables al ambiente (con la opcion de sobreescribir las que ya existan).
En cambio, al pasar las variables de entorno por parametro se pierden las que no esten en el parametro. 

##### Describir brevemente (sin implementar) una posible implementación para que el comportamiento sea el mismo.

Podriamos precargar eargv (y eargc) con el entorno actual cada vez que se construye un cmd.
Luego, en vez de llamar a `set_environ_vars` podriamos utilizar `execvpe`.


---

### Procesos en segundo plano

#### Detallar cuál es el mecanismo utilizado para implementar procesos en segundo plano.

El proceso principal espera al proceso que ejecuta el comando con el flag `WNOHANG`, de modo que no interrumpe a la shell.

---

### Flujo estándar

#### Investigar el significado de 2>&1, explicar cómo funciona su forma general y mostrar qué sucede con la salida de cat out.txt en el ejemplo. Luego repetirlo invertiendo el orden de las redirecciones. ¿Cambió algo?

n>&m significa que redirija el fd n al fd m. Esto se puede conseguir haciendo `dup2(m,n)`

```
➜  ~ ls -c noExiste >f 2>&1
➜  ~ cat f
ls: cannot access 'noExiste': No such file or directory 
➜  ~ ls -c noExiste 2>&1 >f
ls: cannot access 'noExiste': No such file or directory
➜  ~ cat f
➜  ~    
``` 

Si cambiamos el orden redirije el stderr al stdout original ( antes de cambiarlo por un archivo ) y por eso podemos ver el error por consola.

---

### Tuberías múltiples

#### Investigar qué ocurre con el exit code reportado por la shell si se ejecuta un pipe ¿Cambia en algo? ¿Qué ocurre si, en un pipe, alguno de los comandos falla? Mostrar evidencia (e.g. salidas de terminal) de este comportamiento usando bash. Comparar con la implementación del este lab.

**zsh:**

La shell reporta el exit code del ultimo comando en un pipeline, se puede acceder a los exit codes de los procesos en el ultimo pipeline mediante la variable de entorno `$pipestatus`

```
➜  ~ cat noexiste ; echo $?
cat: noexiste: No such file or directory
1
➜  ~ cat noexiste | echo hello ; echo $?
hello
cat: noexiste: No such file or directory
0
➜  ~ cat noexiste | echo hello ; echo $pipestatus
hello
cat: noexiste: No such file or directory
1 0 
```

**fiuba shell:**

La shell implementada no guarda los exit codes de los procesos hijos y siempre hace `_exit(0)`.
Por lo que el exit code sera siempre 0 (excepto que falle in fork).

```
 (/home/nicozu) 
$ cat noexiste
cat: noexiste: No such file or directory
       Program: [cat noexiste] exited, status: 1 

 (/home/nicozu) 
$ cat noexiste | echo hello
hello
cat: noexiste: No such file or directory
       Program: [PIPE] exited, status: 0 

 (/home/nicozu) 
$ cat noexiste | cat noexiste                    
cat: cat: noexistenoexiste: No such file or directory
: No such file or directory
       Program: [PIPE] exited, status: 0 

 (/home/nicozu) 
$ echo $? - $pipestatus
0 - 
       Program: [echo $? - $pipestatus] exited, status: 0 
```

---

### Pseudo-variables

#### Investigar al menos otras tres variables mágicas estándar, y describir su propósito. Incluir un ejemplo de su uso en bash (u otra terminal similar).

** [Variables especiales de shell](https://tldp.org/LDP/abs/html/refcards.html) **
- `$$` : PID del proceso (shell)
```
➜ echo $$
1012
```
- `$0` : nombre del programa (shell)
```
➜ echo $0
1012
```
- `$!` : PID del ultimo proceso corrido en segundo plano
```
➜ sleep 500 &
[2] 12441

➜ sleep 500 &
[3] 12474

➜ echo $!    
12474

➜ kill -9 $!
[3]  + 12474 killed     sleep 500  
```

---


