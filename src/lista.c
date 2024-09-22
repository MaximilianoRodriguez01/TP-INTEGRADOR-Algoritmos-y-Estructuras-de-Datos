#include "lista.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	size_t largo_lista;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *anterior;
	nodo_t *actual;
};

bool lista_iterador_siguiente_no_es_null(lista_iterador_t *iterador);

nodo_t *crear_nodo(nodo_t *nodo_siguiente, void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) {
		return NULL;
	}
	nodo->elemento = elemento;
	nodo->siguiente = nodo_siguiente;

	return nodo;
}

lista_t *lista_crear()
{
	lista_t *lista_auxiliar = malloc(sizeof(lista_t));
	if (lista_auxiliar == NULL) {
		printf("Error: No hay suficiente memoria para crear la lista");
		return NULL;
	}
	lista_auxiliar->nodo_inicio = NULL;
	lista_auxiliar->nodo_final = NULL;
	lista_auxiliar->largo_lista = 0;

	return lista_auxiliar;
}
lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL)
		return NULL;

	nodo_t *nodo_auxiliar = crear_nodo(NULL, elemento);
	if (nodo_auxiliar == NULL)
		return NULL;

	if (lista_vacia(lista))
		lista->nodo_inicio = nodo_auxiliar;
	else
		lista->nodo_final->siguiente = nodo_auxiliar;

	lista->nodo_final = nodo_auxiliar;
	lista->largo_lista++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL)
		return NULL;

	nodo_t *nodo_auxiliar = crear_nodo(NULL, elemento);

	if (nodo_auxiliar == NULL)
		return NULL;

	if (posicion >= lista->largo_lista) {
		if ((lista_vacia(lista))) {
			lista->nodo_inicio = nodo_auxiliar;
			lista->nodo_final = nodo_auxiliar;
			lista->largo_lista++;
			return lista;
		}
		lista->nodo_final->siguiente = nodo_auxiliar;
		lista->nodo_final = nodo_auxiliar;
		lista->largo_lista++;
		return lista;
	}

	if (lista->nodo_inicio == NULL || posicion == 0) {
		nodo_auxiliar->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo_auxiliar;
		lista->largo_lista++;
		return lista;
	}

	lista_iterador_t *it_lista = lista_iterador_crear(lista);
	if (it_lista == NULL) {
		lista_destruir(lista);
		return NULL;
	}

	bool seguir = true;

	for (size_t i = 0; lista_iterador_siguiente_no_es_null(it_lista) &&
			   i < posicion - 1 && seguir;
	     i++) {
		seguir = lista_iterador_avanzar(it_lista);
	}
	nodo_auxiliar->siguiente = it_lista->actual->siguiente;
	it_lista->actual->siguiente = nodo_auxiliar;
	lista->largo_lista++;
	lista_iterador_destruir(it_lista);
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (lista == NULL || lista_vacia(lista))
		return NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;
	void *elemento_extraido = NULL;

	if (lista_tamanio(lista) == 1) {
		elemento_extraido = nodo_actual->elemento;
		free(nodo_actual);
		lista->nodo_final = NULL;
		lista->nodo_inicio = NULL;

	} else {
		for (size_t i = 2; i < lista_tamanio(lista); i++)
			nodo_actual = nodo_actual->siguiente;

		elemento_extraido = nodo_actual->siguiente->elemento;

		free(nodo_actual->siguiente);
		lista->nodo_final = nodo_actual;
		nodo_actual->siguiente = NULL;
	}
	lista->largo_lista--;
	return elemento_extraido;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL)
		return NULL;

	if (lista_vacia(lista))
		return NULL;

	if (posicion >= lista->largo_lista - 1) {
		return lista_quitar(lista);
	}

	nodo_t *nodo = NULL;
	void *elemento_extraido = NULL;

	if (posicion == 0) {
		nodo = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
		if (lista->nodo_inicio == NULL)
			lista->nodo_final = NULL;

	} else {
		nodo_t *nodo_anterior = lista->nodo_inicio;
		for (size_t i = 0; i < posicion - 1; i++)
			nodo_anterior = nodo_anterior->siguiente;

		nodo = nodo_anterior->siguiente;
		nodo_anterior->siguiente = nodo->siguiente;
		if (nodo_anterior->siguiente == NULL)
			lista->nodo_final = nodo_anterior;
	}
	elemento_extraido = nodo->elemento;
	free(nodo);
	lista->largo_lista--;

	return elemento_extraido;
}
void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL || lista_vacia(lista) ||
	    lista_tamanio(lista) < posicion + 1)
		return NULL;

	nodo_t *nodo_actual = lista->nodo_inicio;

	if (posicion == 0)
		return nodo_actual->elemento;

	for (size_t i = 0; i < posicion; i++)
		nodo_actual = nodo_actual->siguiente;

	return (nodo_actual->elemento);
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (lista == NULL)
		return NULL;
	if (lista_vacia(lista) || comparador == NULL)
		return NULL;

	lista_iterador_t *it_lista = lista_iterador_crear(lista);
	if (it_lista == NULL) {
		printf("Error al crear el iterador externo");
		return NULL;
	}

	void *elemento_auxiliar = NULL;
	while (lista_iterador_tiene_siguiente(it_lista)) {
		if (comparador(it_lista->actual->elemento, contexto) == 0) {
			elemento_auxiliar = it_lista->actual->elemento;
			lista_iterador_destruir(it_lista);
			return elemento_auxiliar;
		}
		if (!lista_iterador_avanzar(it_lista)) {
			lista_iterador_destruir(it_lista);
			return NULL;
		}
	}
	lista_iterador_destruir(it_lista);
	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (lista_vacia(lista))
		return NULL;
	if (lista->nodo_inicio != NULL)
		return lista->nodo_inicio->elemento;
	return NULL;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista_vacia(lista))
		return NULL;
	if (lista->nodo_final != NULL)
		return lista->nodo_final->elemento;
	return NULL;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL)
		return true;
	return lista->largo_lista == 0;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista_vacia(lista))
		return 0;
	return lista->largo_lista;
}

