#include "hash.h"
#include <stdlib.h>

#define TAM_DEFECTO 100

typedef struct celda {
	const char* clave;
	void* dato;
	int estado;
} celda_t;

typedef struct hash {
    size_t tam;
    struct celda_t** tabla;
} lista_t;

/*
typedef struct hash_iter {


} hash_iter_t;
*/

/* *****************************************************************
 *                    PRIMITIVAS DE LA CELDA
 * *****************************************************************/

celda_t* crear_celda(const char* clave, void* dato){ 
	celda_t* celda = malloc(sizeof(celda_t)); 
	if (!celda) return NULL; 
	celda->clave = strdup(clave);
	celda->dato = dato;
	celda->estado = 1; 
}

void destruir_celda(celda_t* celda){
	free(celda);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->tabla = malloc(sizeof(celda_t*)*TAM_DEFECTO);
	if (hash->tabla){
		free(hash);
		return NULL;
	}
	hash->tam = TAM_DEFECTO;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	celda_t* celda = crear_celda(clave, dato);
	if (!celda)
		return false;
	
}
