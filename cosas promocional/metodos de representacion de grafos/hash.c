#include "lista.h"
#include "hash.h"
#include "hash_iterador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CANT_INICIAL 0
#define TAM_INICIAL 3
#define FACTOR_CARGA 0.75
#define ERROR -1
#define EXITO 0
#define ORIGEN 0
#define VACIO 0
/////////////////////////////////////////////////////////////////STRUCTS///////////////////////////////////////////////////////////////////
struct hash{
	size_t cantidad;
	size_t tamanio;
	lista_t** indice;
	hash_destruir_dato_t destructor;
};

struct hash_iter{
    hash_t* hash;
	lista_iterador_t** iterador_lista;
	size_t contador_ite;
	size_t posicion;
	size_t tamanio;
};

typedef struct cosa{
	char* clave;
	void* elemento;
}cosa_t;
////////////////////////////////////////////////////////////////AUXILIARES////////////////////////////////////////////////////////////////
char* str_dup_c99(const char* s){
  if(!s)
    return NULL;
  
  char* p = malloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}
/*
 * Crea una estructura con el elemento y la clave recibidas.
 */
cosa_t* crear_cosa(void* elemento, const char* clave){
	if(!clave) return NULL;
	
	cosa_t* aux = malloc(sizeof(cosa_t));
	if(!aux) return NULL;

	aux->clave = str_dup_c99(clave);
	aux->elemento = elemento;
	return aux;
}
/*
 * Recibe una "cosa" y le asigna una posicion determinada.
 */
size_t hashing(size_t tamanio, cosa_t* cosa){
	size_t posicion = strlen(cosa->clave);
	size_t suma = 0;
	for(size_t i = 0; i< posicion; i++){
		suma += (size_t)(cosa->clave[i]);
	}
	suma= (suma*2)+1;
	posicion = suma%(tamanio);
	return posicion;
}
/*
 * Destruye la cosa, aplicado el destructor al elemento, ademas de liberar la memoria reservada para la clave y la cosa.
 */
void destruir_cosa(cosa_t* cosa, hash_destruir_dato_t destructor){
	if(!cosa) return;

	if(destructor){
		destructor(cosa->elemento);	
	}
	free(cosa->clave);
	free(cosa);
}
////////////////////////////////////////////////////////////////HASH CREAR/////////////////////////////////////////////////////////////////
hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){
	size_t tamanio;
	if(capacidad > TAM_INICIAL){
		tamanio = capacidad;
	}else{
		tamanio = TAM_INICIAL;
	}
	
	hash_t* aux = malloc(sizeof(hash_t));
	if(!aux) return NULL;

	lista_t** lista_aux = malloc(sizeof(lista_t*)*tamanio);
	if(!lista_aux){
		free(aux);
		return NULL;
	}
	aux->indice = lista_aux;
	for(size_t i = 0; i < tamanio; i++){
		aux->indice[i] = lista_crear();
	}
	aux->cantidad = CANT_INICIAL;
	aux->destructor = destruir_elemento;
	aux->tamanio = tamanio;
	return aux;
}
//////////////////////////////////////////////////////////////////HASH INSERTAR////////////////////////////////////////////////////////////////////
/*
 * Determina el factor de carga del hash.
 */
float factor_carga(hash_t* hash){
	float factor = 0;
	float cantidad = (float)hash->cantidad;
	float tamnio = (float)hash->tamanio;
	factor = (float)(cantidad/tamnio);
	return factor;
}
/*
 * 
 */
bool es_primo_r(size_t n, size_t x){
	if(x == 1)
		return true;

	if(n % x == 0)
		return false;

	return es_primo_r(n, x-1);
}
/*
 * 
 */
bool es_primo(size_t n){
	if(n < 2) return false;

	return es_primo_r(n, n-1);
}
/*
 * Duplica el tamanio del hash actual y apartir de ahi busca un primo,
 * cuando lo encuentra lo devuelve.
 */
size_t nuevo_tamanio_hash(size_t tamanio_viejo){
	size_t nuevo_tamanio = tamanio_viejo*2;
	bool encontrado = false;
	if(es_primo(nuevo_tamanio)){
		encontrado = true;
	}else{
		while(!encontrado){
			nuevo_tamanio++;
			if(es_primo(nuevo_tamanio)){
				encontrado = true;
			}
		}
	}
	return nuevo_tamanio;
}
/*
 * Mueve los elementos del indice del hash a uno nuevo y luego lo remplaza
 * dentro de la estructura del hash.
 */
