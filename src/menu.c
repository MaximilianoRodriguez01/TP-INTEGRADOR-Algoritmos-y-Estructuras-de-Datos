#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h> // para toupper()
#include <stdbool.h>
#include <math.h>

#include "tp.h"
#include "split.h"
#include <time.h>

enum COLOR { ROJO, VERDE, AMARILLO, AZUL, MAGENTA, CIAN, BLANCO };

enum TP_DIFICULTAD { FACIL, MEDIO, DIFICIL, IMPOSIBLE };

typedef struct menu {
	enum TP_DIFICULTAD dificultad;
	TP *tp;
	double puntaje;
	bool juego;
} menu_t;

void setear_color_printf(enum COLOR color)
{
	printf("\033[3%dm", color + 1);
}

menu_t *menu_crear(TP *tp, char *ruta_pokemones)
{
	if (ruta_pokemones == NULL) {
		setear_color_printf(ROJO);
		printf("Cargame un archivo valido de pokemones para jugar!\n");
		setear_color_printf(BLANCO);
		return NULL;
	}
	menu_t *menu = malloc(sizeof(menu_t));
	if (menu == NULL) {
		return NULL;
	}
	if (tp == NULL) {
		menu->tp = tp_crear(ruta_pokemones);
		if (menu->tp == NULL) {
			free(menu);
			return NULL;
		}
	} else {
		menu->tp = tp;
	}
	menu->dificultad = 0;
	menu->puntaje = 0;
	menu->juego = true;
	return menu;
}

void menu_destruir(menu_t *menu)
{
	if (menu != NULL) {
		tp_destruir(menu->tp);
		free(menu);
	}
}

//-------------------------------------------------funciones que ayudan a leer a los usuarios-------------------------------------------------

void setear_negrita_printf()
{
	printf("\033[1m");
}

void borrar_linea_actual()
{
	printf("\033[2K");
}

void remover_salto_de_linea(char *str)
{
	size_t longitud = strlen(str);
	if (longitud > 0 && str[longitud - 1] == '\n') {
		str[longitud - 1] = '\0';
	}
}

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

void comparar_entrada_disyuntiva(char *opcion, bool *cambiar)
{
	char *mayuscula = string_to_upper(opcion);
	remover_salto_de_linea(mayuscula);
	if (strcmp(mayuscula, "SI") == 0) {
		*cambiar = true;
	} else if (strcmp(mayuscula, "NO") == 0) {
		*cambiar = false;
	} else {
		printf("Entrada inválida. Por favor, ingrese 'sí' o 'no'.\n");
		*cambiar = false;
	}
	free(mayuscula);
}

void menu_limpiar_pantalla()
{
	printf("\033[H\033[J");
}

char *leer_entrada()
{
	size_t tamano = 32;
	char *entrada = malloc(tamano * sizeof(char));
	if (entrada == NULL) {
		printf("Error al asignar memoria\n");
		return NULL;
	}

	size_t indice = 0;
	while (1) {
		char *res =
			fgets(entrada + indice, (int)(tamano - indice), stdin);
		if (res == NULL) {
			free(entrada);
			return NULL;
		}

		size_t longitud = strlen(entrada);
		if (entrada[longitud - 1] == '\n') {
			return entrada;
		}

		tamano *= 2;
		char *tmp = realloc(entrada, tamano * sizeof(char));
		if (tmp == NULL) {
			free(entrada);
			printf("Error al reasignar memoria\n");
			return NULL;
		}
		entrada = tmp;
		indice = longitud;
	}
}

bool comparar_entrada_en_mayusculas(char *s1, char *s2)
{
	remover_salto_de_linea(s1);
	remover_salto_de_linea(s2);

	char *mayuscula_s1 = string_to_upper(s1);
	char *mayuscula_s2 = string_to_upper(s2);

	if (strcmp(mayuscula_s1, mayuscula_s2) == 0) {
		free(mayuscula_s1);
		free(mayuscula_s2);
		return true;
	} else {
		free(mayuscula_s1);
		free(mayuscula_s2);
		return false;
	}
}

bool menu_leer_y_comparar(char *s1, char *s2)
{
	char *entrada = leer_entrada();
	bool resultado1 = comparar_entrada_en_mayusculas(entrada, s1);
	bool resultado2 = comparar_entrada_en_mayusculas(entrada, s2);
	free(entrada);
	return resultado1 || resultado2;
}

void esperar_al_usuario()
{
	char *s = leer_entrada();
	if (s == NULL) {
		printf("Error al leer la entrada\n");
	}
	free(s);
}

void esperar_al_usuario_con_mensaje()
{
	printf("Presione enter para continuar...\n");
	esperar_al_usuario();
}

void menu_mover_cursor(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}

void menu_convertir_opcion_a_entero(char *opcion, int *numero)
{
	remover_salto_de_linea(opcion);
	*numero = atoi(opcion);
}

