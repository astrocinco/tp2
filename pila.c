#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define CAP_INIC 10
#define FACTOR_NVO_TAM 2

struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

pila_t *pila_crear(void){

	pila_t *pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;
	
	pila->datos = malloc(CAP_INIC * (sizeof(void*)));

	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	
	pila->cantidad = 0;
	pila->capacidad = CAP_INIC;
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_cambiar_capacidad(pila_t* pila, size_t nuevo_tamano) {	
	void** datos_nuevo = realloc(pila->datos, nuevo_tamano * sizeof(void*)); 
	if (datos_nuevo == NULL) return false;
	
	pila->capacidad = nuevo_tamano;
	pila->datos = datos_nuevo;
	return true;
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){
	if (pila->cantidad == pila->capacidad){
		if (!pila_cambiar_capacidad(pila, pila->capacidad * FACTOR_NVO_TAM)) return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void *pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;
	
	void* elem_sacado = pila_ver_tope(pila);
	pila->cantidad--;
	if (pila->cantidad * FACTOR_NVO_TAM * FACTOR_NVO_TAM < pila->capacidad) {
		size_t nueva_capacidad = pila->capacidad / FACTOR_NVO_TAM;
		if (nueva_capacidad < CAP_INIC) return elem_sacado;
		pila_cambiar_capacidad(pila, nueva_capacidad);
	}
	return elem_sacado;
}

void *pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;
	return pila->datos[pila->cantidad -1];
}
