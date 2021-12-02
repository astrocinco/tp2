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


void login(hash_t* usuarios, usuario_t* usuario_activo){ // INGREESAR ALGO A ESTA FUNCIÓN DA SEG FAULT
    if (usuario_activo != NULL){
        printf("Error: Ya habia un usuario loggeado\n" );
    }
    char* user;
    size_t buffer;
    ssize_t longitud = getline(&user,&buffer,stdin);

    if (hash_pertenece(usuarios,user)){ // DABA WARNINIG "STDIN" if (hash_pertenece(usuarios,stdin)){ 
        usuario_activo = hash_obtener(usuarios,user); // DABA WARNINIG "STDIN" usuario_activo = hash_obtener(usuarios,stdin);
        printf("Hola %s\n",user); // ESTABA Y NO COMPILABA printf("Hola %s\n",line);
    }else{
        printf("Error: usuario no existente\n");
    }
}
// PROBABLEMENTE NO VAYA VOID. PONER TIPO CORRECTO EN .H Y .C
void logout(usuario_t* usuario_activo){
    if (usuario_activo != NULL){
        usuario_activo = NULL;
        printf("Adios\n");
    }else{
        printf("Error: no habia usuario loggeado.\n");
    }
}

void publicar(){

}
// PROBABLEMENTE NO VAYA VOID. PONER TIPO CORRECTO EN .H Y .C
void ver_prox(){

}

void likear(){

}
// PROBABLEMENTE NO VAYA VOID. PONER TIPO CORRECTO EN .H Y .C
void ver_likes(){

}

void debugger(){
    printf("Debugger\n");
}