void menu_imprimir_dibujo(const char *filename, int x, int y)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("Error al abrir el archivo");
		return;
	}

	char line[256];
	menu_mover_cursor(x, y);
	while (fgets(line, sizeof(line), file)) {
		printf("%s", line);
		y++;
		menu_mover_cursor(x, y);
	}
	printf("\n");
	fclose(file);
}

//------------------------------------------------------------------funciones de menu--------------------------------------------------------------

void menu_imprimir_inicio(const char *f, int x, int y)
{
	menu_limpiar_pantalla();
	printf("                                            Bienvenido a la Carrera Pokemon!!\n");
	setear_color_printf(AMARILLO);
	printf("\n======================================================================================================================\n");
	menu_mover_cursor(3, 3);
	menu_imprimir_dibujo(f, x, y);
	printf("\n\n======================================================================================================================\n");
	setear_color_printf(BLANCO);
	esperar_al_usuario_con_mensaje();
	menu_limpiar_pantalla();
}

void menu_imprimir_tutorial()
{
	menu_limpiar_pantalla();
	setear_color_printf(ROJO);
	printf("\n\n\n======================================================================================================================\n");
	setear_color_printf(BLANCO);
	printf("                                          Bienvenido a la Carrera Pokemon!!\n\n");
	printf("       En este juego, deberás seleccionar un Pokemon y armar una pista de obstáculos para competir contra\n");
	printf("      el Pokemon del JUGADOR 2 (IA).");
	setear_color_printf(ROJO);
	printf(" El objetivo es que ambos Pokemones terminen la carrera de obstáculos\n");
	printf("                                  al mismo tiempo o lo más cercano posible.\n\n\n");
	setear_color_printf(BLANCO);
	printf("                                  Pokemones con atributos altos terminan antes la carrera\n\n");
	printf("           Al iniciar el juego, seleccionarás la dificultad que deseas (Fácil, Medio, Difícil, Imposible) y se\n");
	printf("           seleccionará un Pokemon al azar para el JUGADOR 2. Luego de seleccionar la dificultad, se creará una\n");
	printf("           pista de obstáculos para ambos jugadores. La longitud de la pista generada dependerá de la dificultad\n");
	printf("                                               seleccionada.\n\n");
	printf("          Se presentará la información del Pokemon contra el cuál vas a competir y se mostrará información acerca\n");
	printf("                                de la longitud y los obstáculos de la pista.\n\n");
	printf("          Luego de seleccionar tu Pokemon, que no puede ser el mismo que el del JUGADOR 2, deberás armar tu propia\n");
	printf("            pista de obstáculos. Una vez que hayas armado la pista, podrás correr la carrera y ver el resultado.\n");
	printf("                    Podrás modificar la pista y volver a correr para mejorar tu puntaje.\n\n");
	printf("                            Presiona cualquier tecla y enter para continuar...\n");
	setear_color_printf(ROJO);
	printf("======================================================================================================================\n");
	setear_color_printf(BLANCO);
	esperar_al_usuario();
	menu_limpiar_pantalla();
}

//---------------------------------------------------------------------funciones para seleccionar pokemones------------------------------------------------

void menu_pokemon_destruir_nombres(char **nombres_pokemon,
				   size_t cantidad_pokemon, char *nombres)
{
	for (size_t i = 0; i < cantidad_pokemon; i++) {
		free(nombres_pokemon[i]);
	}
	free(nombres_pokemon);
	free(nombres);
}

void imprimir_pantalla_pokemones_existentes(menu_t *menu)
{
	menu_limpiar_pantalla();
	char *nombres = tp_nombres_disponibles(menu->tp);
	printf("                                             Selecciona tu Pokemon!\n");
	printf("                                             Pokemones disponibles:\n\n");
	printf("======================================================================================================================\n");

	char *pokemon_seleccionado =
		tp_pokemon_seleccionado(menu->tp, JUGADOR_2)->nombre;

	int contador = 0;

	char **nombres_pokemon = split(nombres, ',');
	if (nombres_pokemon == NULL) {
		printf("Error al separar los nombres de los pokemones.\n");
		return;
	}

	for (int i = 0; nombres_pokemon[i] != NULL; i++) {
		if (contador == 5) {
			printf("\n\n");
			contador = 0;
		}

		const struct pokemon_info *pokemon =
			tp_buscar_pokemon(menu->tp, nombres_pokemon[i]);
		if (pokemon == NULL) {
			printf("Error al buscar el pokemon %s.\n",
			       nombres_pokemon[i]);
		}
		if (strcmp(pokemon->nombre, pokemon_seleccionado) == 0) {
			setear_color_printf(ROJO);
		}

		printf("    %-20s", pokemon->nombre);
		setear_color_printf(BLANCO);
		contador++;
	}
	printf("\n======================================================================================================================\n");
	printf("\n");

	menu_pokemon_destruir_nombres(nombres_pokemon,
				      (size_t)tp_cantidad_pokemon(menu->tp),
				      nombres);
}

