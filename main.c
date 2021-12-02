/*--------------------------------------------------------------------


 -- TO DO --
        ARREGLO DE POSTS. 
            STRUCT POST (ID, CREADOR, CONTENIDO, ABB LIKES)          -- HECHO
        HASH DE USUARIOS
            STRUCT USUARIO (NOMBRE, HEAP DE POSTS SIN VER TODAVÍA)   -- HECHO
        HACER CMP PARA HEAP USANDO id_txt del struct "usuario"
    

 -- FUNCIONES QUE DEBE TENER --
        LOGIN
        LOGOUT
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
#define TAM_MAX_INGRESO 150
#define TAM_MAX_NOMBRE_USU 50

// --- STRUCTS


typedef struct post{
    size_t nro_id;
    char* creador;
    char* contenido;
    abb_t* likes; //lo hice lista asi mientras lo recorremos mostrando y es O(u). // Me parece que no, que tiene que ser ABB así se puede leer in-order. Leer in order no es O(u) tambien?
} post_t;


typedef struct arreglo_posts{
    post_t** arreglo;
    size_t cantidad;
} arreglo_posts_t;


typedef struct usuario{
    char* nombre;
    heap_t* feed; // posts_sin_ver
    size_t id_txt;
} usuario_t;


typedef struct dupla{
    size_t prioridad;
    post_t* post;
} dupla_t;


//  DEBUGGERS - BORRAR PARA ENTREGAR
void impresora_hash(hash_t* hash){
    hash_iter_t* iterador = hash_iter_crear(hash);
    printf("    Comienzo impresora hash\n");
    while(!hash_iter_al_final(iterador)){
        char* clave_actual = hash_iter_ver_actual(iterador);
        void* dato = hash_obtener(hash, clave_actual);
        printf("    Nombre: %s     N°ID: %lu\n", clave_actual, ((usuario_t*)dato)->id_txt);
        hash_iter_avanzar(iterador);
    }
    printf("    Fin impresora hash\n");
}

//  --- FUNCIONES


void esperar_orden(hash_t* usuarios){
    bool terminar = false;
	char* ingreso = malloc(sizeof(char) * TAM_MAX_INGRESO);
	size_t tam_buffer; 

    //no hace falta ponerle tamanio al buffer ni a ingreso, se lo pone solo getline
    //por lo menos en el tp1 no les puse y no tuve problema

    usuario_t* usuario_activo = NULL;


    while(!terminar){
        printf("    Debug: Esperando orden en esperar_orden (main.c)\n");
        size_t longitud = getline(&ingreso, &tam_buffer, stdin);

        if (strcmp(ingreso, "login\n") == 0){
            usuario_activo = login(usuarios, usuario_activo);

        }else if(strcmp(ingreso, "logout\n") == 0){
            usuario_activo = logout(usuario_activo);

        }else if(strcmp(ingreso, "publicar\n") == 0){

        }else if(strcmp(ingreso, "ver_siguiente_feed\n") == 0){

        }else if(strcmp(ingreso, "likear_post\n") == 0){

        }else if(strcmp(ingreso, "mostrar_likes\n") == 0){

        }else if(strcmp(ingreso, "quit\n") == 0){
        // TAL VEZ QUITEAR SEA ingreso == NULL. -- VER QUE RETORNA CONTROL+D EN TERMINAL
            printf("Quiting");
            terminar = true;

        }else{
            printf("COMANDO INEXISTENTE. INTENTELO DE NUEVO\n");
        }
    }
    free(ingreso);
    return;
}

int cmp_posts(const void* a, const void* b){
    // Retorna positivo si la dupla A tiene prioridad. Negativo si la tiene la B
    const dupla_t* dupla1 = (dupla_t*)a;
    const dupla_t* dupla2 = (dupla_t*)b;

    size_t prioridad_1 = dupla1->prioridad;
    size_t prioridad_2 = dupla2->prioridad;

    size_t dif_prioridad = prioridad_1 - prioridad_2;

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



void destruir_post(void* post_void){
    post_t* post = (post_t*)post_void; // Para evitar warnings
    // HACER
}


void destruir_usuario(void* usuario_void){
    usuario_t* usuario = (usuario_t*)usuario_void; // Para evitar warnings
    free(usuario->nombre);
    heap_destruir(usuario->feed, NULL); 
    // CREO QUE DEBERÍA LLAMAR A DESTRUIR_POST o NULL. REVISAR --- AUNQUE TAL VEZ LOS HEAPS NO SE ...
    // ... DEBERIAN ENCARGAR DE DESTRUIR LOS POSTS, SINO EL ARREGLO DE POSTS DEBERÍA DESTRUIRLOS. SI LO DELEGAS A CADA HEAP, TENDRÁS INVALID FREES
    free(usuario);
}


hash_t* guardar_usuarios_txt_hash(FILE* archivo){
    hash_t* hash = hash_crear(destruir_usuario);
    char* line = NULL;
    size_t capacidad;
    size_t longitud = getline(&line, &capacidad, archivo); // PONER EZE TRUCO -si
    size_t id = 0;
    while(longitud > 0){ 
        usuario_t* usuario = crear_usuario(line, id);
        hash_guardar(hash, line, usuario);
        id++;
        longitud = getline(&line,&capacidad,archivo);
    }//esto esta incompleto --- Que le falta? Creo que ahora ya está
    //impresora_hash(hash);
    return hash;
}


int main(int argc, char *argv[]){
    printf("    Debug: Comienza el programa\n");
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

    esperar_orden(hash_usuarios);

    hash_destruir(hash_usuarios);
    return 0;
}
