#ifndef _VECTORR_
#define _VECTORR_

#include <stdbool.h>
#include <stddef.h> 

typedef struct vector vector_t;
typedef void (*vector_destruir_dato_t) (void *);

// Primitivas

vector_t* vector_crear();

bool vector_guardar(vector_t* vector, void* dato);

void* vector_obtener(vector_t* vector, size_t pos);

size_t vector_cantidad(vector_t* vector);

void vector_destruir(vector_t* vector, vector_destruir_dato_t func);

#endif //_VECTORR_