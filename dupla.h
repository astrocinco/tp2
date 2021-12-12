#ifndef _DUPLA_H_
#define _DUPLA_H_

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
#include "vectorr.h"
#include "usuario.h"
#include "post.h"

//typedef struct dupla dupla_t;
typedef struct dupla{
    int prioridad;
    post_t* post;
}dupla_t; 

dupla_t* dupla_crear(usuario_t* publicador, usuario_t* receptor, post_t* post);

void dupla_destruir(void* dupla);

#endif // _DUPLA_H_