#define _POSIX_C_SOURCE 200809L
#define TAM_MAX_INGRESO 150
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


typedef struct usuario{ // REVISAR CÓMO HACER PARA NO TENER QUE DECLARARLOS DOS VECES
    char* nombre;
    heap_t* feed; // posts_sin_ver
    size_t id_txt;
} usuario_t;


typedef struct post{ // REVISAR CÓMO HACER PARA NO TENER QUE DECLARARLOS DOS VECES
    size_t nro_id;
    usuario_t* creador;
    char* contenido;
    abb_t* likes; 
} post_t;


typedef struct arreglo_posts{ // REVISAR CÓMO HACER PARA NO TENER QUE DECLARARLOS DOS VECES
    post_t** arreglo;
    size_t cantidad;
} arreglo_posts_t;


typedef struct dupla{
    size_t prioridad;
    post_t* post;
} dupla_t;


// AUXILIARES


post_t* crear_post(arreglo_posts_t* arreglo_st, usuario_t* usuario_activo, char* ingreso){
    post_t* nuevo_post = malloc(sizeof(post_t*));
    if (nuevo_post == NULL) return NULL;

    char* ingreso_copiado = malloc(sizeof(char*) * TAM_MAX_INGRESO);
    char* nombre_usu_copiado = malloc(sizeof(char*) * TAM_MAX_INGRESO);
    strcpy(ingreso_copiado, ingreso);
    strcpy(nombre_usu_copiado, usuario_activo->nombre);

    nuevo_post->nro_id = arreglo_st->cantidad;
    arreglo_st->arreglo[nuevo_post->nro_id] = nuevo_post;
    nuevo_post->creador = usuario_activo;
    nuevo_post->contenido = ingreso_copiado;
    nuevo_post->likes = abb_crear(strcmp, NULL);

    return nuevo_post;
}


dupla_t* crear_dupla(usuario_t* publicador, usuario_t* receptor, post_t* post){
    dupla_t* nueva_dupla = malloc(sizeof(dupla_t));
    if (nueva_dupla == NULL) return NULL;

    ssize_t prioridad = (receptor->id_txt) - (publicador->id_txt);
    if (prioridad < 0) prioridad = prioridad * (-1);
    nueva_dupla->prioridad = prioridad;

    nueva_dupla->post = post;
    return nueva_dupla;
}


bool func_imprimir_likes(const char* clave, void* dato, void* extra){
    printf("    %s", clave);
    return true;
}


// FUNCIONES PARA COMANDOS


usuario_t* login(hash_t* usuarios, usuario_t* usuario_activo){
    char* ingreso_login = NULL;
    size_t buffer;
    getline(&ingreso_login, &buffer, stdin);
    
    if (usuario_activo != NULL){
        printf("Error: Ya habia un usuario loggeado\n" );
        return usuario_activo;
    }
    if (hash_pertenece(usuarios, ingreso_login)){
        usuario_activo = hash_obtener(usuarios, ingreso_login);
        printf("Hola %s", ingreso_login); 
    }else{
        printf("Error: usuario no existente\n");
    }
    free(ingreso_login);
    return usuario_activo;
}


usuario_t* logout(usuario_t* usuario_activo){
    if (usuario_activo != NULL){
        usuario_activo = NULL;
        printf("Adios\n");
    }else{
        printf("Error: no habia usuario loggeado.\n");
    }
    return usuario_activo;
}


void publicar(usuario_t* usuario_activo, arreglo_posts_t* arreglo_posts, hash_t* usuarios){
    char* ingreso_publicar = NULL;
    size_t buffer;
    getline(&ingreso_publicar, &buffer, stdin);

    if (usuario_activo == NULL){
        printf("Error: no habia usuario loggeado.\n" );
        return;
    }
    post_t* nuevo_post = crear_post(arreglo_posts, usuario_activo, ingreso_publicar);
    arreglo_posts->cantidad++;

    hash_iter_t* iterador_usu = hash_iter_crear(usuarios);
    while(!hash_iter_al_final(iterador_usu)){
        const char* nombre_usu = hash_iter_ver_actual(iterador_usu);
        usuario_t* usuario_poner_feed = hash_obtener(usuarios, nombre_usu);
        if (usuario_poner_feed != usuario_activo){
            dupla_t* dupla = crear_dupla(usuario_activo, usuario_poner_feed, nuevo_post);
            heap_encolar(usuario_poner_feed->feed, dupla);
            hash_iter_avanzar(iterador_usu);
        }else{
            hash_iter_avanzar(iterador_usu);
        }
    }
}


void ver_prox(usuario_t* usuario_activo){
    if (heap_esta_vacio(usuario_activo->feed) || usuario_activo == NULL){
        printf("Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }
    dupla_t* dupla = heap_desencolar(usuario_activo->feed);
    printf("    Debug: Post ID %lu - Prioridad: %lu", dupla->post->nro_id, dupla->prioridad);
    printf("dijo %s %s", dupla->post->creador->nombre, dupla->post->contenido);
    printf("Likes: %lu\n", abb_cantidad(dupla->post->likes));
    // TERMINAR. ARREGLAR QUE SE PONEN \N DE MÁS
}


void likear(usuario_t* usuario_activo, arreglo_posts_t* arreglo){
    char* que_id_likear = NULL;
    size_t buffer;
    getline(&que_id_likear, &buffer, stdin);
    if (usuario_activo == NULL) {
        printf("Error: Usuario no loggeado o Post inexistente\n"); 
        return;
    }
    size_t id = atoi(que_id_likear);
    if (id >= arreglo->cantidad) {
        printf("Error: Usuario no loggeado o Post inexistente\n"); 
        return;
    }
    post_t* post_a_likear = arreglo->arreglo[id];
    abb_guardar(post_a_likear->likes, usuario_activo->nombre, NULL);
    printf("Post likeado\n");
}


void ver_likes(usuario_t* usuario_activo, arreglo_posts_t* arreglo){
    char* que_id_mostrar = NULL;
    size_t buffer;
    getline(&que_id_mostrar, &buffer, stdin);
    if (usuario_activo == NULL) {
        printf("Error: Usuario no loggeado o Post inexistente\n"); 
        return;
    }
    size_t id = atoi(que_id_mostrar);
    if (id >= arreglo->cantidad) {
        printf("Error: Usuario no loggeado o Post inexistente\n"); 
        return;
    }
    post_t* post_a_ver = arreglo->arreglo[id];
    printf("El post tiene %lu likes:\n", abb_cantidad(post_a_ver->likes));
    abb_in_order(post_a_ver->likes, func_imprimir_likes, NULL);
}


void debugger(){
    printf("Debugger\n");
}
