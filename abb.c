#include "abb.h"
#include "pila.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LARGO_MAX_CLAVE 150ul

typedef struct nodo{
	struct nodo *izq;
	struct nodo *der;
	char *clave;
	void *dato;
} nodo_t;

struct abb{
	nodo_t *raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter{
	const abb_t *arbol;
	pila_t *pila_iter;
};


nodo_t *nodo_crear(const char *clave, void *dato){
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo){
		return NULL;
	}
	nodo->clave = malloc(sizeof(char) * LARGO_MAX_CLAVE);
	if (nodo->clave == NULL) return NULL;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->dato = dato;
	strcpy(nodo->clave, clave);
	return nodo;
}


void nodo_destruir(nodo_t *nodo, abb_destruir_dato_t destruir_dato){
	if (destruir_dato != NULL){
		destruir_dato(nodo->dato);
	}
	free(nodo->clave);
	free(nodo);
}

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t *abb = malloc(sizeof(abb_t));
	if (!abb){
		return NULL;
	}
	abb->raiz = NULL;
	abb->cmp = cmp;
	abb->destruir_dato = destruir_dato; 
	abb->cantidad = 0;
	return abb;
}

nodo_t *busqueda(nodo_t *nodo, const char *clave, abb_comparar_clave_t cmp){
	if (nodo == NULL){
		return NULL;
	}
	int comparacion = cmp(nodo->clave, clave);
	if (comparacion == 0){
		return nodo;
	}else if (comparacion > 0){ 
		return busqueda(nodo->izq, clave, cmp);
	}else{
		return busqueda(nodo->der, clave, cmp);
	}
}

nodo_t *busqueda_padre(nodo_t* raiz, nodo_t *nodo, const char *clave, abb_comparar_clave_t cmp)
{
	if (!nodo){
		return NULL;
	}
	int comparacion = cmp(clave, nodo->clave);
	if (comparacion == 0){
		return raiz;
	}
	else if (comparacion < 0){
		return busqueda_padre(nodo, nodo->izq, clave, cmp);
	}
	else{
		return busqueda_padre(nodo, nodo->der, clave, cmp);
	}
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_t* nodo_preexistente = busqueda(arbol->raiz,clave, arbol->cmp);
	if (nodo_preexistente){
		if (arbol->destruir_dato != NULL){
			arbol->destruir_dato(nodo_preexistente->dato);
		}
		nodo_preexistente->dato = dato;
		return true;
	}

	if (!arbol->raiz){
		nodo_t *raiz = nodo_crear(clave, dato);
		arbol->raiz = raiz;
		arbol->cantidad++; 
		return true; 
	}

	bool guardado = false; 
	abb_comparar_clave_t comp = arbol->cmp;
	nodo_t *padre = arbol->raiz;

	int comparacion = comp(clave, padre->clave);
	while (!guardado){
		if (comparacion < 0 && padre->izq != NULL){
			padre = padre->izq;
		}else if (comparacion >= 0 && padre->der != NULL){
			padre = padre->der;
		}
		comparacion = comp(clave, padre->clave);

		if (comparacion < 0 && padre->izq == NULL){
			nodo_t *nuevo = nodo_crear(clave, dato);
			padre->izq = nuevo;
			guardado = true;
		}else if (comparacion >= 0 && padre->der == NULL){
			nodo_t *nuevo = nodo_crear(clave, dato);
			padre->der = nuevo;
			guardado = true;	
		}
	}
	arbol->cantidad++;
	return true;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (arbol->raiz == NULL){
		return false;
	}
	nodo_t *buscado = busqueda(arbol->raiz, clave, arbol->cmp);
	if (buscado == NULL){
		return false;
	} else{
		return true;
	}
}

void borrar_sin_hijos(nodo_t* nodo_padre, nodo_t* nodo, abb_t* arbol){

	if(nodo == arbol->raiz) { 
		arbol->raiz = NULL;

	}else if(nodo_padre->der == nodo){
		nodo_padre->der = NULL;
	}else{
		nodo_padre->izq = NULL;
	}
	nodo_destruir(nodo, arbol->destruir_dato);
	return;
}

void borrar_un_hijo(nodo_t* nodo_padre, nodo_t* nodo, abb_t* arbol){
	if(nodo_padre == NULL){ 
		if (nodo->der == NULL){
			arbol->raiz = nodo->izq;
		}else{
			arbol->raiz = nodo->der;
		}
		nodo_destruir(nodo, arbol->destruir_dato);
		return; 
	}
	if (nodo_padre->der == nodo){
		if (nodo->der != NULL){
			nodo_padre->der = nodo->der;
		}else{
			nodo_padre->der = nodo->izq;
		}
	}else{
		if (nodo->der != NULL){
			nodo_padre->izq = nodo->der;
		}else{
			nodo_padre->izq = nodo->izq;
		}
	}
	nodo_destruir(nodo, arbol->destruir_dato);
	return;
}

