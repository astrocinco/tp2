#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "abb.h"

typedef struct usuario usuario_t;
typedef struct post post_t;
typedef struct dupla dupla_t;

/*
typedef struct usuario{ 
    char* nombre;
    heap_t* feed; 
    int id_txt;
} usuario_t;

typedef struct post{ 
    int nro_id;
    usuario_t* creador;
    char* contenido;
    abb_t* likes; 
}post_t;

typedef struct dupla{
    int prioridad;
    post_t* post;
}dupla_t; 
*/

#endif //_STRUCTS_H_