int redimensionar_vector_listas(hash_t* hash){
	size_t nuevo_tamanio = nuevo_tamanio_hash(hash->tamanio);
	size_t cantidad = hash->cantidad;
	lista_t** nuevo_indice = malloc(sizeof(lista_t*)*nuevo_tamanio);
	if(!nuevo_indice) return ERROR;

	for(size_t i = 0; i < nuevo_tamanio; i++){
		nuevo_indice[i] = lista_crear();
	}
	for(size_t j = 0; j < hash->tamanio; j++){
		if(hash->indice[j]){
			while(!lista_vacia(hash->indice[j])){ 
				cosa_t* aux = (cosa_t*)lista_elemento_en_posicion(hash->indice[j], ORIGEN);
				lista_borrar_de_posicion(hash->indice[j], ORIGEN);
				size_t posicion = hashing(nuevo_tamanio, aux);
				lista_insertar(nuevo_indice[posicion], aux);
			}
			lista_destruir(hash->indice[j]);
		}
	}
	free(hash->indice);
	hash->indice = nuevo_indice;
	hash->tamanio = nuevo_tamanio;
	hash->cantidad = cantidad;
	return EXITO;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if(!hash || !clave) return ERROR;
	
	int estado = ERROR;
	size_t posicion;
	size_t cantidad = 0;
	cosa_t* cosa;
	cosa_t* cosa_del_hash = NULL;
	float carga = factor_carga(hash);
	bool agregue_a_existente = false;

	if(carga >= FACTOR_CARGA){
		estado = redimensionar_vector_listas(hash);
		if(estado == ERROR){
			return estado;
		}
	}
	cosa = crear_cosa(elemento, clave);
	if(cosa){
		posicion = hashing(hash->tamanio, cosa);
		cantidad = lista_elementos(hash->indice[posicion]);
		for(size_t i = 0; i < cantidad; i++){
			cosa_del_hash = (cosa_t*)lista_elemento_en_posicion(hash->indice[posicion], i);
			if(cosa_del_hash){
				if(strcmp(cosa->clave, cosa_del_hash->clave)== 0){
					destruir_cosa(cosa_del_hash, hash->destructor);
					lista_borrar_de_posicion(hash->indice[posicion], i);
					lista_insertar_en_posicion(hash->indice[posicion], cosa, i);
					agregue_a_existente = true;
				}
			}
		}
		if(!agregue_a_existente){
			lista_insertar(hash->indice[posicion], cosa);
			hash->cantidad++;
		}
		estado = EXITO;
	}else{
		estado = ERROR;
	}
	return estado;
}
//////////////////////////////////////////////////////////////////HASH QUITAR//////////////////////////////////////////////////////////////////
int hash_quitar(hash_t* hash, const char* clave){
	if(!hash || !clave) return ERROR;

	size_t posicion;
	size_t cantidad_elementos;
	cosa_t* cosa_del_hash;
	cosa_t aux = {"", NULL};
	int estado = ERROR;

	aux.clave = str_dup_c99(clave);
	posicion = hashing(hash->tamanio, &aux);
	cantidad_elementos = lista_elementos(hash->indice[posicion]);
	for(size_t i = 0; i < cantidad_elementos; i++){
		cosa_del_hash = (cosa_t*)lista_elemento_en_posicion(hash->indice[posicion], i);
		if(cosa_del_hash){
			if(strcmp(cosa_del_hash->clave, aux.clave) == 0){
				lista_borrar_de_posicion(hash->indice[posicion], i);
				destruir_cosa(cosa_del_hash, hash->destructor);
				hash->cantidad--;
				estado = EXITO;
			}
		}
	}
	free(aux.clave);
	return estado;
}
/////////////////////////////////////////////////////////////////HASH CONTIENE/////////////////////////////////////////////////////////////////
bool hash_contiene(hash_t* hash, const char* clave){
	size_t posicion;
	size_t cantidad_elementos;
	bool encontre = false;
	cosa_t* cosa_del_hash = NULL;
	cosa_t aux = {"", NULL};
	
	if(!hash || !clave) return encontre;

	aux.clave = str_dup_c99(clave);
	posicion = hashing(hash->tamanio, &aux);
	cantidad_elementos = lista_elementos(hash->indice[posicion]);
	for(size_t i = 0; i < cantidad_elementos; i++){
		cosa_del_hash = (cosa_t*)lista_elemento_en_posicion(hash->indice[posicion], i);
		if(strcmp(cosa_del_hash->clave, aux.clave) == 0){
			encontre = true;
		}
	}
	free(aux.clave);
	return encontre;
}
/////////////////////////////////////////////////////////////////HASH OBTENER/////////////////////////////////////////////////////////////////
void* hash_obtener(hash_t* hash, const char* clave){
	if(!hash || !clave) return NULL;

	size_t posicion;
	size_t cantidad_elementos;
	cosa_t* cosa_del_hash = NULL;
	cosa_t aux = {"", NULL};

	aux.clave = str_dup_c99(clave);
	posicion = hashing(hash->tamanio, &aux);
	cantidad_elementos = lista_elementos(hash->indice[posicion]);
	for(size_t i = 0; i < cantidad_elementos; i++){
		cosa_del_hash = (cosa_t*)lista_elemento_en_posicion(hash->indice[posicion], i);
		if(strcmp(cosa_del_hash->clave, aux.clave) == 0){
			free(aux.clave);
			return cosa_del_hash->elemento;
		}
	}
	free(aux.clave);
	return NULL;
}
/////////////////////////////////////////////////////////////////HASH CANTIDAD/////////////////////////////////////////////////////////////////
size_t hash_cantidad(hash_t* hash){
	if(!hash) return 0;
	
	return hash->cantidad;
}
/////////////////////////////////////////////////////////////////HASH DESTRUIR/////////////////////////////////////////////////////////////////
void hash_destruir(hash_t* hash){
	if(!hash) return;
	
	size_t tamanio = hash->tamanio;
	cosa_t* aux = NULL;

	for(size_t i = 0; i < tamanio; i++){
		if(hash->indice[i]){
			printf("En el nivel:%zu hay %zu autos.\n", i, lista_elementos(hash->indice[i]));
			while(!lista_vacia(hash->indice[i])){
				aux = (cosa_t*)lista_elemento_en_posicion(hash->indice[i], ORIGEN);
				lista_borrar_de_posicion(hash->indice[i], ORIGEN);
				destruir_cosa(aux, hash->destructor);
			}
			lista_destruir(hash->indice[i]);
		}
	}
	free(hash->indice);
	free(hash);
}
/////////////////////////////////////////////////////////////////HASH CON CADA CLAVE/////////////////////////////////////////////////////////////////
size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if(!hash || !funcion) return 0;
	
	size_t tamanio = hash->tamanio;
	size_t cantidad = 0;
	size_t contador = 0;
	bool estado = false;
	cosa_t* cosa_en_hash = NULL;

	for(size_t i = 0; i < tamanio; i++){
		if(hash->indice[i]){
			cantidad = lista_elementos(hash->indice[i]);
			for(size_t j = 0; j < cantidad; j++){
				cosa_en_hash = (cosa_t*)lista_elemento_en_posicion(hash->indice[i], j);
				estado = funcion(hash, cosa_en_hash->clave, aux);
				contador++;
				if(estado) break;
			}
		}
		if(estado) break;
	}
	return contador;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
