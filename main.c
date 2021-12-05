/*--------------------------------------------------------------------


 -- TO DO --
        ARREGLO DE POSTS. 
            STRUCT POST (ID, CREADOR, CONTENIDO, ABB LIKES)          -- HECHO
        HASH DE USUARIOS                                             -- HECHO
            STRUCT USUARIO (NOMBRE, HEAP DE POSTS SIN VER TODAVÍA)   -- HECHO
        HACER CMP PARA HEAP                                          -- HECHO
    

 -- FUNCIONES QUE DEBE TENER --
        LOGIN                           -- HECHO
        LOGOUT                          -- HECHO
        PUBLICAR POST
        VER PROX POST
        LIKEAR POST POR ID
        MOSTRAR LIKE DE POST POR ID


 -- DUDAS. DISCUTIR -- 
        - ¿FPRINTF O PRINTF? IMPRIMIMOS NORMALMENTE O EN STDOUT? // 1         SOLUCIONADO -- > ES PRINTF
        - ¿GETLINE LE PASAMOS UN PUNTERO CADA VEZ? O USAMOS SIEMPRE EL MISMO PUNTERO Y COPIAMOS SU CONTENIDO EN EL LUGAR CORRECTO
        - PARA GUARDAR USUARIO LOGEADO: GUARDAR COMO VARIABLE EN esperar_orden ¿no? Y CUANDO NO HAYA NADIE LOGEADO usuario_actual = NULL
        - CORTAR LOOP CON ORDEN EN esperar_orden (LLAMANDO esperar_orden SOLO UNA VEZ DESDE MAIN) O CORTAR CON UN BOOL EN main que se MODIFIQUE DESDE esperar_orden CON PUNTERO
        LISTO - INIESTA TIENE MISMA AFINIDAD CON PIQUE QUE CON GUARDIOLA?           SOLUCIONADO -- > NO DA LA VUELTA J

        - se guarda el logeado en el main o hacemos un struct

--------------------------------------------------------------------*/


#define _POSIX_C_SOURCE 200809L
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
#define NRO_ARGUMENTOS_INGRESO_TXT 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1
#define CAP_CANT_POSTS 50ul
#define TAM_MAX_INGRESO 150
#define TAM_MAX_NOMBRE_USU 50


typedef struct usuario{ // REVISAR CÓMO HACER PARA NO TENER QUE DECLARARLOS DOS VECES
    char* nombre;
    heap_t* feed; // posts_sin_ver
    size_t id_txt;
} usuario_t;


typedef struct post{ // REVISAR CÓMO HACER PARA NO TENER QUE DECLARARLOS DOS VECES
    size_t nro_id;
    usuario_t* creador;
    char* contenido;
    abb_t* likes; 
} post_t;


typedef struct arreglo_posts{ // REVISAR CÓMO HACER PARA NO TENER QUE DECLARARLOS DOS VECES
    post_t** arreglo;
    size_t cantidad;
} arreglo_posts_t;


typedef struct dupla{
    size_t prioridad;
    post_t* post;
} dupla_t;


//  DEBUGGERS - BORRAR PARA ENTREGAR
void impresora_hash(hash_t* hash){
    hash_iter_t* iterador = hash_iter_crear(hash);
    printf("    Comienzo impresora hash\n");
    while(!hash_iter_al_final(iterador)){
        const char* clave_actual = hash_iter_ver_actual(iterador);
        void* dato = hash_obtener(hash, clave_actual);
        printf("         N°ID: %lu Nombre: %s", ((usuario_t*)dato)->id_txt, clave_actual);
        hash_iter_avanzar(iterador);
    }
    printf("    Fin impresora hash\n");
}


//  DEBUGGERS - BORRAR PARA ENTREGAR
void debugger_feeds(heap_t* feed){
    while(!heap_esta_vacio(feed)){
        dupla_t* elem = heap_desencolar(feed);
        printf("    Func debugger: ID: %lu - Contenido: %s Publicador: %s", elem->post->nro_id, elem->post->contenido, elem->post->creador->nombre);
    }
}


//  --- FUNCIONES


arreglo_posts_t* crear_arreglo(){//y si usamos el tda vector??
    arreglo_posts_t* arreglo_st = malloc(sizeof(arreglo_posts_t));
    if (arreglo_st == NULL){
        return NULL;
    }
    arreglo_st->arreglo = malloc(sizeof(void*) * CAP_CANT_POSTS);
    arreglo_st->cantidad = 0;
    return arreglo_st;
}


void destruir_usuario(void* usuario_void){
    usuario_t* usuario = (usuario_t*)usuario_void; // Para evitar warnings
    free(usuario->nombre);
    heap_destruir(usuario->feed, NULL); 
    // CREO QUE DEBERÍA LLAMAR A DESTRUIR_POST o NULL. REVISAR --- AUNQUE TAL VEZ LOS HEAPS NO SE ...
    // ... DEBERIAN ENCARGAR DE DESTRUIR LOS POSTS, SINO EL ARREGLO DE POSTS DEBERÍA DESTRUIRLOS. SI LO DELEGAS A CADA HEAP, TENDRÁS INVALID FREES
    free(usuario);
}


