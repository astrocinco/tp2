#define _POSIX_C_SOURCE 200809L
#define TAM_MAX_INGRESO 30000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"
#include "comandos.h"
#include "vectorr.h"
#include "usuario.h"
#include "post.h"
#include "dupla.h"


// AUXILIARES

bool func_imprimir_likes(const char* clave, void* dato, void* extra){
    printf("\t%s", clave);
    return true;
}


// FUNCIONES PARA COMANDOS


usuario_t* login(hash_t* usuarios, usuario_t* usuario_activo){
    char* ingreso_login = NULL;
    size_t buffer = 0;
    if(getline(&ingreso_login, &buffer, stdin) == EOF){
        free(ingreso_login);
        return NULL;
    }
    
    if (usuario_activo != NULL){
        printf("Error: Ya habia un usuario loggeado\n" );
        free(ingreso_login);
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
        printf("Error: no habia usuario loggeado\n");
    }
    return usuario_activo;
}


void publicar(usuario_t* usuario_activo, vector_t* arreglo_posts, hash_t* usuarios){
    char* ingreso_publicar = NULL;
    size_t buffer = 0;
    if(getline(&ingreso_publicar, &buffer, stdin) == EOF){
        return;
    }
    if (usuario_activo == NULL){
        printf("Error: no habia usuario loggeado\n" );
        free(ingreso_publicar);
        return;
    }
    
    post_t* nuevo_post = crear_post(arreglo_posts, usuario_activo, ingreso_publicar);

    hash_iter_t* iter = hash_iter_crear(usuarios);
    while(!hash_iter_al_final(iter)){
        const char* user_name = hash_iter_ver_actual(iter);
        usuario_t* usuario = hash_obtener(usuarios,user_name);

        if(usuario != usuario_activo){
            dupla_t* dupla = crear_dupla(usuario_activo, usuario, nuevo_post);
            heap_encolar(usuario->feed,dupla);
        }
        hash_iter_avanzar(iter);
    }
    printf("Post publicado\n");
    hash_iter_destruir(iter);
    free(ingreso_publicar);
}


void imprimir_sin_barra_n(char* cadena){
    int numero = 0;
    char caracter = cadena[numero];
    while (caracter != '\n'){
        printf("%c", caracter);
        numero++;
        caracter = cadena[numero];
    }
}


void ver_prox(usuario_t* usuario_activo){
    if (usuario_activo == NULL || heap_esta_vacio(usuario_activo->feed)){
        printf("Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }
    dupla_t* dupla = heap_desencolar(usuario_activo->feed);
    printf("Post ID %d\n", dupla->post->nro_id);
    imprimir_sin_barra_n(dupla->post->creador->nombre);
    printf(" dijo: %s", dupla->post->contenido);
    printf("Likes: %lu\n", abb_cantidad(dupla->post->likes));
    free(dupla);
}


void likear(usuario_t* usuario_activo, vector_t* arreglo){
    char* que_id_likear = NULL;
    size_t buffer = 0;
    if(getline(&que_id_likear, &buffer, stdin) == EOF){
        return;
    }
    if (usuario_activo == NULL) {
        printf("Error: Usuario no loggeado o Post inexistente\n"); 
        free(que_id_likear);
        return;
    }
    int id = atoi(que_id_likear);
    if (id >= vector_cantidad(arreglo)) {
        printf("Error: Usuario no loggeado o Post inexistente\n"); 
        free(que_id_likear);
        return;
    }
    free(que_id_likear);
    post_t* post_a_likear =  vector_obtener(arreglo,id);
    abb_guardar(post_a_likear->likes, usuario_activo->nombre, NULL);
    printf("Post likeado\n");
}


void ver_likes(usuario_t* usuario_activo, vector_t* arreglo){
    char* que_id_mostrar = NULL;
    size_t buffer = 0;
    if(getline(&que_id_mostrar, &buffer, stdin) == EOF){
        return;
    }
    int id = atoi(que_id_mostrar);
    free(que_id_mostrar);
    post_t* post_a_ver = vector_obtener(arreglo,id);

    if (id >= vector_cantidad(arreglo) || abb_cantidad(post_a_ver->likes) == 0) {
        printf("Error: Post inexistente o sin likes\n"); 
        return;
    }
    printf("El post tiene %lu likes:\n", abb_cantidad(post_a_ver->likes));
    abb_in_order(post_a_ver->likes, func_imprimir_likes, NULL);
}
