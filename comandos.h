#ifndef _COMANDOS_H_
#define _COMANDOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"

typedef struct usuario usuario_t;
typedef struct arreglo_posts arreglo_posts_t;
typedef struct post post_t;
typedef struct dupla dupla_t;

usuario_t* login(hash_t* usuarios, usuario_t* usuario_activo);

usuario_t* logout(usuario_t* usuario_activo);

void publicar(usuario_t* usuario_activo, arreglo_posts_t* arreglo_posts, hash_t* usuarios);

void ver_prox();

void likear();

void ver_likes();

void debugger();

#endif //_COMANDOS_H_