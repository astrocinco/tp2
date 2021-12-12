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
#include "dupla.h"

// struct dupla{
//     int prioridad;
//     post_t* post;
// }; 

dupla_t* dupla_crear(usuario_t* publicador, usuario_t* receptor, post_t* post){
    dupla_t* nueva_dupla = malloc(sizeof(dupla_t));
    if (nueva_dupla == NULL) return NULL;

    int prioridad = (receptor->id_txt) - (publicador->id_txt);
    if (prioridad < 0) prioridad = prioridad * (-1);
    nueva_dupla->prioridad = prioridad;

    nueva_dupla->post = post;
    return nueva_dupla;
}

void dupla_destruir(void* dupla){
    free(dupla);
}