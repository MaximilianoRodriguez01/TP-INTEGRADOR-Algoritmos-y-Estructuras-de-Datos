#include "tp.h"
#include "abb.h"
#include "lista.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define LEER_POKEMON "%19[^,],%i,%i,%i\n"
#define CANTIDAD_PARAMETROS 3

#define MAX_TAMANIO_NOMBRE_POKEMON 20
#define MAX_DIGITOS_TIEMPO 3

#define ERROR -1
#define ERROR_CHAR '\0'

typedef const struct pokemon_info pokemon_t;

struct tp {
	abb_t *pokemones;
	pokemon_t *pokemon_jugador[2];
	lista_t *obstaculos_pista[2];
};

static char *string_to_upper(const char *string)
{
	if (!string)
		return NULL;

	char *upper_string = calloc(strlen(string) + 1, sizeof(char));
	if (!upper_string)
		return NULL;

	for (size_t i = 0; i < strlen(string); i++)
		upper_string[i] = (char)toupper(string[i]);

	upper_string[strlen(string)] = '\0';

	return upper_string;
}

static int negatives_to_zero(int numero)
{
	return numero < 0 ? 0 : numero;
}

static int comparador_alfabetico(void *elemento1, void *elemento2)
{
	pokemon_t *pokemon1 = (pokemon_t *)elemento1;
	pokemon_t *pokemon2 = (pokemon_t *)elemento2;

	return strcmp(pokemon1->nombre, pokemon2->nombre);
}

char *mi_strdup_to_upper(const char *string)
{
	if (!string)
		return NULL;

	char *string_upper = string_to_upper(string);
	if (!string_upper)
		return NULL;

	char *copia = calloc(1, strlen(string) + 1);
	if (!copia)
		return NULL;

	strcpy(copia, string_upper);
	free(string_upper);

	return copia;
}

void pokemon_info_destruir(void *pokemon)
{
	pokemon_t *pokemon_nuevo = (pokemon_t *)pokemon;
	if (!pokemon_nuevo)
		return;

	free((void *)pokemon_nuevo->nombre);
	free((void *)pokemon_nuevo);
}

static pokemon_t *pokemon_info_crear(char *nombre, int fuerza, int destreza,
				     int inteligencia)
{
	if (nombre == NULL)
		return NULL;

	struct pokemon_info *nuevo_pokemon = calloc(1, sizeof(pokemon_t));
	if (!nuevo_pokemon)
		return NULL;

	nuevo_pokemon->nombre = mi_strdup_to_upper(nombre);
	if (!nuevo_pokemon->nombre) {
		free(nuevo_pokemon);
		return NULL;
	}

	nuevo_pokemon->fuerza = fuerza;
	nuevo_pokemon->destreza = destreza;
	nuevo_pokemon->inteligencia = inteligencia;

	return (pokemon_t *)nuevo_pokemon;
}

abb_t *pokemon_info_leer_lineas(FILE *archivo, abb_t *abb_pokemones,
				char *nombre, int fuerza, int destreza,
				int inteligencia)
{
	int valor = 0;

	while ((valor = fscanf(archivo, LEER_POKEMON, nombre, &fuerza,
			       &destreza, &inteligencia)) != EOF) {
		if (valor != CANTIDAD_PARAMETROS + 1) {
			abb_destruir_todo(abb_pokemones, pokemon_info_destruir);
			return NULL;
		}

		pokemon_t *nuevo_pokemon =
			pokemon_info_crear(nombre, negatives_to_zero(fuerza),
					   negatives_to_zero(destreza),
					   negatives_to_zero(inteligencia));
		if (!nuevo_pokemon) {
			abb_destruir_todo(abb_pokemones, free);
			return NULL;
		}

		if (!abb_insertar(abb_pokemones, (void *)nuevo_pokemon)) {
			abb_destruir_todo(abb_pokemones, free);
			return NULL;
		}
	}

	return abb_pokemones;
}