void destruir_post(void* post_void){
    post_t* post = (post_t*)post_void; // Para evitar warnings

    abb_destruir(post->likes);
    free(post->contenido);
    free(post);
}

void destruir_arreglo(arreglo_posts_t* arreglo_st){
    for (size_t i = 0; i < arreglo_st->cantidad; i++){
        destruir_post(arreglo_st->arreglo[i]);
    }
    free(arreglo_st->arreglo);
    free(arreglo_st);
}

int cmp_posts(const void* a, const void* b){
    // Retorna positivo si la dupla A tiene prioridad. Negativo si la tiene la B
    const dupla_t* dupla1 = (dupla_t*)a;
    const dupla_t* dupla2 = (dupla_t*)b;

    size_t prioridad_1 = dupla1->prioridad;
    size_t prioridad_2 = dupla2->prioridad;

    int dif_prioridad = prioridad_2 - prioridad_1;

    // En caso de igual prioridad, el que fue publicado primero tiene prioridad
    if (dif_prioridad == 0){
        size_t orden_post_1 = dupla1->post->nro_id;
        size_t orden_post_2 = dupla2->post->nro_id;
        dif_prioridad = orden_post_2 - orden_post_1;
    }
    return dif_prioridad;
}


usuario_t* crear_usuario(char* nombre, size_t id){
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (!usuario) return NULL;

    usuario->nombre = malloc(sizeof(char) * TAM_MAX_NOMBRE_USU);
    if (usuario->nombre == NULL) return NULL;

    strcpy(usuario->nombre, nombre); 
    usuario->feed = heap_crear(cmp_posts); 
    usuario->id_txt = id;
    return usuario;
}

void esperar_orden(hash_t* usuarios){
    bool terminar = false;
	char* ingreso = NULL;
	size_t tam_buffer; 

    usuario_t* usuario_activo = NULL;
    arreglo_posts_t* arreglo_posts = crear_arreglo();
    

    while(!terminar){
        //printf("    Debug: Esperando orden en esperar_orden (main.c)\n");
        getline(&ingreso, &tam_buffer, stdin);

        if (strcmp(ingreso, "login\n") == 0){
            usuario_activo = login(usuarios, usuario_activo);

        }else if(strcmp(ingreso, "logout\n") == 0){
            usuario_activo = logout(usuario_activo);

        }else if(strcmp(ingreso, "publicar\n") == 0){
            publicar(usuario_activo, arreglo_posts,usuarios);

        }else if(strcmp(ingreso, "ver_siguiente_feed\n") == 0){
            ver_prox(usuario_activo);

        }else if(strcmp(ingreso, "likear_post\n") == 0){
            likear(usuario_activo, arreglo_posts);

        }else if(strcmp(ingreso, "mostrar_likes\n") == 0){
            ver_likes(usuario_activo, arreglo_posts);

        }else if(strcmp(ingreso, "quit\n") == 0){
        // TAL VEZ QUITEAR SEA ingreso == NULL. -- VER QUE RETORNA CONTROL+D EN TERMINAL
            printf("Quiting\n");
            terminar = true;
        }else{
            printf("COMANDO INEXISTENTE. INTENTELO DE NUEVO\n");
        }
    }
    free(ingreso);
    destruir_arreglo(arreglo_posts);
    return;
}

hash_t* guardar_usuarios_txt_hash(FILE* archivo){
    hash_t* hash = hash_crear(destruir_usuario);
    char* line = NULL;
    size_t capacidad;
    ssize_t longitud = getline(&line, &capacidad, archivo); // PONER EZE TRUCO -si
    size_t id = 0;
    while(longitud > 0){ 
        usuario_t* usuario = crear_usuario(line, id);
        hash_guardar(hash, usuario->nombre, usuario);
        id++;
        longitud = getline(&line,&capacidad,archivo);
    }
    free(line);
    return hash;
}


int main(int argc, char *argv[]){
    if (argc != NRO_ARGUMENTOS_INGRESO_TXT) {
        printf("ERROR: el número de argumentos ingresados es erroneo.\n"); 
        return -1;
    }
    if (access(argv[ARGUMENTO_NOMBRE_ARCHIVO], R_OK) == -1) {
        printf("Error: archivo fuente inaccesible"); 
        return -1;
    }
    FILE* archivo = fopen(argv[ARGUMENTO_NOMBRE_ARCHIVO], "r");
    hash_t* hash_usuarios = guardar_usuarios_txt_hash(archivo);
    fclose(archivo);

    hash_iter_t* iter = hash_iter_crear(hash_usuarios);
    // impresora_hash(hash_usuarios); // FUNCION DEBUG. BORRAR LUEGO

    esperar_orden(hash_usuarios);
    
    hash_destruir(hash_usuarios);
    return 0;
}