void caracteristica_pokemon(const struct pokemon_info *pokemon)
{
	printf("\n💪 Fuerza 💪 : %d\n", pokemon->fuerza);
	printf("\n⚡️Destreza⚡️: %d\n", pokemon->destreza);
	printf("\n🧠 Inteligencia 🧠 : %d\n", pokemon->inteligencia);
}

void imprimir_pantalla_pokemon_seleccionado(menu_t *menu, int frameWidth,
					    int frameHeight, int x, int y,
					    enum TP_JUGADOR jugador)
{
	menu_limpiar_pantalla();

	const struct pokemon_info *pokemon =
		tp_pokemon_seleccionado(menu->tp, jugador);
	if (pokemon == NULL) {
		printf("No hay ningún Pokémon seleccionado.\n");
		printf("Presione cualquier tecla para continuar...\n");
		esperar_al_usuario_con_mensaje();
		menu_limpiar_pantalla();
		return;
	}

	printf("El JUGADOR %d ha seleccionado a %s\n", jugador + 1,
	       pokemon->nombre);

	char ruta[50];
	sprintf(ruta, "ejemplo/dibujos_pokemon/%s.txt", pokemon->nombre);

	printf("\nPokémon %s seleccionado con éxito.\n", pokemon->nombre);
	caracteristica_pokemon(tp_pokemon_seleccionado(menu->tp, jugador));
	printf("  __ ___         _            __     ___      ___\n");
	printf("  | '_  \\       | |           | |    \\  \\    /  /\n");
	printf("  | |_) |  ___  | | _____  ___| |___  \\  \\  /  /\n");
	printf("  | .___/ / _ \\ | |/ / _ \\/  _  | _ \\  \\  \\/  /\n");
	printf("  | |    | (_) ||   <| __/| (_) | __/  /  /\\  \\\n");
	printf("  | |     \\___/ |_|\\ _\\__/\\_____|___  /  /  \\  \\   \n");
	printf("  |_|                                /__/    \\__\\\n");
	menu_imprimir_dibujo(ruta, x, y);
}

void menu_seleccionar_pokemon_usuario(menu_t *menu, enum TP_JUGADOR jugador)
{
	imprimir_pantalla_pokemones_existentes(menu);
	bool seleccionado = false;
	while (!seleccionado) {
		printf("\033[2K");
		printf("                                Ingrese el nombre del Pokémon que desea seleccionar: ");
		char *nombre = leer_entrada();
		if (nombre == NULL) {
			printf("Error al leer la entrada\n");
		}
		remover_salto_de_linea(nombre);
		seleccionado =
			tp_seleccionar_pokemon(menu->tp, jugador, nombre);
		menu_mover_cursor(20, 18);
		if (!seleccionado) {
			setear_color_printf(ROJO);
			menu_mover_cursor(20, 23);
			printf("             No se pudo seleccionar el Pokémon. Intente nuevamente.\n");
			setear_color_printf(BLANCO);
			menu_mover_cursor(20, 20);

			printf("                                                                                           \n");
		}
		free(nombre);
	}
	imprimir_pantalla_pokemon_seleccionado(menu, 80, 24, 50, 5, jugador);
	esperar_al_usuario_con_mensaje();

	menu_limpiar_pantalla();
}

//---------------------------------------------------------------------funciones para cambiar la seleccion de pokemones------------------------------------------------

void menu_cambiar_seleccion(menu_t *menu, enum TP_JUGADOR jugador)
{
	int contador_anti_bucle = 0;
	while (1) {
		if (contador_anti_bucle == 100) {
			printf("ERROR 945: Juego no responde. Saliendo del programa.\n");
			return;
		}
		menu_limpiar_pantalla();
		menu_imprimir_dibujo(
			"ejemplo/dibujos_pokemon/pokemon_triste.txt", 30, 1);
		setear_color_printf(CIAN);
		printf("======================================================================================================================\n");
		setear_color_printf(BLANCO);
		printf("¿Deseas elegir de vuelta tu Pokémon? (Sí/No): ");

		char *opcion = leer_entrada();
		if (opcion == NULL) {
			printf("Error al leer la entrada\n");
			break;
		}
		if (comparar_entrada_en_mayusculas(opcion, "No")) {
			free(opcion);
			return;
		}
		if (comparar_entrada_en_mayusculas(opcion, "Si")) {
			menu_seleccionar_pokemon_usuario(menu, jugador);
		}
		free(opcion);
		contador_anti_bucle++;
	}
}

