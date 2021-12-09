#ifndef _VECTORR_
#define _VECTORR_

#include <stdbool.h>
#include <stddef.h> 

typedef struct vector vector_t;

// Primitivas

vector_t* vector_crear();

bool vector_guardar();

void* vector_obtener();

size_t vector_cantidad();

void vector_destruir();

#endif //_VECTORR_