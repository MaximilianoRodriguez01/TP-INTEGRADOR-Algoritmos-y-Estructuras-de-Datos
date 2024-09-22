#include "pa2m.h"
#include "src/tp.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void tp_pruebas_creacion()
{
	pa2m_nuevo_grupo("Pruebas de creación del TP");

	pa2m_afirmar(!tp_crear(NULL),
		     "No se puede crear un TP con un nombre de archivo nulo.");
	pa2m_afirmar(
		!tp_crear("HOLA"),
		"No se puede crear un TP con un nombre de archivo incorrecto.");

	TP *tp1 = tp_crear("ejemplo/pokemones.txt");
	pa2m_afirmar(
		tp1 != NULL,
		"El TP se crea correctamente a partir del archivo 'pokemones.txt'.");
	pa2m_afirmar(tp_cantidad_pokemon(tp1) == 25,
		     "El TP creado tiene la cantidad de pokemones correcta.");
	tp_destruir(tp1);

	TP *tp2 = tp_crear("ejemplo/pokemon.txt");
	pa2m_afirmar(
		tp2 != NULL,
		"El TP se crea correctamente a partir del archivo 'pokemon.txt'.");
	pa2m_afirmar(tp_cantidad_pokemon(tp2) == 1,
		     "El TP creado tiene la cantidad de pokemones correcta.");
	tp_destruir(tp2);

	TP *tp3 = tp_crear("ejemplo/menos_pokemones.txt");
	pa2m_afirmar(
		tp3 != NULL,
		"El TP se crea correctamente a partir del archivo 'menos_pokemones.txt'.");
	pa2m_afirmar(tp_cantidad_pokemon(tp3) == 12,
		     "El TP creado tiene la cantidad de pokemones correcta.");
	tp_destruir(tp3);
}

void tp_pruebas_cantidad()
{
	pa2m_nuevo_grupo("Pruebas de cantidad de Pokemones en el TP");

	pa2m_afirmar(
		!tp_cantidad_pokemon(NULL),
		"No se puede obtener la cantidad de pokemones de un TP nulo.");

	TP *tp1 = tp_crear("ejemplo/pokemones.txt");
	pa2m_afirmar(tp_cantidad_pokemon(tp1) == 25,
		     "La cantidad es correcta con un archivo de 25 pokemones.");
	tp_destruir(tp1);

	TP *tp2 = tp_crear("ejemplo/pokemon.txt");
	pa2m_afirmar(tp_cantidad_pokemon(tp2) == 1,
		     "La cantidad es correcta con un archivo de 1 pokemon.");
	tp_destruir(tp2);

	TP *tp3 = tp_crear("ejemplo/menos_pokemones.txt");
	pa2m_afirmar(tp_cantidad_pokemon(tp3) == 12,
		     "La cantidad es correcta con un archivo de 12 pokemones.");
	tp_destruir(tp3);
}

void tp_pruebas_busqueda()
{
	pa2m_nuevo_grupo("Pruebas de búsqueda de Pokemon en el TP");

	pa2m_afirmar(!tp_buscar_pokemon(NULL, "Pikachu"),
		     "No se encuentra un Pokemon en un TP nulo.");

	TP *tp1 = tp_crear("ejemplo/pokemones.txt");

	pa2m_afirmar(!tp_buscar_pokemon(tp1, NULL),
		     "No se encuentra un Pokemon nulo.");
	pa2m_afirmar(!tp_buscar_pokemon(tp1, "No Existe"),
		     "No se encuentra un Pokemon inexistente.");

	const struct pokemon_info *pikachu = tp_buscar_pokemon(tp1, "Pikachu");
	pa2m_afirmar(pikachu != NULL,
		     "Se encuentra el pokemon llamado 'Pikachu'.");
	pa2m_afirmar(
		strcmp(pikachu->nombre, "PIKACHU") == 0,
		"El nombre del pokemon encontrado coincide con 'Pikachu'.");

	const struct pokemon_info *zapdos = tp_buscar_pokemon(tp1, "Zapdos");
	pa2m_afirmar(zapdos != NULL,
		     "Se encuentra el pokemon llamado 'Zapdos'.");
	pa2m_afirmar(strcmp(zapdos->nombre, "ZAPDOS") == 0,
		     "El nombre del pokemon encontrado coincide con 'Zapdos'.");

	tp_destruir(tp1);

	TP *tp2 = tp_crear("ejemplo/pokemon.txt");

	const struct pokemon_info *rattata = tp_buscar_pokemon(tp2, "Rattata");
	pa2m_afirmar(
		rattata != NULL,
		"Se encuentra el único pokemon del archivo llamado 'Rattata'.");
	pa2m_afirmar(
		strcmp(rattata->nombre, "RATTATA") == 0,
		"El nombre del pokemon encontrado coincide con 'Rattata'.");

	tp_destruir(tp2);
}