void menu_ia_elige_pokemon(menu_t *menu)
{
	char *nombres_csv = tp_nombres_disponibles(menu->tp);
	char **nombres_separados = split(nombres_csv, ',');
	if (nombres_separados == NULL) {
		printf("Error al elegir pokemon de la IA.\n");
		return;
	}

	int cantidad = tp_cantidad_pokemon(menu->tp);

	srand((unsigned int)time(NULL));
	int random = rand() % cantidad;

	tp_seleccionar_pokemon(menu->tp, JUGADOR_2, nombres_separados[random]);
	imprimir_pantalla_pokemon_seleccionado(menu, 80, 24, 55, 5, JUGADOR_2);
	esperar_al_usuario_con_mensaje();
	menu_pokemon_destruir_nombres(nombres_separados, (size_t)cantidad,
				      nombres_csv);
}

//---------------------------------------------------------------------funciones para elegir la dificultad------------------------------------------------

void menu_imprimir_dificultad()
{
	menu_limpiar_pantalla();
	setear_color_printf(MAGENTA);
	printf("======================================================================================================================\n");
	setear_color_printf(BLANCO);
	printf("                                       Elija la dificultad del juego:\n");
	setear_color_printf(MAGENTA);
	printf("======================================================================================================================\n");
	setear_color_printf(BLANCO);
	printf("\n                                                1. Fácil😊\n\n");
	printf("                                                2. Medio😐\n\n");
	printf("                                                3. Difícil😰\n\n");
	printf("                                                4. Imposible😱\n\n");
	setear_color_printf(MAGENTA);
	printf("======================================================================================================================\n");
	setear_color_printf(BLANCO);
}

void imprimir_mensaje_dificultad(enum TP_DIFICULTAD dificultad)
{
	borrar_linea_actual();
	switch (dificultad) {
	case FACIL:
		setear_color_printf(VERDE);
		printf("Dificultad seleccionada: Fácil\n");
		break;
	case MEDIO:
		setear_color_printf(AMARILLO);
		printf("Dificultad seleccionada: Medio\n");
		break;
	case DIFICIL:
		setear_color_printf(ROJO);
		printf("Dificultad seleccionada: Difícil\n");
		break;
	case IMPOSIBLE:
		setear_color_printf(MAGENTA);
		printf("Dificultad seleccionada: Imposible\n");
		break;
	default:
		setear_color_printf(ROJO);
		printf("\n                                    Opción inválida. Intente nuevamente: ");
		setear_color_printf(BLANCO);
		menu_mover_cursor(0, 14);
		return;
	}
	setear_color_printf(BLANCO);
}

// Función para leer y validar la opción del usuario
bool leer_y_validar_opcion(menu_t *menu)
{
	borrar_linea_actual();
	printf("                                           Ingrese una opción: ");
	char *opcion = leer_entrada();
	if (opcion == NULL) {
		printf("Error al leer la entrada\n");
		return false;
	}

	int dificultad_temp;
	menu_convertir_opcion_a_entero(opcion, &dificultad_temp);
	menu->dificultad = (enum TP_DIFICULTAD)(dificultad_temp - 1);

	imprimir_mensaje_dificultad(menu->dificultad);

	bool opcion_valida =
		(menu->dificultad == FACIL || menu->dificultad == MEDIO ||
		 menu->dificultad == DIFICIL || menu->dificultad == IMPOSIBLE);
	free(opcion);
	return opcion_valida;
}

// Función modularizada principal
void menu_elegir_difficultad(menu_t *menu)
{
	menu_imprimir_dificultad();
	bool opcion_valida = false;
	while (!opcion_valida) {
		opcion_valida = leer_y_validar_opcion(menu);
	}
}

//---------------------------------------------------------------------funciones para cambiar la dificultad------------------------------------------------

void menu_cambiar_dificultad(menu_t *menu)
{
	while (1) {
		borrar_linea_actual();
		printf("¿Deseas elegir de vuelta la dificultad del juego? (Sí/No): ");
		char *opcion = leer_entrada();
		if (opcion == NULL) {
			printf("Error al leer la entrada\n");

			break;
		}
		if (comparar_entrada_en_mayusculas(opcion, "No")) {
			free(opcion);
			return;
		} else if (comparar_entrada_en_mayusculas(opcion, "Si")) {
			menu_limpiar_pantalla();
			menu_elegir_difficultad(menu);
			free(opcion);
		} else {
			setear_color_printf(ROJO);
			printf("Entrada inválida. Por favor, ingrese 'sí' o 'no'.\n");
			setear_color_printf(BLANCO);
			free(opcion);
			menu_mover_cursor(0, 16);
		}
	}
}

//---------------------------------------------------------------------funciones para agregar obstaculos------------------------------------------------

void menu_ia_elige_pista(menu_t *menu)
{
	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < 4 + 2 * menu->dificultad; i++) {
		tp_agregar_obstaculo(menu->tp, JUGADOR_2, rand() % 3,
				     (unsigned int)rand() % 10);
	}
}