abb_t *pokemon_info_leer(const char *nombre_archivo)
{
	FILE *archivo = fopen(nombre_archivo, "r");
	if (!archivo)
		return NULL;

	abb_t *abb_pokemones = abb_crear(comparador_alfabetico);
	if (!abb_pokemones) {
		fclose(archivo);
		return NULL;
	}

	char *nombre = calloc(1, MAX_TAMANIO_NOMBRE_POKEMON);
	if (!nombre) {
		abb_destruir(abb_pokemones);
		fclose(archivo);
		return NULL;
	}

	int fuerza = 0, destreza = 0, inteligencia = 0;

	if (!pokemon_info_leer_lineas(archivo, abb_pokemones, nombre, fuerza,
				      destreza, inteligencia)) {
		free(nombre);
		fclose(archivo);
		return NULL;
	}

	free(nombre);
	fclose(archivo);

	return abb_pokemones;
}

TP *tp_crear(const char *nombre_archivo)
{
	if (!nombre_archivo)
		return NULL;

	TP *nuevo_tp = calloc(1, sizeof(TP));
	if (!nuevo_tp)
		return NULL;

	nuevo_tp->pokemones = pokemon_info_leer(nombre_archivo);
	if (!nuevo_tp->pokemones) {
		free(nuevo_tp);
		return NULL;
	}

	nuevo_tp->obstaculos_pista[0] = lista_crear();
	if (nuevo_tp->obstaculos_pista[0] == NULL) {
		abb_destruir_todo(nuevo_tp->pokemones, pokemon_info_destruir);
		free(nuevo_tp);
		return NULL;
	}

	nuevo_tp->obstaculos_pista[1] = lista_crear();
	if (!nuevo_tp->obstaculos_pista[1]) {
		lista_destruir(nuevo_tp->obstaculos_pista[0]);
		abb_destruir_todo(nuevo_tp->pokemones, pokemon_info_destruir);
		free(nuevo_tp);
		return NULL;
	}

	return nuevo_tp;
}

int tp_cantidad_pokemon(TP *tp)
{
	return !tp ? 0 : (int)abb_tamanio(tp->pokemones);
}

const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre)
{
	if (!tp || !nombre)
		return NULL;

	pokemon_t *nuevo_pokemon = pokemon_info_crear((char *)nombre, 0, 0, 0);
	if (!nuevo_pokemon)
		return NULL;

	pokemon_t *pokemon_encontrado =
		(pokemon_t *)abb_buscar(tp->pokemones, (void *)nuevo_pokemon);

	free((void *)nuevo_pokemon->nombre);
	free((void *)nuevo_pokemon);

	return pokemon_encontrado;
}

char *tp_nombres_disponibles(TP *tp)
{
	if (!tp || abb_vacio(tp->pokemones))
		return NULL;

	pokemon_t **vector_pokemones =
		calloc(abb_tamanio(tp->pokemones), sizeof(pokemon_t *));
	if (!vector_pokemones)
		return NULL;

	abb_recorrer(tp->pokemones, INORDEN, (void **)vector_pokemones,
		     abb_tamanio(tp->pokemones));

	char *string =
		calloc(abb_tamanio(tp->pokemones) * MAX_TAMANIO_NOMBRE_POKEMON,
		       sizeof(char));
	if (!string) {
		free(vector_pokemones);
		return NULL;
	}

	for (size_t i = 0; i < abb_tamanio(tp->pokemones); i++) {
		strcat(string, vector_pokemones[i]->nombre);
		if (i < (abb_tamanio(tp->pokemones) - 1))
			strcat(string, ",");
	}

	free(vector_pokemones);

	return string;
}

bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre)
{
	if (!tp || !nombre || jugador < JUGADOR_1 || jugador > JUGADOR_2)
		return false;

	pokemon_t *pokemon_seleccionado = tp_buscar_pokemon(tp, nombre);
	if (!pokemon_seleccionado)
		return false;

	switch (jugador) {
	case JUGADOR_1:
		if (tp->pokemon_jugador[1] == pokemon_seleccionado)
			return false;

		tp->pokemon_jugador[0] = pokemon_seleccionado;
		return true;

	case JUGADOR_2:
		if (tp->pokemon_jugador[0] == pokemon_seleccionado)
			return false;

		tp->pokemon_jugador[1] = pokemon_seleccionado;
		return true;

	default:
		return false;
	}
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp,
						   enum TP_JUGADOR jugador)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2)
		return NULL;

	return tp->pokemon_jugador[jugador];
}

