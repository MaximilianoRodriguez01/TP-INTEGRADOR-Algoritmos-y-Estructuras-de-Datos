#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL)
		return NULL;
	abb_t *abb = calloc(1, sizeof(abb_t));
	if (abb == NULL)
		return NULL;
	abb->comparador = comparador;
	return abb;
}

nodo_abb_t *nodo_abb_insertar_(nodo_abb_t *nodo_raiz, void *elemento,
			       abb_comparador comparador,
			       bool *sin_error_al_insertar)
{
	if (comparador == NULL)
		return NULL;
	if (nodo_raiz == NULL) {
		nodo_abb_t *nuevo = calloc(1, sizeof(nodo_abb_t));
		if (nuevo == NULL) {
			*sin_error_al_insertar = false;
			return NULL;
		}
		nuevo->elemento = elemento;
		return nuevo;
	}
	if (comparador(elemento, nodo_raiz->elemento) > 0) {
		nodo_raiz->derecha = nodo_abb_insertar_(nodo_raiz->derecha,
							elemento, comparador,
							sin_error_al_insertar);
	} else {
		nodo_raiz->izquierda =
			nodo_abb_insertar_(nodo_raiz->izquierda, elemento,
					   comparador, sin_error_al_insertar);
	}
	return nodo_raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || arbol->comparador == NULL) {
		return NULL;
	}
	bool sin_error_al_insertar = true;
	arbol->nodo_raiz = nodo_abb_insertar_(arbol->nodo_raiz, elemento,
					      arbol->comparador,
					      &sin_error_al_insertar);
	if (sin_error_al_insertar == false)
		return NULL;
	arbol->tamanio++;
	return arbol;
}

void *nodo_recuperar_elemento_borrado(nodo_abb_t *nodo, void *elemento)
{
	void *elemento_viejo = nodo->elemento;
	nodo->elemento = elemento;

	return elemento_viejo;
}

nodo_abb_t *nodo_mas_grande(nodo_abb_t *raiz)
{
	if (raiz->derecha == NULL)
		return raiz;

	return nodo_mas_grande(raiz->derecha);
}

nodo_abb_t *nodo_abb_quitar_nodo_raiz(nodo_abb_t *raiz,
				      abb_comparador comparador);

nodo_abb_t *nodo_abb_quitar(nodo_abb_t *nodo_raiz, abb_comparador comparador,
			    void *elemento, void **elemento_quitado,
			    bool *quitado_exitoso)
{
	if (nodo_raiz == NULL) {
		if (quitado_exitoso != NULL)
			*quitado_exitoso = false;
		return NULL;
	}
	int comparacion = comparador(elemento, nodo_raiz->elemento);

	if (comparacion == 0) {
		if (elemento_quitado != NULL)
			*elemento_quitado = nodo_raiz->elemento;

		if (nodo_raiz->izquierda != NULL &&
		    nodo_raiz->derecha != NULL) {
			return nodo_abb_quitar_nodo_raiz(nodo_raiz, comparador);
		}
		nodo_abb_t *siguiente;
		if (nodo_raiz->izquierda)
			siguiente = nodo_raiz->izquierda;
		else
			siguiente = nodo_raiz->derecha;
		free(nodo_raiz);
		return siguiente;
	}
	if (comparacion < 0)
		nodo_raiz->izquierda = nodo_abb_quitar(nodo_raiz->izquierda,
						       comparador, elemento,
						       elemento_quitado,
						       quitado_exitoso);
	else
		nodo_raiz->derecha = nodo_abb_quitar(nodo_raiz->derecha,
						     comparador, elemento,
						     elemento_quitado,
						     quitado_exitoso);
	return nodo_raiz;
}

nodo_abb_t *nodo_abb_quitar_nodo_raiz(nodo_abb_t *raiz,
				      abb_comparador comparador)
{
	nodo_abb_t *predecesor = nodo_mas_grande(raiz->izquierda);
	nodo_recuperar_elemento_borrado(raiz, predecesor->elemento);
	raiz->izquierda = nodo_abb_quitar(raiz->izquierda, comparador,
					  predecesor->elemento, NULL, NULL);
	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL) {
		return NULL;
	}
	void *elemento_borrado = NULL;
	bool sin_error_borrado = true;
	arbol->nodo_raiz = nodo_abb_quitar(arbol->nodo_raiz, arbol->comparador,
					   elemento, &elemento_borrado,
					   &sin_error_borrado);
	if (sin_error_borrado == false)
		return NULL;
	arbol->tamanio--;
	return elemento_borrado;
}

void *nodo_abb_buscar_(nodo_abb_t *nodo_raiz, void *elemento,
		       abb_comparador comparador)
{
	if (nodo_raiz == NULL)
		return NULL;
	nodo_abb_t *nodo = nodo_raiz;
	int comparacion = 0;
	while (nodo != NULL) {
		comparacion = comparador(elemento, nodo->elemento);
		if (comparacion == 0)
			return nodo->elemento;
		if (comparacion > 0)
			nodo = nodo->derecha;
		if (comparacion < 0)
			nodo = nodo->izquierda;
	}
	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || elemento == NULL || abb_vacio(arbol))
		return NULL;
	return nodo_abb_buscar_(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
	if (arbol == NULL)
		return true;
	return arbol->tamanio == 0;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (arbol == NULL)
		return 0;
	return arbol->tamanio;
}