void menu_imprimir_pista_de_ia_segun_dificultad(menu_t *menu, char *pista)
{
	menu_limpiar_pantalla();
	size_t tamanio = strlen(pista);
	printf("                                    LA PISTA DEL JUGADOR 2 TIENE AL MENOS ESTOS OBSTÁCULOS: \n\n");
	setear_color_printf(AMARILLO);
	printf("======================================================================================================================\n");
	setear_color_printf(BLANCO);
	for (size_t i = 0; i < tamanio; i += menu->dificultad + 1) {
		printf("\n                                                     TIPO: '%c' ",
		       pista[i]);
		switch (pista[i]) {
		case 'F':
			printf("💪");
		case 'I':
			printf("🧠");
			break;
		case 'D':
			printf("⚡");
		default:
			printf("❓"); // Emoji de interrogación para tipos desconocidos
		}
		printf("\n");
	}
	setear_color_printf(AMARILLO);
	printf("\n======================================================================================================================\n\n");
	setear_color_printf(BLANCO);
	printf("LA PISTA DEL JUGADOR 2 TIENE UN TOTAL DE %i OBSTÁCULOS\n\n",
	       (int)tamanio);
}

void menu_imprimir_pista_de_ia(menu_t *menu)
{
	char *pista = tp_obstaculos_pista(menu->tp, JUGADOR_2);
	if (pista == NULL) {
		printf("No hay obstáculos en la pista.\n");
	} else {
		printf("                                 LA PISTA DEL JUGADOR 2 TIENE LAS SIGUIENTES CARACTERISTICAS: \n");
		menu_imprimir_pista_de_ia_segun_dificultad(menu, pista);
	}
	free(pista);
	esperar_al_usuario_con_mensaje();
}

// Función para leer y validar la posición
char *leer_y_validar_posicion()
{
	char *posicion = NULL;
	bool entrada_valida = false;
	while (!entrada_valida) {
		borrar_linea_actual();
		printf("Ingrese la posición en la que desea agregar el obstáculo: ");
		posicion = leer_entrada();
		if (posicion == NULL) {
			printf("Error al leer la entrada\n");
			return NULL;
		}
		if (posicion[0] != '0' && atoi(posicion) == 0) {
			setear_color_printf(ROJO);
			printf("Posición inválida. Intente nuevamente: ");
			setear_color_printf(BLANCO);
			free(posicion);
			menu_mover_cursor(0, 23);
			borrar_linea_actual();
		} else {
			entrada_valida = true;
		}
	}
	return posicion;
}

// Función para leer y validar el tipo de obstáculo
enum TP_OBSTACULO leer_y_validar_tipo_obstaculo()
{
	enum TP_OBSTACULO obstaculo;
	bool obstaculo_correcto = false;
	while (!obstaculo_correcto) {
		borrar_linea_actual();
		printf("Ingrese el tipo de obstáculo que desea agregar (F/D/I): ");
		char *tipo = leer_entrada();
		if (tipo == NULL) {
			printf("Error al leer la entrada\n");
			return -1; // Valor inválido indicando error
		}
		if (comparar_entrada_en_mayusculas(tipo, "F")) {
			obstaculo = OBSTACULO_FUERZA;
			obstaculo_correcto = true;
		} else if (comparar_entrada_en_mayusculas(tipo, "D")) {
			obstaculo = OBSTACULO_DESTREZA;
			obstaculo_correcto = true;
		} else if (comparar_entrada_en_mayusculas(tipo, "I")) {
			obstaculo = OBSTACULO_INTELIGENCIA;
			obstaculo_correcto = true;
		} else {
			setear_color_printf(ROJO);
			printf("Tipo de obstáculo inválido. Intente nuevamente: ");
			setear_color_printf(BLANCO);
			menu_mover_cursor(0, 24);
			borrar_linea_actual();
		}
		free(tipo);
	}
	return obstaculo;
}

// Función modularizada principal
void menu_agregar_obstaculo(menu_t *menu, enum TP_JUGADOR jugador)
{
	int numero_posicion = 0;

	char *posicion = leer_y_validar_posicion();
	if (posicion == NULL) {
		return;
	}

	menu_convertir_opcion_a_entero(posicion, &numero_posicion);
	free(posicion);

	enum TP_OBSTACULO obstaculo = leer_y_validar_tipo_obstaculo();
	if (obstaculo == -1) {
		return;
	}

	tp_agregar_obstaculo(menu->tp, jugador, obstaculo,
			     (unsigned int)numero_posicion);
}

//--------------------------------------------funciones para quitar obstaculos---------------------------------------------

