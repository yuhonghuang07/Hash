#include "hash.h"
#include <stdlib.h>
#include <string.h>

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
	campo->estado = LIBRE;
	return campo; 
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
	return hash;
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

typedef struct hash_iter {
	hash_t* hash;
	int posicion;


} hash_iter_t;

hash_iter_t *hash_iter_crear(const hash_t *hash){
	if(!hash){
		return NULL;
	}
	hash_iter_t* iter=malloc(sizeof(hash_iter_t));
	if(!iter){
		return NULL;
	}
	iter->hash=(hash_t*)hash;
	iter->posicion=0;
	while(iter->hash->cantidad>iter->posicion){
		if(iter->hash->tabla[iter->posicion].estado==OCUPADO){
			break;
		}
		iter->posicion++;
	}

	return iter;
}
bool hash_iter_avanzar(hash_iter_t *iter){
	if(!iter){
		return false;
	}
	iter->posicion++;
	while(iter->hash->cantidad>iter->posicion){
		if(iter->hash->tabla[iter->posicion].estado==OCUPADO){
			return true;
		}
		iter->posicion++;
	}
	return false;
}
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(!iter){
		return NULL;
	}
	return(iter->hash->tabla[iter->posicion].clave);
}
bool hash_iter_al_final(const hash_iter_t *iter){
	if(!iter){
		return false;
	}
	return(iter->posicion==iter->hash->cantidad);

}
void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}
