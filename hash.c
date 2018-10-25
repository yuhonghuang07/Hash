#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

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
	size_t posicion;
} hash_iter_t;

int hashing_(const char* cadena){
	int b    = 378551;
	int a    = 63689;
	int hash = 0;

	for(int i = 0; i < strlen(cadena); i++){
		  hash = hash * a + cadena[i];
		  a    = a * b;
	}
	return (hash & 0x7FFFFFFF);
}
/*BKDRHash*/
int hashing(const char *str){
	int seed = 13131313; // 31 131 1313 13131 131313 etc..
	int hash = 0;

	while (*str){
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}

size_t _raiz(size_t ini,size_t fin, size_t numero){
	if(ini==fin){
		return ini;
	}
	size_t medio=(ini+fin)/2;
	if(medio*medio==numero){
		return medio;
	}
	if(medio*medio>numero){
		return _raiz(ini,medio-1,numero);
	}
	return _raiz(medio,fin-1,numero);
}
size_t raiz(size_t numero){
	if(numero<0){
		return -1;
	}
	if(numero<4){
		return 1;
	}
	size_t fin =numero;
	size_t ini=0;
	return _raiz(ini,fin,numero);
}
size_t obtener_nuevo_largo(size_t primo){
	size_t resultado=raiz(primo);
	for(size_t i=primo;i;i++){
		for(size_t j=2;j<=resultado+1;j++){
			if(i%j==0){
				break;
			}
			if(j==resultado+1){
				return i;

			}
		}
	}
	return 0;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* _hash_crear(hash_destruir_dato_t destruir_dato, size_t largo);

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	return _hash_crear(destruir_dato, LARGO_DEFECTO);
}

hash_t* _hash_crear(hash_destruir_dato_t destruir_dato, size_t largo){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->largo = largo;
	hash->tabla = malloc(sizeof(hash_campo_t)*hash->largo);
	if (!hash->tabla){
		free(hash);
		return NULL;
	}
	for (size_t posicion=0; posicion<hash->largo; posicion++){
		hash->tabla[posicion].estado=LIBRE;
	}
	hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->carga = 0;
	return hash;
}

hash_t* hash_redimensionar(hash_t* hash, size_t largo_nuevo){
	hash_campo_t* tabla_nueva=malloc(sizeof(hash_campo_t)*largo_nuevo);
	if (!tabla_nueva) return NULL;
	for(int i =0;i<largo_nuevo;i++){
		tabla_nueva[i].estado=LIBRE;
		tabla_nueva[i].clave=NULL;
		tabla_nueva[i].dato=NULL;
	}
	for (size_t posicion=0; posicion<hash->largo; posicion++){
		if (hash->tabla[posicion].estado==OCUPADO){
			size_t posicion_nueva = hashing(hash->tabla[posicion].clave)%largo_nuevo;
			if(tabla_nueva[posicion_nueva].estado!=LIBRE){
				posicion_nueva= hashing_(hash->tabla[posicion].clave)%largo_nuevo;
			}
			while (tabla_nueva[posicion_nueva].estado!=LIBRE){
				posicion_nueva++;
				if (posicion_nueva>=largo_nuevo){
					posicion_nueva=0;
				}
			}
			tabla_nueva[posicion_nueva].estado=OCUPADO;
			tabla_nueva[posicion_nueva].dato=(hash->tabla[posicion].dato);
			tabla_nueva[posicion_nueva].clave=hash->tabla[posicion].clave;

		}
	}

	free(hash->tabla);
	hash->largo=largo_nuevo;
	hash->tabla=tabla_nueva;
	hash->carga=hash->cantidad;
	return hash;
}

size_t obtener_posicion(const hash_t *hash, const char *clave);

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if (hash->carga*100/hash->largo > 75){
		size_t largo=obtener_nuevo_largo(hash->largo*2);
		hash = hash_redimensionar(hash, largo);
	}
	size_t posicion = obtener_posicion(hash, clave);
	if (hash->tabla[posicion].estado==OCUPADO){
		if (hash->destruir_dato)
			hash->destruir_dato(hash->tabla[posicion].dato);
		hash->tabla[posicion].dato=dato;
		return true;
	}
	hash->tabla[posicion].clave=strdup(clave);
	hash->tabla[posicion].dato=dato;
	hash->tabla[posicion].estado=OCUPADO;
	hash->cantidad++;
	hash->carga++;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	size_t posicion = obtener_posicion(hash,clave);
	if (hash->tabla[posicion].estado==LIBRE)
		return NULL;
	void* dato = hash->tabla[posicion].dato;
	free(hash->tabla[posicion].clave);
	hash->tabla[posicion].estado=BORRADO;
	hash->cantidad--;
	if(hash->largo>LARGO_DEFECTO){
		if(hash->carga*100/hash->largo < 35){
			size_t largo=obtener_nuevo_largo(hash->largo/2);			
			hash = hash_redimensionar(hash, largo);
		}

	}
	return dato;

}

size_t obtener_posicion(const hash_t *hash, const char *clave){
	size_t posicion = hashing(clave)%hash->largo;
	if(hash->tabla[posicion].estado!=LIBRE){
		posicion=hashing_(clave)%hash->largo;
	}
	while (hash->tabla[posicion].estado!=LIBRE){
		if (hash->tabla[posicion].estado==OCUPADO && strcmp(hash->tabla[posicion].clave,clave)==0) //meter esta condicion al while
			break;
		posicion++;
		if (posicion>=hash->largo){
			posicion=0;
		}
	}
	return posicion;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t posicion = obtener_posicion(hash, clave);
	if (hash->tabla[posicion].estado==LIBRE)
		return NULL;
	return hash->tabla[posicion].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t posicion = obtener_posicion(hash, clave);
	return hash->tabla[posicion].estado==OCUPADO;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){
	for (size_t posicion=0; posicion<hash->largo; posicion++){
		if (hash->tabla[posicion].estado==OCUPADO){
			free(hash->tabla[posicion].clave);
			if (hash->destruir_dato)
				hash->destruir_dato(hash->tabla[posicion].dato);
		}
	}
	free(hash->tabla);
	free(hash);
}



hash_iter_t* hash_iter_crear(const hash_t *hash){
	if (!hash) return NULL;
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->hash=(hash_t*)hash;
	iter->posicion=-1;
	hash_iter_avanzar(iter);
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
	if (hash_iter_al_final(iter)){
		return NULL;
	}
	return (iter->hash->tabla[iter->posicion].clave);
}



bool hash_iter_al_final(const hash_iter_t *iter){
	return(iter->posicion==iter->hash->largo);
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}