void menu_quitar_obstaculo(menu_t *menu, enum TP_JUGADOR jugador)
{
	char *pista = tp_obstaculos_pista(menu->tp, jugador);
	if (pista == NULL) {
		printf("No se puede quitar obstáculos de una lista vacía. Presione enter para volver al menú anterior.\n");
		esperar_al_usuario();
		return;
	}
	free(pista);
	printf("Ingrese la posición en la que desea quitar un obstáculo: ");
	char *posicion = leer_entrada();
	if (posicion == NULL) {
		printf("Error al leer la entrada\n");
		return;
	}
	int numero_posicion = 0;
	menu_convertir_opcion_a_entero(posicion, &numero_posicion);
	if (numero_posicion < 0 ||
	    (posicion[0] != '0' && numero_posicion == 0)) {
		setear_color_printf(ROJO);
		printf("Posición inválida. La posición debe ser un número mayor o igual a 0. Presione enter para continuar...\n");
		setear_color_printf(BLANCO);
		esperar_al_usuario();
		free(posicion);
		return;
	}
	tp_quitar_obstaculo(menu->tp, jugador, (unsigned int)numero_posicion);
	free(posicion);
}

//-----------------------------------------funciones para el menu de obstaculos-------------------------------

void imprimir_menu_obstaculos()
{
	menu_limpiar_pantalla();
	printf("                                           Crear pista de obstáculos\n");
	setear_color_printf(AZUL);
	printf("======================================================================================================================\n\n");
	setear_color_printf(BLANCO);
	printf("                                              1. Agregar Obstáculo➕\n");
	printf("                                              2. Quitar Obstáculo➖\n");
	printf("                                              3. Limpiar Pista🧹\n");
	printf("                                              4. Correr Carrera🏁\n\n");
	setear_color_printf(AZUL);
	printf("======================================================================================================================\n\n");
	setear_color_printf(BLANCO);
	printf("                    Crea tu pista de obstáculos para la carrera. Puedes agregar, quitar o limpiar obstáculos.\n");
	printf("                      Los obstaculos pueden ser de tres tipos: Fuerza💪 (F), Destreza🧠 (D) o Inteligencia⚡ (I).\n\n");
	printf("                                            Las posiciones arrancan desde el '0'.\n\n");
	printf("                        Si una posicion excede el final de la pista, el obstaculo se inserta al final.\n\n");
	printf("                        Si el obstaculo anterior es del mismo tipo, se resta en 1 el tiempo del nuevo.\n\n");
	setear_color_printf(AZUL);
	printf("======================================================================================================================\n");
	setear_color_printf(BLANCO);
}

void menu_leer_opcion_obstaculos(menu_t *menu)
{
	bool seguir = true;
	while (seguir) {
		menu_limpiar_pantalla();
		imprimir_menu_obstaculos();
		char *pista = tp_obstaculos_pista(menu->tp, JUGADOR_1);
		if (pista != NULL) {
			printf("Pista de obstáculos actual: %s\n", pista);
			free(pista);
		} else {
			printf("No hay obstáculos actualmente en la pista.\n");
		}
		printf("Ingrese una opción: ");
		char *opcion = leer_entrada();
		if (opcion == NULL) {
			printf("Error al leer la entrada\n");
			continue;
		}
		if (comparar_entrada_en_mayusculas(opcion, "1")) {
			menu_agregar_obstaculo(menu, JUGADOR_1);
		} else if (comparar_entrada_en_mayusculas(opcion, "2")) {
			menu_quitar_obstaculo(menu, JUGADOR_1);
		} else if (comparar_entrada_en_mayusculas(opcion, "3")) {
			tp_limpiar_pista(menu->tp, JUGADOR_1);
		} else if (comparar_entrada_en_mayusculas(opcion, "4")) {
			seguir = false;
			menu_limpiar_pantalla();
		} else {
			printf("Opción inválida. Intente nuevamente.\n");
		}
		free(opcion);
	}
}

char **obtener_tiempos_por_obstaculo(menu_t *menu, enum TP_JUGADOR jugador,
				     char **tiempos)
{
	*tiempos = tp_tiempo_por_obstaculo(menu->tp, jugador);
	if (*tiempos == NULL) {
		printf("Error al simular la carrera.\n");
		return NULL;
	}
	char **tiempos_separados = split(*tiempos, ',');
	if (tiempos_separados == NULL) {
		free(*tiempos);
		printf("Error al separar los tiempos.\n");
		return NULL;
	}
	return tiempos_separados;
}

// Función para obtener la pista y su longitud
char *obtener_pista(menu_t *menu, enum TP_JUGADOR jugador,
		    size_t *longitud_pista)
{
	char *pista = tp_obstaculos_pista(menu->tp, jugador);
	*longitud_pista = strlen(pista);
	return pista;
}

//--------------------------------------------funciones para simular la carrera---------------------------------------------

