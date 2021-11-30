/*--------------------------------------------------------------------


 -- TO DO --
        ARREGLO DE POSTS. 
            STRUCT POST (ID, CREADOR, CONTENIDO, ABB LIKES)
        HASH DE USUARIOS
            STRUCT USUARIO (NOMBRE, HEAP DE POSTS SIN VER TODAVÍA)
    

 -- FUNCIONES QUE DEBE TENER --
        LOGIN
        LOGOUT
        PUBLICAR POST
        VER PROX POST
        LIKEAR POST POR ID
        MOSTRAR LIKE DE POST POR ID


 -- DUDAS. DISCUTIR -- 
        ¿FPRINTF O PRINTF? IMPRIMIMOS NORMALMENTE O EN STDOUT? // 1


--------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "heap.h"
#include "hash.h"
#include "abb.h"
#define NRO_ARGUMENTOS_INGRESO_TXT 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1


// STRUCTS


typedef struct post{
    size_t nro_id;
    char* nombre_creador;
    abb_t* likes;
} post_t;


typedef struct arreglo_posts{
    post_t** arreglo;
    size_t cantidad;
} arreglo_posts_t;


typedef struct usuario{
    char* nombre;
    heap_t* posts_sin_ver;
} usuario_t;


typedef struct todos_usuarios{ // REVISAR SI DEBERÍA HACERSE ASÍ
    hash_t* diccionario_usu;   // TAL VEZ QUE EL HASH NO ESTE ADENTRO DE ESTE STRUCT
} todos_usuarios_t;            // FALTARÍA ALGÚN ATRIBUTO MÁS? NRO_USUARIOS? (el número ya estaría en el hash ¿no?)


// FUNCIONES


void esperar_orden(){
    // GETLINE. RECIBIR COMANDO
    // METER COMANDO EN SWTICHCASE Y EJECUTAR FUNCION CORRESPONDIENTE
    // LOOP HASTA QUE EL USUARIO LLAME A QUIT
    return;
}


int main(int argc, char *argv[]){
    // CHECKEAR NUMERO DE INGRESOS CORRECTO
    if (argc != NRO_ARGUMENTOS_INGRESO_TXT) {
        printf("ERROR: el número de argumentos ingresados es erroneo."); // 1
        return -1;
    }
    // ASEGURARSE QUE EL ARCHIVO EXISTA Y SE PUEDA LEER
    if (access(argv[ARGUMENTO_NOMBRE_ARCHIVO], R_OK) == -1) {
        fprintf(stderr, "Error: archivo fuente inaccesible"); // 1
        return -1;
    }
    // SACAR USUARIOS DEL ARCHIVO Y CREAR LOS USUARIOS. CADA USUARIO METER EN HASH DE USUARIOS. --- SIN TERMINAR
    FILE* archivo = fopen(argv[ARGUMENTO_NOMBRE_ARCHIVO], "r"); //      SEGUIR SACANDO USUARIOS. 

    // LLAMAR FUNCION LOOP ESPERAR ORDEN
    esperar_orden();

    // VOLVER AQUI CUANDO SE LLAME A QUIT EN esperar_orden. ELIMINAR TODAS LAS ESTRUCTURAS, LIBERAR MEMORIA
    return 0;
}