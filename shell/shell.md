# Lab: shell

### Búsqueda en $PATH

#### ¿cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?

Los wrappers de la funcion execve proporcionan las siguientes funcionalidades:

- l : se pasan los argumentos en los parametros de la funcion ( argumento variatico )
- v : se pasan los argumentos en un arreglo terminado con NULL
- e : se pasan los argumentos de ambiente en un arreglo terminado en NULL
- p : encuentra el archivo a ejectuar si este se encuentra en algun directorio del PATH

#### ¿Puede la llamada a exec(3) fallar? ¿Cómo se comporta la implementación de la shell en ese caso?

Si, la implementacion llama a `_exit`, finalizando

---

### Comandos built-in

---

### Variables de entorno adicionales

---

### Procesos en segundo plano

---

### Flujo estándar

---

### Tuberías simples (pipes)

---

### Pseudo-variables

---


