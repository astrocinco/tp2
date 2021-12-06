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
#include "structs.h"

usuario_t* login(hash_t* usuarios, usuario_t* usuario_activo);

usuario_t* logout(usuario_t* usuario_activo);

void publicar(usuario_t* usuario_activo, arreglo_posts_t* arreglo_posts, hash_t* usuarios);

void ver_prox(usuario_t* usuario_activo);

void likear(usuario_t* usuario_activo, arreglo_posts_t* arreglo);

void ver_likes(usuario_t* usuario_activo, arreglo_posts_t* arreglo);

void debugger();

#endif //_COMANDOS_H_