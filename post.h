#ifndef _POST_H_
#define _POST_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "vectorr.h"
#include "usuario.h"

typedef struct post{
    int nro_id;
    usuario_t* creador;
    char* contenido;
    abb_t* likes; 
} post_t;

post_t* post_crear(vector_t* arreglo_st, usuario_t* usuario_activo, char* ingreso);

void post_destruir(void* post_void);

#endif //_POST_H_