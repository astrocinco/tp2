#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "vectorr.h"
#include "usuario.h"
#include "abb.h"
#include "post.h"


struct post{ 
    int nro_id;
    usuario_t* creador;
    char* contenido;
    abb_t* likes; 
};


post_t* crear_post(vector_t* arreglo_st, usuario_t* usuario_activo, char* ingreso){
    post_t* nuevo_post = malloc(sizeof(post_t));
    if (nuevo_post == NULL) return NULL;

    char* ingreso_copiado = malloc(sizeof(char) * TAM_MAX_INGRESO);
    if (ingreso_copiado == NULL) return NULL;
    strcpy(ingreso_copiado, ingreso);

    nuevo_post->nro_id = vector_cantidad(arreglo_st);

    vector_guardar(arreglo_st,nuevo_post);

    nuevo_post->creador = usuario_activo;
    nuevo_post->contenido = ingreso_copiado;
    nuevo_post->likes = abb_crear(strcmp, NULL);

    return nuevo_post;
}


void destruir_post(void* post_void){
    post_t* post = (post_t*)post_void;

    abb_destruir(post->likes);
    free(post->contenido);
    free(post);
}