void borrar_dos_hijos(nodo_t* nodo, abb_t* arbol){ // Revisar
	nodo_t* nodo_reemplazante = nodo->der;
	while(nodo_reemplazante->izq != NULL){
		nodo_reemplazante = nodo_reemplazante->izq;
	}
	char* clave_copia = malloc(sizeof(char) * LARGO_MAX_CLAVE);
	strcpy(clave_copia, nodo_reemplazante->clave);
	
	char* clave = nodo_reemplazante->clave;
	void* dato = abb_borrar(arbol, clave);
	strcpy(nodo->clave, clave_copia);

	free(clave_copia);
	nodo->dato = dato;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (!abb_pertenece(arbol, clave)){
		return NULL;
	}
	abb_comparar_clave_t func_cmp = arbol->cmp; 
	nodo_t *nodo;
	nodo_t *padre = busqueda_padre(NULL, arbol->raiz, clave, func_cmp);

	if (padre == NULL){
		nodo = arbol->raiz;
	}else if ( (padre-> izq != NULL) && arbol->cmp(padre->izq->clave,clave) == 0){
		nodo = padre->izq;
	}else {
		nodo = padre->der;
	}
	void *dato = nodo->dato;

	if (nodo->izq == NULL && nodo->der == NULL){
		borrar_sin_hijos(padre, nodo, arbol);
	}else if (nodo->izq == NULL || nodo->der == NULL){
		borrar_un_hijo(padre, nodo, arbol);
	}else{
		borrar_dos_hijos(nodo, arbol);
		arbol->cantidad++;//porque borrar dos hijos vuelve a llamas a abb_borrar
	}
	arbol->cantidad--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	if (!abb_pertenece(arbol, clave)){
		return NULL;
	}
	nodo_t *buscado = busqueda(arbol->raiz, clave, arbol->cmp);
	if (buscado != NULL){
		return buscado->dato;
	}else{
		return NULL; 
	}
}

size_t abb_cantidad(const abb_t *arbol){
	return arbol->cantidad;
}

void post_order_dest_dato(nodo_t *nodo, abb_destruir_dato_t destruir){ 
	if (!nodo){
		return;
	}
	post_order_dest_dato(nodo->izq, destruir);
	post_order_dest_dato(nodo->der, destruir);
	if (destruir != NULL){
		destruir(nodo->dato);
	}
	free(nodo->clave);
	free(nodo);
}

void abb_destruir(abb_t *arbol){
	post_order_dest_dato(arbol->raiz,arbol->destruir_dato);
	free(arbol);
}
/*
void pre_order(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra){
	if (nodo == NULL){
		return;
	}
	visitar(nodo->clave, nodo->dato, extra);
	pre_order(nodo->izq, visitar, extra);
	pre_order(nodo->der, visitar, extra);

}
void abb_pre_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (!arbol->raiz){
		return;
	}
	pre_order(arbol->raiz, visitar, extra);
}
*/

void in_order(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra, bool* anterior){
	if(nodo == NULL){
		return;
	}
	if (*anterior){
		in_order(nodo->izq, visitar, extra,anterior);
	}
	if (*anterior){
		bool visita = visitar(nodo->clave, nodo->dato, extra);
		(*anterior) =visita;
	}
	if (*anterior){
		in_order(nodo->der, visitar, extra, anterior);
	}
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if (!arbol->raiz){
		return;
	}
	bool condicion =true; 
	in_order(arbol->raiz, visitar, extra,&condicion);
}

// Iterador externo - IN ORDER

void abb_iter_aux_apilar_hijos_izq(nodo_t *raiz, pila_t *pila){
	if (raiz == NULL){
		return;
	}
	pila_apilar(pila, raiz);
	abb_iter_aux_apilar_hijos_izq(raiz->izq, pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t *iterador = malloc(sizeof(abb_iter_t));
	if (iterador == NULL){
		return NULL;
	}
	iterador->arbol = arbol;
	iterador->pila_iter = pila_crear();
	abb_iter_aux_apilar_hijos_izq(iterador->arbol->raiz, iterador->pila_iter);

	return iterador;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)){ 
		return false;
	}
	nodo_t* desapilado = pila_desapilar(iter->pila_iter);
	abb_iter_aux_apilar_hijos_izq(desapilado->der, iter->pila_iter);
	return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t *iter){
	if (abb_iter_in_al_final(iter))
		return NULL;

	nodo_t* tope_pila_iterador = pila_ver_tope(iter->pila_iter);
	return tope_pila_iterador->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila_iter);
}

void abb_iter_in_destruir(abb_iter_t *iter){
	nodo_t *nodos_desapilados;
	while (!pila_esta_vacia(iter->pila_iter)){
		nodos_desapilados = pila_desapilar(iter->pila_iter);
		nodo_destruir(nodos_desapilados, *(iter->arbol->destruir_dato)); 
	}
	pila_destruir(iter->pila_iter);
	free(iter);
}