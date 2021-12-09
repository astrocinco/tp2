#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define CAP_INIC 50lu
#define FACTOR_NVA_CAP 2lu

typedef struct vector{
    void** arreglo;
    size_t cant;
    size_t capa;
} vector_t;

typedef void (*vector_destruir_dato_t) (void *);

// Auxiliares

bool vector_redimensionar(vector_t* vector, size_t nva_cap){
    if (nva_cap < CAP_INIC) return true;
    void** nuevo_arr = realloc(vector->arreglo, sizeof(void*) * nva_cap);
    if (nuevo_arr == NULL) return false;

    vector->arreglo = nuevo_arr;
    vector->capa = nva_cap;
    printf("asdsdaasd\n");
    return true;
}

// Primitivas

vector_t* vector_crear(){
    vector_t* nuevo_vector = malloc(sizeof(vector_t));
    if (nuevo_vector == NULL) return NULL;
    void** nuevo_arreglo = malloc(sizeof(void*) * CAP_INIC);
    if (nuevo_arreglo == NULL) return NULL;

    nuevo_vector->arreglo = nuevo_arreglo;
    nuevo_vector->cant = 0;
    nuevo_vector->capa = CAP_INIC;

    return nuevo_vector; 
}

bool vector_guardar(vector_t* vector, void* dato){
    bool ok = true;
    if (vector->cant == vector->capa-1) ok = vector_redimensionar(vector, vector->capa * FACTOR_NVA_CAP);
    if(!ok) return false;
    
    vector->arreglo[vector->cant] = dato;
    vector->cant++;
    return true;
}

void* vector_obtener(vector_t* vector, size_t pos){
    if (pos >= vector->cant) return NULL;

    return vector->arreglo[pos];
}

size_t vector_cantidad(vector_t* vector){
    return vector->cant;
}

void vector_destruir(vector_t* vector, vector_destruir_dato_t func){
    printf("Llamo a vector_destruir\n");
    if (func != NULL && vector->cant != 0){
        printf("Entré a func destruir por elem\n");
        for (int i = 0; i < vector->cant ; i++){
            func(vector->arreglo[i]);
        }
    }
    free(vector->arreglo);
    free(vector);
}
