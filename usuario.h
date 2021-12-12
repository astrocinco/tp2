#ifndef _USUARIO_H_
#define _USUARIO_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

typedef struct usuario usuario_t;

usuario_t* usuario_crear();

void destruir_usuario(void* usuario_void);

#endif //_USUARIO_H_