void simular_carrera(menu_t *menu, char **tiempos_separados,
		     size_t longitud_pista)
{
	int puntaje_anterior = 0;
	int puntaje_actual = 0;
	setear_color_printf(AZUL);
	printf("EMPIEZA LA CARRERA! PRESIONE %d VECES ENTER HASTA LLEGAR A LA META\n",
	       (int)longitud_pista + 1);

	for (size_t i = 0; i < longitud_pista; i++) {
		esperar_al_usuario();
		menu_mover_cursor(30, 10);
		menu_convertir_opcion_a_entero(tiempos_separados[i],
					       &puntaje_actual);

		if (i < longitud_pista / 3)
			setear_color_printf(ROJO);
		else if (i < longitud_pista * 2 / 3)
			setear_color_printf(AMARILLO);
		else
			setear_color_printf(VERDE);
		printf("POSICION ACTUAL DEL JUGADOR 1: %zu\n", i);
		menu_mover_cursor(30, 12);
		if (puntaje_actual + puntaje_anterior > 0)
			setear_color_printf(ROJO);
		else if (puntaje_actual + puntaje_anterior > 50)
			setear_color_printf(AMARILLO);
		else
			setear_color_printf(VERDE);
		setear_color_printf(CIAN);
		printf("TIEMPO HASTA EL MOMENTO DE LA CARRERA DEL JUGADOR 1: %d\n",
		       puntaje_actual + puntaje_anterior);
		puntaje_anterior += puntaje_actual;
		setear_color_printf(BLANCO);
	}
	esperar_al_usuario();
}

// Función para liberar memoria
void liberar_memoria(char **tiempos_separados, char *tiempos, char *pista,
		     size_t longitud_pista)
{
	for (size_t i = 0; i < longitud_pista; i++) {
		free(tiempos_separados[i]);
	}
	free(tiempos_separados);
	free(tiempos);
	free(pista);
}

// Función modularizada principal
void menu_simular_carrera(menu_t *menu)
{
	char *tiempos = NULL;
	char **tiempos_separados =
		obtener_tiempos_por_obstaculo(menu, JUGADOR_1, &tiempos);
	if (tiempos_separados == NULL) {
		return;
	}

	size_t longitud_pista;
	char *pista = obtener_pista(menu, JUGADOR_1, &longitud_pista);

	simular_carrera(menu, tiempos_separados, longitud_pista);

	liberar_memoria(tiempos_separados, tiempos, pista, longitud_pista);
}

void calcular_mostrar_tiempos(menu_t *menu, unsigned *tiempo1,
			      unsigned *tiempo2)
{
	*tiempo1 = tp_calcular_tiempo_pista(menu->tp, JUGADOR_1);
	*tiempo2 = tp_calcular_tiempo_pista(menu->tp, JUGADOR_2);
	printf("El tiempo del jugador 1 es: %u\n", *tiempo1);
	printf("El tiempo del jugador 2 es: %u\n", *tiempo2);
}

// Función para calcular el puntaje
double calcular_puntaje(unsigned tiempo1, unsigned tiempo2)
{
	return 100 - 100 * fabs((double)tiempo1 - (double)tiempo2) /
			     ((double)tiempo1 + (double)tiempo2);
}

// ----------------------------- funciones para mostrar el puntaje y un mensaje ----------------------------

// Función para mostrar el puntaje
void mostrar_puntaje(double puntaje)
{
	printf("\n\n\nEl puntaje del jugador 1 es: %.2f.\n", puntaje);
}

// Función para mostrar los mensajes y dibujos según el puntaje
void mostrar_mensaje_y_dibujo(double puntaje)
{
	if (puntaje < 50) {
		setear_color_printf(ROJO);
		printf("Vamos, puedes hacerlo mejor la próxima vez!\n");
		menu_imprimir_dibujo(
			"ejemplo/dibujos_pokemon/psyduck_shockeado_mas_chico.txt",
			50, 5);
	} else if (puntaje >= 50 && puntaje < 75) {
		setear_color_printf(AMARILLO);
		printf("Buen trabajo, pero puedes mejorar!\n");
		menu_imprimir_dibujo(
			"ejemplo/dibujos_pokemon/charmander_festejando.txt", 50,
			5);
	} else if (puntaje >= 75 && puntaje < 90) {
		setear_color_printf(VERDE);
		printf("Excelente trabajo, sigue así!\n");
		menu_imprimir_dibujo(
			"ejemplo/dibujos_pokemon/pikachu_sorprendido.txt", 50,
			5);
	} else {
		setear_color_printf(CIAN);
		printf("¡Felicidades! Eres un MAESTRO POKEMON!\n");
		menu_imprimir_dibujo(
			"ejemplo/dibujos_pokemon/pikachu_feliz.txt", 50, 5);
	}
	setear_color_printf(BLANCO);
}

// Función principal modularizada
void menu_calcular_puntaje(menu_t *menu)
{
	menu_limpiar_pantalla();
	unsigned tiempo1, tiempo2;
	calcular_mostrar_tiempos(menu, &tiempo1, &tiempo2);
	menu->puntaje = calcular_puntaje(tiempo1, tiempo2);
	mostrar_puntaje(menu->puntaje);
	mostrar_mensaje_y_dibujo(menu->puntaje);
}
void imprimir_intento_actual(int contador, int intentos)
{
	menu_limpiar_pantalla();
	printf("Intento %d de %d\n", contador + 1, intentos);
	esperar_al_usuario_con_mensaje();
}

