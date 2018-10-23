#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define LARGO_DEFECTO 101

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

typedef struct hash_iter {
	hash_t* hash;
	int posicion;


} hash_iter_t;

int hashing(char* cadena){
    int b    = 378551;
    int a    = 63689;
	int hash = 0;

    for(int i = 0; i < strlen(cadena); i++){
          hash = hash * a + cadena[i];
          a    = a * b;
    }
    return (hash & 0x7FFFFFFF);
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
	for (int i=0; i<hash->largo; i++){
		hash->tabla[i].estado=LIBRE;
	}

	hash->cantidad = 0;
	hash->carga = 0;
	hash->destruir_dato = destruir_dato;
	return hash;
}
bool hash_redimensionar(hash_t hash,size_t capacidad){
	void* hash_nuevo= realloc(hash->tabla, capacidad * sizeof(void*));
	if(datos_nuevo==NULL)
		return false;
	pila->datos=datos_nuevo;
	return true;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

	/* PONER REDIMENSIONAR */

	int posicion = hashing((char*)clave)%hash->largo;
	if (hash->tabla[posicion].estado!=LIBRE){

		while (hash->tabla[posicion].estado!=LIBRE){
			if (hash->tabla[posicion].clave==clave){
				hash->tabla[posicion].dato=dato;
				return true;
			}
			posicion++;
			if (posicion==hash->largo){
				posicion=0;
			}
		}
	}
	hash->tabla[posicion].clave=strdup(clave);
	hash->tabla[posicion].dato=dato;
	hash->tabla[posicion].estado=OCUPADO;
	hash->cantidad++;
	hash->carga++;
	return true;
}

int obtener_posicion(const hash_t *hash, const char *clave);

void *hash_borrar(hash_t *hash, const char *clave){
	int posicion = obtener_posicion(hash,clave);
	if (posicion==-1)
		return NULL;
	void* dato = hash->tabla[posicion].dato;
	hash->destruir_dato(hash->tabla[posicion].dato);
	free(hash->tabla[posicion].clave);
	hash->tabla[posicion].estado=BORRADO;
	hash->cantidad--;
	return dato;

}

int obtener_posicion(const hash_t *hash, const char *clave){
	int posicion = hashing((char*)clave)%hash->largo;
	if (hash->tabla[posicion].estado==LIBRE)
		return -1;
	bool pertenece = false;
	while (hash->tabla[posicion].estado!=LIBRE ){
		if (hash->tabla[posicion].estado==OCUPADO && hash->tabla[posicion].clave==clave){
			pertenece = true;
			break;
		}
		posicion++;
		if (posicion==hash->largo){
			posicion=0;
		}
	if (!pertenece)
		return -1;
	}
	return posicion;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	int posicion = obtener_posicion(hash, clave);
	if (posicion==-1)
		return NULL;
	return hash->tabla[posicion].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	return(obtener_posicion(hash,clave)!=-1);
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;

}

void hash_destruir(hash_t *hash){
	for (int posicion=0; posicion<hash->largo; posicion++){
		if (hash->tabla[posicion].estado==OCUPADO){
			hash->destruir_dato(hash->tabla[posicion].dato);
			free(hash->tabla[posicion].clave);
		}
	}
	free(hash->tabla);
	free(hash);
}



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
	while(iter->hash->largo>iter->posicion){
		if(iter->hash->tabla[iter->posicion].estado==OCUPADO){
			break;
		}
		iter->posicion++;
	}

	return iter;
}
bool hash_iter_avanzar(hash_iter_t *iter){
	if(hash_iter_al_final(iter)){
		return false;
	}
	iter->posicion++;
	while(iter->hash->largo>iter->posicion){
		if(iter->hash->tabla[iter->posicion].estado==OCUPADO){
			return true;
		}
		iter->posicion++;
	}
	return false;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	return(iter->hash->tabla[iter->posicion].clave);
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return(iter->posicion==iter->hash->largo);
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}

