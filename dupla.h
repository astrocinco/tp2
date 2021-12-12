#ifndef _DUPLA_H_
#define _DUPLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"
#include "comandos.h"
#include "vectorr.h"
#include "usuario.h"
#include "post.h"

typedef struct dupla dupla_t;

dupla_t* crear_dupla(usuario_t* publicador, usuario_t* receptor, post_t* post);

#endif // _DUPLA_H_