void nodo_abb_destruir_todo(nodo_abb_t *nodo_raiz, void (*destructor)(void *))
{
	if (nodo_raiz == NULL)
		return;
	nodo_abb_destruir_todo(nodo_raiz->derecha, destructor);
	nodo_abb_destruir_todo(nodo_raiz->izquierda, destructor);
	if (destructor != NULL)
		destructor(nodo_raiz->elemento);
	free(nodo_raiz);
}

void abb_destruir(abb_t *arbol)
{
	if (arbol == NULL)
		return;
	abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (arbol == NULL)
		return;
	nodo_abb_destruir_todo(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento_postorden(nodo_abb_t *nodo,
				       bool (*funcion)(void *, void *),
				       void *aux, bool *recorrer)
{
	if ((*recorrer) == false || nodo == NULL)
		return 0;
	size_t cantidad_llamadas = 0;
	cantidad_llamadas += abb_con_cada_elemento_postorden(
		nodo->izquierda, funcion, aux, recorrer);

	cantidad_llamadas += abb_con_cada_elemento_postorden(
		nodo->derecha, funcion, aux, recorrer);
	if (*recorrer) {
		*recorrer = funcion(nodo->elemento, aux);
		cantidad_llamadas++;
	}
	return cantidad_llamadas;
}

size_t abb_con_cada_elemento_inorden(nodo_abb_t *nodo,
				     bool (*funcion)(void *, void *), void *aux,
				     bool *recorrer)
{
	if ((*recorrer) == false || nodo == NULL)
		return 0;
	size_t cantidad_llamadas = 0;
	cantidad_llamadas += abb_con_cada_elemento_inorden(
		nodo->izquierda, funcion, aux, recorrer);
	if (*recorrer) {
		*recorrer = funcion(nodo->elemento, aux);
		cantidad_llamadas++;
	}
	cantidad_llamadas += abb_con_cada_elemento_inorden(
		nodo->derecha, funcion, aux, recorrer);
	return cantidad_llamadas;
}

size_t abb_con_cada_elemento_preorden(nodo_abb_t *nodo,
				      bool (*funcion)(void *, void *),
				      void *aux, bool *recorrer)
{
	if ((*recorrer) == false || nodo == NULL)
		return 0;

	size_t cantidad_llamadas = 0;

	if (*recorrer) {
		*recorrer = funcion(nodo->elemento, aux);
		cantidad_llamadas++;
	}

	cantidad_llamadas += abb_con_cada_elemento_preorden(
		nodo->izquierda, funcion, aux, recorrer);

	cantidad_llamadas += abb_con_cada_elemento_preorden(
		nodo->derecha, funcion, aux, recorrer);

	return cantidad_llamadas;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (arbol == NULL || funcion == false)
		return 0;

	bool recorrer = true;

	switch (recorrido) {
	case INORDEN:
		return abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion,
						     aux, &recorrer);
	case PREORDEN:
		return abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion,
						      aux, &recorrer);
	case POSTORDEN:
		return abb_con_cada_elemento_postorden(arbol->nodo_raiz,
						       funcion, aux, &recorrer);
	default:
		return 0;
	}
}

size_t abb_recorrer_postorden(nodo_abb_t *nodo, void **array,
			      size_t tamanio_array, size_t *posicion)
{
	if (nodo == NULL)
		return 0;

	size_t cantidad_recorridos = 0;

	cantidad_recorridos += abb_recorrer_postorden(nodo->izquierda, array,
						      tamanio_array, posicion);

	cantidad_recorridos += abb_recorrer_postorden(nodo->derecha, array,
						      tamanio_array, posicion);

	if (*posicion < tamanio_array) {
		array[*posicion] = nodo->elemento;
		cantidad_recorridos += 1;
		*posicion += 1;
	}

	return cantidad_recorridos;
}

size_t abb_recorrer_inorden(nodo_abb_t *nodo, void **array,
			    size_t tamanio_array, size_t *n)
{
	if (nodo == NULL)
		return 0;

	size_t cantidad_recorridos = 0;
	cantidad_recorridos +=
		abb_recorrer_inorden(nodo->izquierda, array, tamanio_array, n);
	if (*n < tamanio_array) {
		array[*n] = nodo->elemento;
		cantidad_recorridos += 1;
		*n += 1;
	}
	cantidad_recorridos +=
		abb_recorrer_inorden(nodo->derecha, array, tamanio_array, n);
	return cantidad_recorridos;
}

size_t abb_recorrer_preorden(nodo_abb_t *nodo, void **array,
			     size_t tamanio_array, size_t *n)
{
	if (nodo == NULL)
		return 0;

	size_t cantidad_recorridos = 0;
	if (*n < tamanio_array) {
		array[*n] = nodo->elemento;
		cantidad_recorridos += 1;
		*n += 1;
	}
	cantidad_recorridos +=
		abb_recorrer_preorden(nodo->izquierda, array, tamanio_array, n);
	cantidad_recorridos +=
		abb_recorrer_preorden(nodo->derecha, array, tamanio_array, n);
	return cantidad_recorridos;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (arbol == NULL || array == NULL || tamanio_array == 0)
		return 0;

	size_t n = 0;

	switch (recorrido) {
	case INORDEN:
		return abb_recorrer_inorden(arbol->nodo_raiz, array,
					    tamanio_array, &n);
	case PREORDEN:
		return abb_recorrer_preorden(arbol->nodo_raiz, array,
					     tamanio_array, &n);
	case POSTORDEN:
		return abb_recorrer_postorden(arbol->nodo_raiz, array,
					      tamanio_array, &n);
	default:
		return 0;
	}
}