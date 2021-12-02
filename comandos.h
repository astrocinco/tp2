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

void login(hash_t* usuarios, usuario_t* usuario_activo); 
// PROBABLEMENTE NO VAYA VOID. PONER TIPO CORRECTO EN .H Y .C
void logout(usuario_t* usuario_activo);

void publicar();

void ver_prox();

void likear();
// PROBABLEMENTE NO VAYA VOID. PONER TIPO CORRECTO EN .H Y .C
void ver_likes();

void debugger();

#endif //_COMANDOS_H_