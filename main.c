/*--------------------------------------------------------------------


 -- TO DO --
        ARREGLO DE POSTS. 
            STRUCT POST (ID, CREADOR, CONTENIDO, ABB LIKES)          -- HECHO
        HASH DE USUARIOS
            STRUCT USUARIO (NOMBRE, HEAP DE POSTS SIN VER TODAVÍA)   -- HECHO
        HACER CMP PARA HEAP USANDO id_txt del struct "usuario"
    

 -- FUNCIONES QUE DEBE TENER --
        LOGIN
        LOGOUT
        PUBLICAR POST
        VER PROX POST
        LIKEAR POST POR ID
        MOSTRAR LIKE DE POST POR ID


 -- DUDAS. DISCUTIR -- 
        - ¿FPRINTF O PRINTF? IMPRIMIMOS NORMALMENTE O EN STDOUT? // 1         SOLUCIONADO -- > ES PRINTF
        - ¿GETLINE LE PASAMOS UN PUNTERO CADA VEZ? O USAMOS SIEMPRE EL MISMO PUNTERO Y COPIAMOS SU CONTENIDO EN EL LUGAR CORRECTO
        - PARA GUARDAR USUARIO LOGEADO: GUARDAR COMO VARIABLE EN esperar_orden ¿no? Y CUANDO NO HAYA NADIE LOGEADO usuario_actual = NULL
        - CORTAR LOOP CON ORDEN EN esperar_orden (LLAMANDO esperar_orden SOLO UNA VEZ DESDE MAIN) O CORTAR CON UN BOOL EN main que se MODIFIQUE DESDE esperar_orden CON PUNTERO
        - INIESTA TIENE MISMA AFINIDAD CON PIQUE QUE CON GUARDIOLA?           SOLUCIONADO -- > NO DA LA VUELTA J

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
//#define _POSIX_C_SOURCE 200809L

// --- STRUCTS


typedef struct post{
    size_t nro_id;
    char* creador;
    char* contenido;
    abb_t* likes;
} post_t;


typedef struct arreglo_posts{
    post_t** arreglo;
    size_t cantidad;
} arreglo_posts_t;


typedef struct usuario{
    char* nombre;
    heap_t* feed; // posts_sin_ver
    size_t id_txt;
} usuario_t;


//  --- FUNCIONES


void esperar_orden(){

    // GETLINE. RECIBIR COMANDO
    // METER COMANDO EN SWTICHCASE Y EJECUTAR FUNCION CORRESPONDIENTE
    // LOOP HASTA QUE EL USUARIO LLAME A QUIT
    return;
}

bool crear_usuario(hash_t* hash,char* nombre,int id){
    usuario_t* usuario = malloc(sizeof(usuario));
    if (!usuario){
        return false;     
    } 

    strcpy(usuario->nombre,nombre);
    heap_t* feed = heap_crear(/* cmp */);
    usuario->feed = feed;
    usuario->id_txt = id;
}


int main(int argc, char *argv[]){
    // CHECKEAR NUMERO DE INGRESOS CORRECTO

    if (argc != NRO_ARGUMENTOS_INGRESO_TXT) {
        printf("ERROR: el número de argumentos ingresados es erroneo."); // 1
        return -1;
    }

    // ASEGURARSE QUE EL ARCHIVO EXISTA Y SE PUEDA LEER
    if (access(argv[ARGUMENTO_NOMBRE_ARCHIVO], R_OK) == -1) {//esto esta en py??

        fprintf(stderr, "Error: archivo fuente inaccesible"); // 1
        return -1;
    }

    // SACAR USUARIOS DEL ARCHIVO Y CREAR LOS USUARIOS. CADA USUARIO METER EN HASH DE USUARIOS. --- SIN TERMINAR

    FILE* archivo = fopen(argv[ARGUMENTO_NOMBRE_ARCHIVO], "r");
    //      SEGUIR SACANDO USUARIOS.
    // CREAR ACÁ HASH USUARIOS? Y MANDAR A esperar_orden
    hash_t* hash = hash_crear(free);
    
    char* line = NULL;
    size_t capacidad;
    ssize_t longitud = getline(&line,&capacidad,archivo);
    int id = 0;

    while(longitud > 0){
        hash_guardar(hash,line,id);
        id++;
        longitud = getline(&line,&capacidad,archivo);
    }//esto esta incompleto



    for linea in archivo:   // HACER
       guardar_usuario_hash // HACER

    // LLAMAR FUNCION ESPERAR ORDEN LOOPEA ADENTRO DE esperar_orden (CONFIRMAR)
    
    esperar_orden();

    // VOLVER ACA CUANDO SE LLAME A QUIT EN esperar_orden. ELIMINAR AQUI TODAS LAS ESTRUCTURAS, LIBERAR MEMORIA
    return 0;
}
