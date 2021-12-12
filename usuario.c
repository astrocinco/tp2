#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "usuario.h"
#include "hash.h"
#include "heap.h"
#include "dupla.h"
#define TAM_MAX_INGRESO 15000

// struct usuario{ 
//     char* nombre;
//     heap_t* feed; 
//     int id_txt;
// };

// Auxiliares

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

// Primitivas

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

void destruir_usuario(void* usuario_void){
    usuario_t* usuario = (usuario_t*)usuario_void;
    free(usuario->nombre);
    heap_destruir(usuario->feed, free);
    free(usuario);
}