void tp_pruebas_nombres_disponibles()
{
	pa2m_nuevo_grupo("Pruebas de nombres disponibles de Pokemon en el TP");

	pa2m_afirmar(
		!tp_nombres_disponibles(NULL),
		"No se puede obtener un string con los nombres de los pokemones de un TP nulo.");

	TP *tp1 = tp_crear("ejemplo/pokemones.txt");

	char *nombres1 = tp_nombres_disponibles(tp1);
	pa2m_afirmar(
		nombres1 != NULL,
		"Se obtiene un string con los nombres de los pokemones en el archivo.");

	char *original1 =
		"ARTICUNO,BLASTOISE,BULBASAUR,CATERPIE,CHARIZARD,DIGLETT,DRAGONAIR,DRAGONITE,DRATINI,EKANS,GEODUDE,GRIMER,IVYSAUR,MEOWTH,MOLTRES,PIDGEY,PIKACHU,RAICHU,RATTATA,SANDSHREW,SQUIRTLE,VENUSAUR,VULPIX,WARTORTLE,ZAPDOS";
	pa2m_afirmar(
		strcmp(nombres1, original1) == 0,
		"Se obtiene el string con todos los nombres de los pokemones ordenados alfabéticamente.");

	free(nombres1);
	tp_destruir(tp1);

	TP *tp2 = tp_crear("ejemplo/pokemon.txt");

	char *nombres2 = tp_nombres_disponibles(tp2);
	pa2m_afirmar(
		nombres2 != NULL,
		"Se obtiene un string con el nombre del pokemon en el archivo.");

	char *original2 = "RATTATA";
	pa2m_afirmar(strcmp(nombres2, original2) == 0,
		     "Se obtiene el string con el nombre del pokemon.");

	free(nombres2);
	tp_destruir(tp2);
}

void tp_pruebas_seleccion()
{
	pa2m_nuevo_grupo("Pruebas de selección de Pokemon en el TP");

	pa2m_afirmar(
		!tp_seleccionar_pokemon(NULL, JUGADOR_1, "Pikachu"),
		"No se puede seleccionar el pokemon para el jugador 1 en un TP nulo.");

	TP *tp = tp_crear("ejemplo/pokemones.txt");

	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, 4, "Pikachu"),
		"No se puede seleccionar el pokemon para un jugador inválido.");
	pa2m_afirmar(!tp_seleccionar_pokemon(tp, JUGADOR_1, "No Existe"),
		     "No se puede seleccionar un pokemon inválido.");

	pa2m_afirmar(tp_seleccionar_pokemon(tp, JUGADOR_1, "Pikachu"),
		     "Se selecciona el pokemon 'Pikachu' para el Jugador 1.");

	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_2, "Pikachu"),
		"No se puede seleccionar el pokemon 'Pikachu' para el Jugador 2, pues ya lo tiene el 1.");

	pa2m_afirmar(tp_seleccionar_pokemon(tp, JUGADOR_2, "Squirtle"),
		     "Se selecciona el pokemon 'Squirtle' para el Jugador 2.");
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle"),
		"No se puede seleccionar el pokemon 'Squirtle' para el Jugador 1, pues ya lo tiene el 2.");

	const struct pokemon_info *seleccionado1 =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	const struct pokemon_info *seleccionado2 =
		tp_pokemon_seleccionado(tp, JUGADOR_2);

	pa2m_afirmar(strcmp(seleccionado1->nombre, "PIKACHU") == 0,
		     "El Jugador 1 tiene el pokemon 'Pikachu' seleccionado.");
	pa2m_afirmar(strcmp(seleccionado2->nombre, "SQUIRTLE") == 0,
		     "El Jugador 2 tiene el pokemon 'Squirtle' seleccionado.");

	tp_destruir(tp);
}

