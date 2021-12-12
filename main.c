/*--------------------------------------------------------------------


 -- TO DO --
        LIMPIAR EL HASH ??
        Pasar prueba 7. Si publicas algo muy grande, te muestra solo una porcion. No se guarda todo? No se muestra todo?
        Struct

 -- DUDAS. DISCUTIR -- 


--------------------------------------------------------------------*/


#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"
#include "comandos.h"
#include "post.h"
#include "usuario.h"
#include "dupla.h"
#define NRO_ARGUMENTOS_INGRESO_TXT 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1
#define CAP_CANT_POSTS 50
#define TAM_MAX_INGRESO 30000


//  --- FUNCIONES


void esperar_orden(hash_t* usuarios){
    bool terminar = false;
	char* ingreso = NULL;
	size_t tam_buffer = 0; 

    usuario_t* usuario_activo = NULL;
    vector_t* arreglo_posts = vector_crear();
    
    while(!terminar){
        if(getline(&ingreso, &tam_buffer, stdin) == EOF){
            break;
        }
        if (strcmp(ingreso, "login\n") == 0){
            usuario_activo = login(usuarios, usuario_activo);

        }else if(strcmp(ingreso, "logout\n") == 0){
            usuario_activo = logout(usuario_activo);

        }else if(strcmp(ingreso, "publicar\n") == 0){
            publicar(usuario_activo, arreglo_posts,usuarios);

        }else if(strcmp(ingreso, "ver_siguiente_feed\n") == 0){
            ver_prox(usuario_activo);

        }else if(strcmp(ingreso, "likear_post\n") == 0){
            likear(usuario_activo, arreglo_posts);

        }else if(strcmp(ingreso, "mostrar_likes\n") == 0){
            ver_likes(usuario_activo, arreglo_posts);

        }else if(strcmp(ingreso, "quit\n") == 0){
            terminar = true;
        }else{
            printf("COMANDO INEXISTENTE. INTENTELO DE NUEVO\n");
        }
    }
    free(ingreso);
    vector_destruir(arreglo_posts,post_destruir);
    return;
}


int main(int argc, char *argv[]){
    if (argc != NRO_ARGUMENTOS_INGRESO_TXT) {
        printf("ERROR: el número de argumentos ingresados es erroneo.\n"); 
        return -1;
    }
    if (access(argv[ARGUMENTO_NOMBRE_ARCHIVO], R_OK) == -1) {
        printf("Error: archivo fuente inaccesible"); 
        return -1;
    }
    FILE* archivo = fopen(argv[ARGUMENTO_NOMBRE_ARCHIVO], "r");
    hash_t* hash_usuarios = usuario_guardar_hash(archivo);
    fclose(archivo);

    esperar_orden(hash_usuarios);
    
    hash_destruir(hash_usuarios);
    return 0;
}
