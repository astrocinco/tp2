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
#include <string.h>
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "comandos.h"
#define NRO_ARGUMENTOS_INGRESO_TXT 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1
#define TAM_MAX_INGRESO 150
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
    bool terminar = false;
	char* ingreso = malloc(sizeof(char) * TAM_MAX_INGRESO);
	int tam_buffer = 1000; // HACER CONSTANTE O VER DE DONDE SACAR.

    while(!terminar){
        int nro_car = getline(&ingreso, &tam_buffer, stdin);
        if (strcmp(ingreso, "login\n") == 0){
            printf("QUERÉS LOGEARTE CHIGADO?\n");
            login();

        }else if(strcmp(ingreso, "logout\n") == 0){
            printf("QUERÉS salir CHIGADO?\n");
            debugger();

        }else if(strcmp(ingreso, "publicar\n") == 0){
            printf("QUERÉS publicar CHIGADO?\n");

        }else if(strcmp(ingreso, "ver_siguiente_feed\n") == 0){
            printf("QUERÉS ver_siguiente CHIGADO?\n");

        }else if(strcmp(ingreso, "likear_post\n") == 0){
            printf("QUERÉS likear CHIGADO?\n");

        }else if(strcmp(ingreso, "mostrar_likes\n") == 0){
            printf("QUERÉS mostrar_likes CHIGADO?\n");

        }else if(strcmp(ingreso, "quit\n") == 0){ // TAL VEZ QUITEAR SEA ingreso == NULL. -- VER
            printf("quitEAMOS CHIGADO!\n");
            terminar = true;

        }else{
            printf("COMANDO INEXISTENTE. INTENTELO DE NUEVO, CHIGADO\n");
        }
    }
    free(ingreso);
    return;
}


bool crear_usuario(hash_t* hash,char* nombre,int id){
    usuario_t* usuario = malloc(sizeof(usuario));
    if (!usuario){
        return false;     
    } 
    strcpy(usuario->nombre,nombre);
    //usuario->feed;//  = heap_crear(/* cmp */); // -- HACER
    usuario->id_txt = id;
    return true;
}


hash_t* guardar_usuarios_txt_hash(FILE* archivo){ // TERMINAR
    hash_t* hash = hash_crear(free);
    char* line = NULL;
    size_t capacidad;
    ssize_t longitud = getline(&line,&capacidad,archivo); // PONER EZE TRUCO
    int id = 0;

    while(longitud > 0){
        hash_guardar(hash,line,&id);
        id++;
        longitud = getline(&line,&capacidad,archivo);
    }//esto esta incompleto
    return hash;
}


int main(int argc, char *argv[]){
    if (argc != NRO_ARGUMENTOS_INGRESO_TXT) {
        printf("ERROR: el número de argumentos ingresados es erroneo."); 
        return -1;
    }
    if (access(argv[ARGUMENTO_NOMBRE_ARCHIVO], R_OK) == -1) {//esto esta en py?? -- Nono, C de Cristiano. Cómo checkeaste vos si un archivo era valido en el tp1?
        printf("Error: archivo fuente inaccesible"); 
        return -1;
    }
    FILE* archivo = fopen(argv[ARGUMENTO_NOMBRE_ARCHIVO], "r");
    hash_t* hash_usuarios = guardar_usuarios_txt_hash(archivo);
    fclose(archivo);
    
    esperar_orden();
    //hash_destruir(hash_usuarios);

    // VOLVER ACA CUANDO SE LLAME A QUIT EN esperar_orden. ELIMINAR AQUI TODAS LAS ESTRUCTURAS, LIBERAR MEMORIA
    return 0;
}