// Función para ejecutar las acciones de la carrera
void ejecutar_acciones_carrera(menu_t *menu, int contador)
{
	if (contador > 0) {
		menu_leer_opcion_obstaculos(menu);
	}
	menu_simular_carrera(menu);
	menu_calcular_puntaje(menu);
}

// Función para preguntar al usuario si desea continuar
void preguntar_continuar()
{
	menu_mover_cursor(14, 24);
	printf("¿Desea correr la carrera de nuevo? (Sí/No): ");
}

// Función para leer y validar la entrada del usuario
bool leer_y_validar_entrada(bool *continuar)
{
	bool entrada_valida = false;
	while (!entrada_valida) {
		char *opcion = leer_entrada();
		if (opcion == NULL) {
			printf("Error al leer la entrada\n");
		} else if (comparar_entrada_en_mayusculas(opcion, "No")) {
			*continuar = false;
			entrada_valida = true;
		} else if (comparar_entrada_en_mayusculas(opcion, "Si")) {
			*continuar = true;
			entrada_valida = true;
		} else {
			setear_color_printf(ROJO);
			printf("            Opción inválida. Intente nuevamente: ");
			setear_color_printf(BLANCO);
			menu_mover_cursor(14, 24);
			borrar_linea_actual();
			printf("¿Desea correr la carrera de nuevo? (Sí/No): ");
		}
		free(opcion);
	}
	return entrada_valida;
}

// Función modularizada principal
void menu_intentos(menu_t *menu, int intentos)
{
	bool continuar = true;
	int contador = 0;

	while (continuar && contador < intentos) {
		imprimir_intento_actual(contador, intentos);
		ejecutar_acciones_carrera(menu, contador);

		if (intentos > 1 && contador < intentos - 1) {
			preguntar_continuar();
		}

		if (contador == intentos - 1) {
			esperar_al_usuario_con_mensaje();
			return;
		}

		leer_y_validar_entrada(&continuar);
		contador++;
	}
}

void menu_correr_carrera(menu_t *menu)
{
	switch (menu->dificultad) {
	case FACIL:
		menu_intentos(menu, 4);
		break;
	case MEDIO:
		menu_intentos(menu, 3);
		break;
	case DIFICIL:
		menu_intentos(menu, 2);
		break;
	case IMPOSIBLE:
		menu_intentos(menu, 1);
		break;
	default:
		break;
	}
}

//------------------------------------------------funciones para preguntar si se quiere jugar de nuevo--------------------------------------------

void menu_limpiar_pistas(menu_t *menu)
{
	tp_limpiar_pista(menu->tp, JUGADOR_1);
	tp_limpiar_pista(menu->tp, JUGADOR_2);
}

void imprimir_mensaje_inicial()
{
	menu_limpiar_pantalla();
	setear_color_printf(AMARILLO);
	menu_imprimir_dibujo("ejemplo/dibujos_pokemon/pokemon_saludando.txt",
			     40, 4);
	setear_color_printf(BLANCO);
	menu_mover_cursor(4, 27);
	printf("Gracias por jugar a la Carrera Pokemon!!\n");
	printf("   ¿Desea jugar de nuevo? (Sí/No): ");
}

// Función para leer y validar la opción del usuario
char *leer_y_validar_opcion_menu()
{
	char *opcion = leer_entrada();
	if (opcion == NULL) {
		printf("Error al leer la entrada\n");
		return NULL;
	}
	while (!comparar_entrada_en_mayusculas(opcion, "No") &&
	       !comparar_entrada_en_mayusculas(opcion, "Si")) {
		setear_color_printf(ROJO);
		printf("   Opción inválida. Intente nuevamente.");
		setear_color_printf(BLANCO);
		free(opcion);
		menu_mover_cursor(4, 28);
		printf("\033[2K");
		menu_mover_cursor(4, 28);
		printf("¿Desea jugar de nuevo? (Sí/No): ");
		opcion = leer_entrada();
		if (opcion == NULL) {
			printf("Error al leer la entrada\n");
			break;
		}
	}
	return opcion;
}

// Función para actualizar el estado del juego basado en la opción del usuario
void actualizar_estado_juego(menu_t *menu, char *opcion)
{
	comparar_entrada_disyuntiva(opcion, &(menu->juego));
}

// Función modularizada principal
void menu_preguntar_jugar_de_nuevo(menu_t *menu)
{
	imprimir_mensaje_inicial();
	char *opcion = leer_y_validar_opcion_menu();
	if (opcion != NULL) {
		actualizar_estado_juego(menu, opcion);
		free(opcion);
	}
}