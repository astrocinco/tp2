#ifndef _USUARIO_H_
#define _USUARIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "heap.h"

//typedef struct usuario usuario_t;
typedef struct usuario{ 
    char* nombre;
    heap_t* feed; 
    int id_txt;
}usuario_t;

usuario_t* usuario_crear(char* nombre, int id);

hash_t* usuario_guardar_hash(FILE* archivo);

void usuario_destruir(void* usuario_void);

#endif //_USUARIO_H_