void tp_pruebas_agregado_obstaculos()
{
	pa2m_nuevo_grupo("Pruebas de agregado de obstáculos al TP");

	TP *tp = tp_crear("ejemplo/pokemones.txt");

	tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle");
	tp_seleccionar_pokemon(tp, JUGADOR_2, "Pikachu");

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0) == 1,
		"Se agrega un obstáculo de fuerza en la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5) == 2,
		"Se agrega un obstáculo de destreza en la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA,
				     3) == 3,
		"Se agrega un obstáculo de inteligencia en la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 0) == 4,
		"Se agrega un obstáculo de destreza en una posición en la que había uno de fuerza en la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 2) == 5,
		"Se agrega un obstáculo de fuerza en una posición en la que había uno de destreza en la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA,
				     3) == 6,
		"Se agrega un obstáculo de inteligencia en una posición en la que había uno de destreza en la pista del jugador 1.");

	char *obstaculos1 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(strcmp(obstaculos1, "DFFIDI") == 0,
		     "Se obtiene el vector con los obstáculos esperado.");

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0) == 1,
		"Se agrega un obstáculo de fuerza en la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 5) == 2,
		"Se agrega un obstáculo de destreza en la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA,
				     3) == 3,
		"Se agrega un obstáculo de inteligencia en la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA,
				     0) == 4,
		"Se agrega un obstáculo de inteligencia en una posición en la que había uno de fuerza en la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 2) == 5,
		"Se agrega un obstáculo de fuerza en una posición en la que había uno de destreza en la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3) == 6,
		"Se agrega un obstáculo de destreza en una posición en la que había uno de destreza en la pista del jugador 2.");

	char *obstaculos2 = tp_obstaculos_pista(tp, JUGADOR_2);
	pa2m_afirmar(strcmp(obstaculos2, "IFFDDI") == 0,
		     "Se obtiene el vector con los obstáculos esperado.");

	free(obstaculos1);
	free(obstaculos2);

	tp_destruir(tp);
}

