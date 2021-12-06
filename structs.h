#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"

typedef struct usuario{ 
    char* nombre;
    heap_t* feed; 
    size_t id_txt;
} usuario_t;


typedef struct post{ 
    size_t nro_id;
    usuario_t* creador;
    char* contenido;
    abb_t* likes; 
} post_t;


typedef struct arreglo_posts{ 
    post_t** arreglo;
    size_t cantidad;
} arreglo_posts_t;


typedef struct dupla{
    size_t prioridad;
    post_t* post;
} dupla_t;

#endif //_STRUCTS_H_