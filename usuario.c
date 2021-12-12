#ifndef _USUARIO_H_
#define _USUARIO_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "usuario.h"

usuario_t* crear_usuario(char* nombre, int id){
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (!usuario) return NULL;

    usuario->nombre = malloc(sizeof(char) * TAM_MAX_INGRESO);
    if (usuario->nombre == NULL) return NULL;

    strcpy(usuario->nombre, nombre); 
    usuario->feed = heap_crear(cmp_posts); 
    usuario->id_txt = id;
    return usuario;
}

hash_t* guardar_usuarios_txt_hash(FILE* archivo){
    hash_t* hash = hash_crear(destruir_usuario);
    char* line = NULL;
    size_t capacidad = 0;
    int id = 0;
    ssize_t longitud = getline(&line, &capacidad, archivo); // PONER EZE TRUCO -si

    while(longitud != EOF){ 
        usuario_t* usuario = crear_usuario(line, id);
        hash_guardar(hash, usuario->nombre, usuario);
        id++;
        longitud = getline(&line,&capacidad,archivo);
    }
    free(line);
    return hash;
}

void destruir_usuario(void* usuario_void){
    usuario_t* usuario = (usuario_t*)usuario_void;
    free(usuario->nombre);
    heap_destruir(usuario->feed, free);
    free(usuario);
}