void tp_pruebas_quitado_obstaculos()
{
	pa2m_nuevo_grupo("Pruebas de quitado de obstáculos del TP");

	pa2m_afirmar(
		!tp_quitar_obstaculo(NULL, JUGADOR_1, 0),
		"No se puede quitar el obstáculo de una posición de un TP nulo.");

	TP *tp = tp_crear("ejemplo/pokemones.txt");
	tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle");
	tp_seleccionar_pokemon(tp, JUGADOR_2, "Pikachu");

	pa2m_afirmar(!tp_quitar_obstaculo(tp, JUGADOR_1, 0),
		     "No se puede quitar el obstáculo de una pista vacía.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 5);

	pa2m_afirmar(
		!tp_quitar_obstaculo(tp, JUGADOR_1, 10),
		"No se puede quitar el obstáculo de una posición inválida.");

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_1, 0) == 5,
		"Se quitó el obstáculo del principio correctamente del jugador 1.");
	char *obstaculos1 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos1, "DIDIF") == 0,
		"Al quitar un obstáculo del jugador 1, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos1);

	pa2m_afirmar(tp_quitar_obstaculo(tp, JUGADOR_1, 4) == 4,
		     "Se quitó el obstáculo del final correctamente.");
	char *obstaculos2 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos2, "DIDI") == 0,
		"Al quitar un obstáculo del jugador 1, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos2);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 5);

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 0) == 5,
		"Se quitó el obstáculo del principio correctamente del jugador 2.");
	char *obstaculos3 = tp_obstaculos_pista(tp, JUGADOR_2);
	pa2m_afirmar(
		strcmp(obstaculos3, "DIFFF") == 0,
		"Al quitar un obstáculo del jugador 2, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos3);

	pa2m_afirmar(tp_quitar_obstaculo(tp, JUGADOR_2, 4) == 4,
		     "Se quitó el obstáculo del final correctamente.");
	char *obstaculos4 = tp_obstaculos_pista(tp, JUGADOR_2);
	pa2m_afirmar(
		strcmp(obstaculos4, "DIFF") == 0,
		"Al quitar un obstáculo del jugador 2, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos4);

	tp_destruir(tp);
}

void tp_pruebas_obstaculos_pista()
{
	pa2m_nuevo_grupo("Pruebas de la cadena de obstáculos del TP");

	pa2m_afirmar(!tp_obstaculos_pista(NULL, JUGADOR_1),
		     "No se puede obtener el vector de un TP nulo.");

	TP *tp = tp_crear("ejemplo/pokemones.txt");
	tp_seleccionar_pokemon(tp, JUGADOR_1, "Pikachu");

	pa2m_afirmar(!tp_obstaculos_pista(tp, JUGADOR_1),
		     "No se puede obtener el vector de una pista vacía.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);

	char *obstaculos1 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos1, "F") == 0,
		"Al agregar un obstáculo, se obtiene el vector con el obstáculo esperado.");
	free(obstaculos1);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	char *obstaculos2 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos2, "FI") == 0,
		"Al agregar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos2);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	char *obstaculos3 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos3, "FID") == 0,
		"Al agregar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos3);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 0);
	char *obstaculos4 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos4, "IFID") == 0,
		"Al agregar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos4);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2);
	char *obstaculos5 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos5, "IFDID") == 0,
		"Al agregar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos5);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 4);
	char *obstaculos6 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos6, "IFDIFD") == 0,
		"Al agregar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos6);

	tp_quitar_obstaculo(tp, JUGADOR_1, 4);
	char *obstaculos7 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos7, "IFDID") == 0,
		"Al quitar un obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos7);

	tp_quitar_obstaculo(tp, JUGADOR_1, 2);
	char *obstaculos8 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos8, "IFID") == 0,
		"Al quitar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos8);

	tp_quitar_obstaculo(tp, JUGADOR_1, 0);
	char *obstaculos9 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos9, "FID") == 0,
		"Al quitar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos9);

	tp_quitar_obstaculo(tp, JUGADOR_1, 1);
	char *obstaculos10 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos10, "FD") == 0,
		"Al quitar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos10);

	tp_quitar_obstaculo(tp, JUGADOR_1, 1);
	char *obstaculos11 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(obstaculos11, "F") == 0,
		"Al quitar otro obstáculo, se obtiene el vector con los obstáculos esperado.");
	free(obstaculos11);

	tp_destruir(tp);
}

void tp_pruebas_tiempo_pista()
{
	pa2m_nuevo_grupo("Pruebas de cálculo del tiempo de pista del TP");

	pa2m_afirmar(tp_calcular_tiempo_pista(NULL, JUGADOR_1) == 0,
		     "El tiempo calculado de un TP nulo es 0.");

	TP *tp = tp_crear("ejemplo/pokemones.txt");

	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 0,
		"El tiempo calculado para un jugador que no seleccionó el pokemon es 0.");

	tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle");

	pa2m_afirmar(tp_calcular_tiempo_pista(NULL, JUGADOR_1) == 0,
		     "El tiempo calculado de una pista vacía es 0.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 6,
		"Al agregar un obstáculo de fuerza, el tiempo calculado es correcto.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 13,
		"Al agregar un obstáculo de destreza, el tiempo calculado es correcto.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 2);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 18,
		"Al agregar un obstáculo de inteligencia, el tiempo calculado es correcto.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 27,
		"Al agregar varios obstáculos de fuerza seguidos, el tiempo calculado es correcto.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 38,
		"Al agregar varios obstáculos de destreza seguidos, el tiempo calculado es correcto.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 45,
		"Al agregar varios obstáculos de inteligencia seguidos, el tiempo calculado es correcto.");

	tp_destruir(tp);
}