void lista_destruir(lista_t *lista)
{
	lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista == NULL)
		return;
	while (lista->nodo_inicio != NULL) {
		nodo_t *nodo_auxiliar = lista->nodo_inicio;
		if (funcion != NULL)
			funcion(nodo_auxiliar->elemento);
		lista->nodo_inicio = nodo_auxiliar->siguiente;
		free(nodo_auxiliar);
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;
	lista_iterador_t *it_lista = malloc(sizeof(lista_iterador_t));

	if (it_lista == NULL) {
		printf("Error al crear iterador externo de lista");
		return NULL;
	}
	it_lista->lista = lista;
	it_lista->actual = it_lista->lista->nodo_inicio;
	it_lista->anterior = NULL;

	return it_lista;
}

bool lista_iterador_siguiente_no_es_null(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return false;
	if (iterador->actual->siguiente != NULL)
		return true;
	return false;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return false;
	return iterador->actual;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return false;
	if (iterador->actual == NULL)
		return false;
	iterador->anterior = iterador->actual;
	return (iterador->actual = iterador->actual->siguiente);
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return NULL;
	if (iterador->actual == NULL)
		return NULL;
	return iterador->actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return;
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (lista == NULL)
		return 0;
	if (lista_vacia(lista))
		return 0;
	if (funcion == NULL)
		return 0;
	nodo_t *nodo_auxiliar = lista->nodo_inicio;
	size_t contador = 0;
	bool seguir_iterando = true;
	while (seguir_iterando && contador < lista_tamanio(lista)) {
		seguir_iterando = funcion(nodo_auxiliar->elemento, contexto);
		nodo_auxiliar = nodo_auxiliar->siguiente;
		contador++;
	}
	return contador;
}
