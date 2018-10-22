#include "hash.h"
#include <stdlib.h>

#define LARGO_DEFECTO 100

typedef enum estado {LIBRE, OCUPADO, BORRADO} estado_t;

typedef struct hash_campo {
    char *clave;
    void *dato;
    estado_t estado; 

} hash_campo_t;

typedef struct hash {
    size_t cantidad;                  
    size_t largo;                     
    size_t carga;                     
    hash_campo_t *tabla; 
    hash_destruir_dato_t destruir_dato; 
} hash_t;

/*
typedef struct hash_iter {


} hash_iter_t;
*/

/* *****************************************************************
 *                    PRIMITIVAS DEL CAMPO
 * *****************************************************************/

hash_campo_t* crear_campo(const char* clave, void* dato){ 
	hash_campo_t* campo = malloc(sizeof(hash_campo_t)); 
	if (!campo) return NULL; 
	campo->clave = strdup(clave);
	campo->dato = dato;
	campo->estado = 1; 
}

void destruir_campo(hash_campo_t* campo){
	free(campo->clave);
	free(campo);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->largo = LARGO_DEFECTO;
	hash->tabla = malloc(sizeof(hash_campo_t*)*hash->largo);
	if (hash->tabla){
		free(hash);
		return NULL;
	}
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	hash_campo_t* campo = crear_campo(clave, dato);
	if (!campo)
		return false;

}

void *hash_borrar(hash_t *hash, const char *clave){

}

void *hash_obtener(const hash_t *hash, const char *clave){

}

bool hash_pertenece(const hash_t *hash, const char *clave){

}

size_t hash_cantidad(const hash_t *hash){

}

void hash_destruir(hash_t *hash){

}