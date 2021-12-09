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
#include "structs.h"
#define NRO_ARGUMENTOS_INGRESO_TXT 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1
#define CAP_CANT_POSTS 50
#define TAM_MAX_INGRESO 30000


//  --- FUNCIONES


void destruir_usuario(void* usuario_void){
    usuario_t* usuario = (usuario_t*)usuario_void;
    free(usuario->nombre);
    heap_destruir(usuario->feed, free);
    free(usuario);
}


void destruir_post(void* post_void){
    post_t* post = (post_t*)post_void;

    abb_destruir(post->likes);
    free(post->contenido);
    free(post);
}


int cmp_posts(const void* a, const void* b){
    // Retorna positivo si la dupla A tiene prioridad. Negativo si la tiene la B
    const dupla_t* dupla1 = (dupla_t*)a;
    const dupla_t* dupla2 = (dupla_t*)b;

    int prioridad_1 = dupla1->prioridad;
    int prioridad_2 = dupla2->prioridad;

    int dif_prioridad = prioridad_2 - prioridad_1;

    // En caso de igual prioridad, el que fue publicado primero tiene prioridad
    if (dif_prioridad == 0){
        int orden_post_1 = dupla1->post->nro_id;
        int orden_post_2 = dupla2->post->nro_id;
        dif_prioridad = orden_post_2 - orden_post_1;
    }
    return dif_prioridad;
}


usuario_t* crear_usuario(char* nombre, int id){
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (!usuario) return NULL;

    usuario->nombre = malloc(sizeof(char) * TAM_MAX_INGRESO);
    if (usuario->nombre == NULL) return NULL;

    strcpy(usuario->nombre, nombre); 
    usuario->feed = heap_crear(cmp_posts); 
    usuario->id_txt = id;
    return usuario;
}


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
    vector_destruir(arreglo_posts,destruir_post);
    return;
}


hash_t* guardar_usuarios_txt_hash(FILE* archivo){
    hash_t* hash = hash_crear(destruir_usuario);
    char* line = NULL;
    size_t capacidad = 0;
    int id = 0;
    ssize_t longitud = getline(&line, &capacidad, archivo); // PONER EZE TRUCO -si

    while(longitud != EOF){ 
        usuario_t* usuario = crear_usuario(line, id);
        hash_guardar(hash, usuario->nombre, usuario);
        id++;
        longitud = getline(&line,&capacidad,archivo);
    }
    free(line);
    return hash;
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
    hash_t* hash_usuarios = guardar_usuarios_txt_hash(archivo);
    fclose(archivo);

    esperar_orden(hash_usuarios);
    
    hash_destruir(hash_usuarios);
    return 0;
}