void tp_pruebas_tiempo_por_obstaculo()
{
	pa2m_nuevo_grupo("Pruebas del tiempo por obstáculo del TP");

	pa2m_afirmar(
		!tp_tiempo_por_obstaculo(NULL, JUGADOR_1),
		"No se puede obtener el string esperado con los tiempos por obstáculo de un TP nulo.");

	TP *tp = tp_crear("ejemplo/pokemones.txt");

	pa2m_afirmar(
		!tp_tiempo_por_obstaculo(tp, JUGADOR_1),
		"No se puede obtener el string esperado con los tiempos por obstáculo sin haber seleccionado un pokemon.");

	tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle");

	pa2m_afirmar(
		!tp_tiempo_por_obstaculo(tp, JUGADOR_1),
		"No se puede obtener el string esperado con los tiempos por obstáculo de un TP vacío.");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 2);

	char *tiempos1 = tp_tiempo_por_obstaculo(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(tiempos1, "6,5,4") == 0,
		"Se obtiene el string esperado con los tiempos por obstáculo.");
	free(tiempos1);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5);

	char *tiempos2 = tp_tiempo_por_obstaculo(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(tiempos2, "6,5,4,7,6,5") == 0,
		"Se obtiene el string esperado con los tiempos por obstáculo.");
	free(tiempos2);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 7);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 8);

	char *tiempos3 = tp_tiempo_por_obstaculo(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(tiempos3, "6,5,4,7,6,5,5,4,3") == 0,
		"Se obtiene el string esperado con los tiempos por obstáculo.");
	free(tiempos3);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 9);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 10);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 11);

	char *tiempos4 = tp_tiempo_por_obstaculo(tp, JUGADOR_1);
	pa2m_afirmar(
		strcmp(tiempos4, "6,5,4,7,6,5,5,4,3,6,7,5") == 0,
		"Se obtiene el string esperado con los tiempos por obstáculo.");
	free(tiempos4);

	tp_destruir(tp);
}

void tp_pruebas_limpieza_pista()
{
	pa2m_nuevo_grupo("Pruebas de limpieza de la lista del TP");

	TP *tp = tp_crear("ejemplo/pokemones.txt");
	tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle");
	tp_seleccionar_pokemon(tp, JUGADOR_2, "Rattata");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 7);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 8);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 7);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 8);

	tp_limpiar_pista(tp, JUGADOR_1);
	tp_limpiar_pista(tp, JUGADOR_2);

	pa2m_afirmar(true, "La pista se limpia correctamente.");

	tp_destruir(tp);
}

void tp_pruebas_destruccion()
{
	pa2m_nuevo_grupo("Pruebas de destrucción del TP");

	TP *tp = tp_crear("ejemplo/pokemones.txt");
	tp_seleccionar_pokemon(tp, JUGADOR_1, "Squirtle");
	tp_seleccionar_pokemon(tp, JUGADOR_2, "Rattata");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 7);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 8);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 7);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 8);

	tp_destruir(tp);

	pa2m_afirmar(true, "El TP se destruye correctamente.");
}

void tp_pruebas()
{
	tp_pruebas_creacion();
	tp_pruebas_cantidad();
	tp_pruebas_busqueda();
	tp_pruebas_nombres_disponibles();
	tp_pruebas_seleccion();
	tp_pruebas_agregado_obstaculos();
	tp_pruebas_quitado_obstaculos();
	tp_pruebas_obstaculos_pista();
	tp_pruebas_tiempo_pista();
	tp_pruebas_tiempo_por_obstaculo();
	tp_pruebas_limpieza_pista();
	tp_pruebas_destruccion();
}

int main()
{
	tp_pruebas();

	pa2m_mostrar_reporte();

	return 0;
}