char obstaculo_identificar(enum TP_OBSTACULO obstaculo)
{
	if (obstaculo < OBSTACULO_FUERZA || obstaculo > OBSTACULO_INTELIGENCIA)
		return ERROR_CHAR;

	switch (obstaculo) {
	case OBSTACULO_FUERZA:
		return IDENTIFICADOR_OBSTACULO_FUERZA;

	case OBSTACULO_DESTREZA:
		return IDENTIFICADOR_OBSTACULO_DESTREZA;

	case OBSTACULO_INTELIGENCIA:
		return IDENTIFICADOR_OBSTACULO_INTELIGENCIA;
	}
	return ERROR_CHAR;
}

unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2)
		return 0;

	char id_obstaculo = obstaculo_identificar(obstaculo);
	if (id_obstaculo == ERROR_CHAR)
		return 0;

	char *id_obstaculo_dinamico = calloc(1, sizeof(char));
	if (!id_obstaculo_dinamico)
		return 0;
	*id_obstaculo_dinamico = id_obstaculo;

	switch (jugador) {
	case JUGADOR_1:
		if (!lista_insertar_en_posicion(tp->obstaculos_pista[0],
						id_obstaculo_dinamico,
						posicion)) {
			free(id_obstaculo_dinamico);
			return 0;
		}
		return (unsigned)(lista_tamanio(tp->obstaculos_pista[0]));

	case JUGADOR_2:
		if (!lista_insertar_en_posicion(tp->obstaculos_pista[1],
						id_obstaculo_dinamico,
						posicion)) {
			free(id_obstaculo_dinamico);
			return 0;
		}
		return (unsigned)(lista_tamanio(tp->obstaculos_pista[1]));
	}

	return 0;
}

unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2 ||
	    lista_vacia(tp->obstaculos_pista[jugador]))
		return 0;

	if (posicion > lista_tamanio(tp->obstaculos_pista[jugador]))
		return 0;

	char *aux = (char *)lista_elemento_en_posicion(
		tp->obstaculos_pista[jugador], posicion);
	if (!aux)
		return 0;

	if (!lista_quitar_de_posicion(tp->obstaculos_pista[jugador], posicion))
		return 0;

	free(aux);
	return (unsigned)(lista_tamanio(tp->obstaculos_pista[jugador]));
}

bool concatenar_obstaculo(void *elemento, void *contexto)
{
	char **string = (char **)contexto;
	**string = *(char *)elemento;
	(*string)++;

	return true;
}

char *tp_obstaculos_pista_string(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2)
		return NULL;

	char *nueva_string = calloc(
		lista_tamanio(tp->obstaculos_pista[jugador]) + 1, sizeof(char));
	if (!nueva_string)
		return NULL;

	char *string = nueva_string;

	lista_con_cada_elemento(tp->obstaculos_pista[jugador],
				concatenar_obstaculo, &string);

	*string = '\0';
	return nueva_string;
}

char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2 ||
	    lista_vacia(tp->obstaculos_pista[jugador]))
		return NULL;

	return tp_obstaculos_pista_string(tp, jugador);
}

void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador > 1 || jugador < 0 ||
	    lista_vacia(tp->obstaculos_pista[jugador]))
		return;

	lista_destruir_todo(tp->obstaculos_pista[jugador], free);

	tp->obstaculos_pista[jugador] = lista_crear();
	if (!tp->obstaculos_pista[jugador])
		return;
}

int obtener_atributo(pokemon_t *pokemon, char tipo_obstaculo)
{
	switch (tipo_obstaculo) {
	case 'F':
		return pokemon->fuerza;
	case 'D':
		return pokemon->destreza;
	case 'I':
		return pokemon->inteligencia;
	}
	return ERROR;
}

void actualizar_contador(int *contador_actualizar, int *contador_resetear1,
			 int *contador_resetear2, int atributo, int *tiempo)
{
	*contador_resetear1 = 0;
	*contador_resetear2 = 0;

	*tiempo = 10 - *contador_actualizar - atributo;
	if (*tiempo < 0)
		*tiempo = 0;

	(*contador_actualizar)++;
}