hash_iterador_t* hash_iterador_crear(hash_t* hash){
    if(!hash) return NULL;

    hash_iterador_t* iterador = malloc(sizeof(hash_iterador_t));
    if(!iterador) return NULL;

    iterador->hash = hash;
	lista_iterador_t** aux = malloc(sizeof(lista_iterador_t*)*(hash->tamanio));
	if(!aux){
		free(iterador);
		return NULL;
	}
	
	iterador->iterador_lista = aux;
	for(size_t i = 0; i< iterador->hash->tamanio; i++){
		if(iterador->hash->indice[i]){
			iterador->iterador_lista[i] = lista_iterador_crear(iterador->hash->indice[i]);
		}
	}
	iterador->posicion = 0;
	iterador->contador_ite = 0;
	iterador->tamanio = hash->tamanio;
	return iterador;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* hash_iterador_siguiente(hash_iterador_t* iterador){
	if(!iterador) return NULL;

	size_t posicion = iterador->posicion;
	bool encontre = false;
	if(hash_iterador_tiene_siguiente(iterador)){
		cosa_t* aux = (cosa_t*)lista_iterador_siguiente(iterador->iterador_lista[posicion]);
		if(aux){
			return aux->clave;
		}else{
			while(!encontre){
				posicion++;
				aux = (cosa_t*)lista_iterador_siguiente(iterador->iterador_lista[posicion]);
				if(aux){
					iterador->posicion = posicion;
					encontre = true;
					return aux->clave;
				}
			}
		}
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador) return false;

	size_t posicion = iterador->posicion;
	if(lista_iterador_tiene_siguiente(iterador->iterador_lista[posicion])){
		return true;
	}else{
		posicion++;
		while(posicion < iterador->tamanio){
			if(lista_iterador_tiene_siguiente(iterador->iterador_lista[posicion])){
				return true;
			}
			posicion++;
		}
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hash_iterador_destruir(hash_iterador_t* iterador){
	if(!iterador) return;

	for(size_t i = 0; i < iterador->tamanio; i++){
		lista_iterador_destruir(iterador->iterador_lista[i]);
	}
	free(iterador->iterador_lista);
	free(iterador);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////