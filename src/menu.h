#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "abb.h"
#include "lista.h"

enum COLOR { ROJO, VERDE, AMARILLO, AZUL, MAGENTA, CIAN, BLANCO };

enum TP_DIFICULTAD { FACIL, MEDIO, DIFICIL, IMPOSIBLE };

typedef struct menu {
	enum TP_DIFICULTAD dificultad;
	TP *tp;
	double puntaje;
	bool juego;
} menu_t;

menu_t *menu_crear(TP *tp, const char *ruta_pokemones);

void menu_destruir(menu_t *menu);

void setear_color_printf(enum COLOR color);

void setear_negrita_printf();

void borrar_linea_actual();

void remover_salto_de_linea(char *str);

char *string_to_upper(const char *string);

void comparar_entrada_disyuntiva(char *opcion, bool *cambiar);

void menu_limpiar_pantalla();

char *leer_entrada();

bool comparar_entrada_en_mayusculas(char *s1, char *s2);

bool menu_leer_y_comparar(char *s1, char *s2);

void esperar_al_usuario();

void esperar_al_usuario_con_mensaje();

void menu_mover_cursor(int x, int y);

void menu_convertir_opcion_a_entero(char *opcion, int *numero);

void menu_imprimir_dibujo(const char *filename, int x, int y);

void menu_imprimir_inicio(const char *f, int x, int y);

void menu_imprimir_tutorial();

void menu_imprimir_final();

void imprimir_pantalla_ganador(const char *filename, int frameHeight, int x,
			       int y);

void imprimir_pantalla_perdedor(int frameWidth, int frameHeight,
				const char *filename, int x, int y);

void menu_pokemon_destruir_nombres(char **nombres_pokemon,
				   size_t cantidad_pokemon, char *nombres);

void imprimir_pantalla_pokemones_existentes(menu_t *menu);

void caracteristica_pokemon(const struct pokemon_info *pokemon);

void imprimir_pantalla_pokemon_seleccionado(menu_t *menu, int frameWidth,
					    int frameHeight, int x, int y,
					    enum TP_JUGADOR jugador);

void menu_seleccionar_pokemon_usuario(menu_t *menu, enum TP_JUGADOR jugador);

void menu_cambiar_seleccion(menu_t *menu, enum TP_JUGADOR jugador);

void menu_ia_elige_pokemon(menu_t *menu);

void menu_elegir_difficultad(menu_t *menu);

void menu_cambiar_dificultad(menu_t *menu);

void menu_ia_elige_pista(menu_t *menu);

void menu_imprimir_pista_de_ia_segun_dificultad(menu_t *menu, char *pista);

void menu_imprimir_pista_de_ia(menu_t *menu);

void menu_agregar_obstaculo(menu_t *menu, enum TP_JUGADOR jugador);

void menu_quitar_obstaculo(menu_t *menu, enum TP_JUGADOR jugador);

void imprimir_menu_obstaculos();

void menu_leer_opcion_obstaculos(menu_t *menu);

void menu_simular_carrera(menu_t *menu);

void menu_calcular_puntaje(menu_t *menu);

void menu_intentos(menu_t *menu, int intentos);

void menu_correr_carrera(menu_t *menu);

void menu_limpiar_pistas(menu_t *menu);

void menu_preguntar_jugar_de_nuevo(menu_t *menu);
