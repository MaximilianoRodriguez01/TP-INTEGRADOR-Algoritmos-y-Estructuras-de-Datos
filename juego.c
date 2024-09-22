#include "src/tp.h"
#include "src/menu.h"
#include "src/lista.h"
#include "src/split.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	//Se recomienda pasar el archivo de texto con los pokemon como argumento al ejecutar el programa

	setear_negrita_printf();
	const char *pokemon = "ejemplo/pokemon_logo.txt";
	menu_imprimir_inicio(pokemon, 30, 5);
	menu_imprimir_tutorial();

	menu_t *menu = menu_crear(NULL, argv[1]);
	if (menu == NULL) {
		printf("Error al crear el menú\n");
		return 0; // O manejar el error de manera adecuada
	}

	while (menu->juego) {
		menu_ia_elige_pokemon(menu);

		menu_seleccionar_pokemon_usuario(menu, JUGADOR_1);
		menu_cambiar_seleccion(menu, JUGADOR_1);

		menu_elegir_difficultad(menu);
		menu_cambiar_dificultad(menu);

		menu_ia_elige_pista(menu);
		menu_imprimir_pista_de_ia(menu);

		imprimir_menu_obstaculos();
		menu_leer_opcion_obstaculos(menu);

		menu_correr_carrera(menu);
		menu_preguntar_jugar_de_nuevo(menu);
	}
	menu_destruir(menu);
	menu_limpiar_pantalla();
	return 0;
}
