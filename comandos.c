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

/*
int cmp_alfa_usuarios(const char* nombre_liker_1, const char* nombre_liker_2){
    int diferencia = 0;
    int contador = 0;
    while(diferencia == 0){
        if (nombre_liker_1[contador] > nombre_liker_2[contador]){
            return diferencia++;
        }else if (nombre_liker_1[contador] < nombre_liker_2[contador]){
            return diferencia--;
        }else contador++;
    }
    printf("ERROR EN CMP DE ALFABETICO\n");
    return NULL; // NO DEBERÍA LLEGAR ACÁ
}
*/

post_t* crear_post(arreglo_posts_t* arreglo_st, usuario_t* usuario_activo, char* ingreso){
    post_t* nuevo_post = malloc(sizeof(post_t*));
    if (nuevo_post == NULL) return NULL;

    char* ingreso_copiado = malloc(sizeof(char*) * TAM_MAX_INGRESO);

    nuevo_post->nro_id = arreglo_st->cantidad;
    nuevo_post->creador = usuario_activo->nombre;
    strcpy(ingreso_copiado, ingreso);
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
        printf("Hola %s", ingreso_login); // Sin \n porque nos nombres de usuarios en hash ya tienen \n
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

    hash_iter_t* iterador_usu = hash_iter_crear(usuarios);
    printf("antes del while\n");
    while(!hash_iter_al_final(iterador_usu)){
        printf("entro al while\n");
        const char* nombre_usu = hash_iter_ver_actual(iterador_usu);
        usuario_t* usuario_poner_feed = hash_obtener(usuarios, nombre_usu);
        printf("declaraciones\n");
        if (usuario_poner_feed != usuario_activo){
            printf("entra al if\n");
            dupla_t* dupla = crear_dupla(usuario_activo, usuario_poner_feed, nuevo_post);
            heap_encolar(usuario_poner_feed->feed, dupla);
            printf("heap encolar\n");
            
            hash_iter_avanzar(iterador_usu);
            const char* nombre_usu = hash_iter_ver_actual(iterador_usu);
            usuario_t* usuario_poner_feed = hash_obtener(usuarios, nombre_usu);
            printf("actualiza nombre y usuario\n");
        }
    }
    printf("    Debug: Publicaste wey\n");
}


void ver_prox(usuario_t* usuario_activo){
    if (heap_esta_vacio(usuario_activo->feed) || usuario_activo == NULL){
        printf("Usuario no loggeado o no hay mas posts para ver\n");
    }
    post_t* post = heap_desencolar(usuario_activo->feed);
    printf("Post ID %lu\n",post->nro_id);
    printf("%s dijo: %s\n",post->creador->nombre,post->contenido);
    printf("Likes: %lu\n", abb_cantidad(post->likes));

}

void likear(){

}


void ver_likes(){

}


void debugger(){
    printf("Debugger\n");
}
