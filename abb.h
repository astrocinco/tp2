#ifndef _ABB_H_
#define _ABB_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


//PRIMITIVAS ABB

/*crea un abb vacio que luego usara las funciones indicadas de comparacion y de destruccion.
 *POST: devuelve un puntero al abb. NULL en caso de error.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* guarda un elemento en el abb.
 *PRE:el abb fue creado, y la clave y el dato, inicializados.
 *POST: devuelve true si fue guardado correctamente, false en otro caso.
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/*borra un elemento del abb.
 *PRE:el abb fue creado
 *POST:devuelve un puntero al elemento o NULL si el elemento no estaba guardado
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/*se obtiene el dato asociado a una clave.
 *PRE:el abb fue creado.
 *POST:devuelve un puntero al dato asociado a la clave o NULL si no se encuentra la clave.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/*PRE: el abb fue creado, la clave declarada.
 *POST: devuelve true si la clave esta guardada en el abb, false si no.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/*devuelve la cantidad de elementos que estan guardados en el abb.
 *PRE:el abb fue creado.
 */
size_t abb_cantidad(const abb_t *arbol);

/*Destruye el abb usando la funcion recibida cuando fue creado.
 *POST: destruye el abb y libera la memoria.
 */
void abb_destruir(abb_t *arbol);



//iterador
/*recorre el abb siguiendo un recorrido in-order y aplicando la funcion visitar en todos sus elementos.
 *PRE:el abb fue creado, la funcion debe existir, y extra debe estar declarado. 
 *POST: se aplica la funcion visitar en todos los elementos.
 */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);



//iterador externo


/*crea el iterador externo.
 *PRE: el arbol dbe haber sido creado. 
 *POST:devuelve un iterador externo posicionado en el primer elemento, y que seguira recorrido in-order
 */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*avanza una posicion el iterador.
 *PRE:el iterador fue creado.
 *POST:el iterador se encuentra en el siguiente elemento.
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*ver la clave actual del iterador.
 *PRE: el iterador fue creado.
 *POST:devuelve la clave del elemento sobre la que se encuentra el iterador.
 */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*devuelve true si el iterador se encuentra al final del abb, false si no.
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*destruye el iterador.
 *PRE:el iterador fue creado.
 *POST:libera la memoria pedida.
 */
void abb_iter_in_destruir(abb_iter_t* iter);

#endif //_ABB_H_