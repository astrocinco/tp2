#define _POSIX_C_SOURCE 200809L
#define TAM_MAX_INGRESO 150
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


usuario_t* login(hash_t* usuarios, usuario_t* usuario_activo){

    char* ingreso_login = malloc(sizeof(char) * TAM_MAX_INGRESO);
    size_t buffer;
    size_t nro_car = getline(&ingreso_login, &buffer, stdin);
    
    if (usuario_activo != NULL){
        printf("Error: Ya habia un usuario loggeado\n" );
        return usuario_activo;
    }
    if (hash_pertenece(usuarios, ingreso_login)){
        usuario_activo = hash_obtener(usuarios, ingreso_login);
        printf("Hola %s", ingreso_login); // Sin \n porque nos nombres de usuarios en hash ya tienen \n
    }else{
        printf("Error: usuario no existente\n");
    }
    return usuario_activo;
}

/*
void login_2(hash_t* usuarios, usuario_t* usuario_activo){ // INGREESAR ALGO A ESTA FUNCIÓN DA SEG FAULT
    // Por lo que interpreto de las pruebas, tenes que dejar que ingrese un usuario antes de decirle que se equivocó
    if (usuario_activo != NULL){
        printf("Error: Ya habia un usuario loggeado\n" );
        // Acá debería co
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
*/

usuario_t* logout(usuario_t* usuario_activo){
    if (usuario_activo != NULL){
        usuario_activo = NULL;
        printf("Adios\n");
    }else{
        printf("Error: no habia usuario loggeado.\n");
    }
    return usuario_activo;
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