void identificador_actual(char identificador, int *contador_fuerza,
			  int *contador_destreza, int *contador_inteligencia,
			  int atributo, int *tiempo)
{
	if (identificador == 'F')
		actualizar_contador(contador_fuerza, contador_destreza,
				    contador_inteligencia, atributo, tiempo);

	else if (identificador == 'D')
		actualizar_contador(contador_destreza, contador_fuerza,
				    contador_inteligencia, atributo, tiempo);

	else if (identificador == 'I')
		actualizar_contador(contador_inteligencia, contador_fuerza,
				    contador_destreza, atributo, tiempo);
}

unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2 ||
	    !tp->pokemon_jugador[jugador] ||
	    lista_vacia(tp->obstaculos_pista[jugador]))
		return 0;

	lista_iterador_t *iterador =
		lista_iterador_crear(tp->obstaculos_pista[jugador]);
	if (!iterador)
		return 0;

	int contador_fuerza = 0, contador_destreza = 0,
	    contador_inteligencia = 0;
	int tiempo_total = 0, tiempo_obstaculo = 0;

	while (lista_iterador_tiene_siguiente(iterador)) {
		char *actual = (char *)lista_iterador_elemento_actual(iterador);

		int atributo =
			obtener_atributo(tp->pokemon_jugador[jugador], *actual);
		if (atributo == ERROR) {
			lista_iterador_destruir(iterador);
			return 0;
		}

		identificador_actual(*actual, &contador_fuerza,
				     &contador_destreza, &contador_inteligencia,
				     atributo, &tiempo_obstaculo);
		tiempo_total += tiempo_obstaculo;

		lista_iterador_avanzar(iterador);
	}

	lista_iterador_destruir(iterador);
	return (unsigned)tiempo_total;
}

char *expandir_buffer(char *buffer, size_t *buffer_size, size_t nuevo_tamano)
{
	if (nuevo_tamano > *buffer_size) {
		*buffer_size *= 2;
		char *nuevo_buffer = realloc(buffer, *buffer_size);
		if (!nuevo_buffer) {
			free(buffer);
			return NULL;
		}
		return nuevo_buffer;
	}
	return buffer;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador < JUGADOR_1 || jugador > JUGADOR_2 ||
	    lista_vacia(tp->obstaculos_pista[jugador]))
		return NULL;

	lista_iterador_t *it =
		lista_iterador_crear(tp->obstaculos_pista[jugador]);
	if (!it)
		return NULL;

	size_t buffer_size = 256;
	char *csv = calloc(1, buffer_size);
	if (!csv) {
		lista_iterador_destruir(it);
		return NULL;
	}
	csv[0] = '\0';

	int contador_fuerza = 0, contador_destreza = 0,
	    contador_inteligencia = 0;
	int tiempo_obstaculo = 0;

	while (lista_iterador_tiene_siguiente(it)) {
		char *actual = (char *)lista_iterador_elemento_actual(it);

		int atributo =
			obtener_atributo(tp->pokemon_jugador[jugador], *actual);
		if (atributo == ERROR) {
			lista_iterador_destruir(it);
			return 0;
		}

		identificador_actual(*actual, &contador_fuerza,
				     &contador_destreza, &contador_inteligencia,
				     atributo, &tiempo_obstaculo);

		char string_tiempo[MAX_DIGITOS_TIEMPO];
		sprintf(string_tiempo, "%d,", tiempo_obstaculo);
		size_t nuevo_tamano = strlen(csv) + strlen(string_tiempo) + 1;

		csv = expandir_buffer(csv, &buffer_size, nuevo_tamano);
		if (!csv) {
			lista_iterador_destruir(it);
			return NULL;
		}

		strcat(csv, string_tiempo);
		lista_iterador_avanzar(it);
	}

	lista_iterador_destruir(it);

	if (strlen(csv) > 0 && csv[strlen(csv) - 1] == ',')
		csv[strlen(csv) - 1] = '\0';

	return csv;
}

void tp_destruir(TP *tp)
{
	if (!tp)
		return;

	abb_destruir_todo(tp->pokemones, pokemon_info_destruir);
	tp->pokemon_jugador[0] = NULL;
	tp->pokemon_jugador[1] = NULL;

	lista_destruir_todo(tp->obstaculos_pista[0], free);
	lista_destruir_todo(tp->obstaculos_pista[1], free);

	free(tp);
}
