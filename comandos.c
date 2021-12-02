#include <stdio.h>








void login(hash_t* usuarios, usuario_t* usuario_activo){
    if (usuario_activo != NULL){
        printf("Error: Ya habia un usuario loggeado\n" );
    }
    char* user;
    int buffer;
    int longitud = getline(&user,&buffer,stdin);

    if (hash_pertenece(usuarios,stdin)){
        usuario_activo = hash_obtener(usuarios,stdin);
        printf("Hola %s\n",line);
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
