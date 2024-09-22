#include "split.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **split(const char *string, char delimitador)
{
	//chequeo si mi string apunta a null
	if (string == NULL)
		return NULL;

	size_t largo = strlen(string);
	size_t cantidad_de_delimitadores = 0;

	for (size_t posicion = 0; posicion < largo; posicion++) {
		if (string[posicion] == delimitador)
			cantidad_de_delimitadores++;
	}

	//Para reservar memoria plantee 3 contadores que me van a decir cuántos caracteres
	//tiene que tener cada vector de chars (cantidad_de_caracteres),
	//en qué posición estoy del char**     (contador)
	//y desde donde hasta donde tengo
	//que copiar mi string usando strncpy   (inicio)

	size_t cantidad_de_caracteres = 0;
	size_t contador = 0;
	size_t inicio = 0;

	//uso cantidad de delimitadores + 2 ya que, por ejemplo ALGO!2 tiene 1 delimitador, se convierte en 2 strings
	//y la ultima posicion tiene que ser el NULL

	char **nueva_cadena_de_strings =
		malloc((cantidad_de_delimitadores + 2) * sizeof(char *));
	if (nueva_cadena_de_strings == NULL) {
		printf("Error al asignar memoria'\n'");
		return NULL;
	}

	//Recorro el vector, si no es el delimitador, sigo iterando y voy contando cuantos caracteres tengo hasta llegar al delimitador
	//Cuando lo encuentro, me guardo un espacio de memoria con la cantidad de caracteres que leí, y me copio en ese espacio
	//los chars que habia leido.
	//Una vez copiados, pongo en la posicion siguiente el '\0' y pongo que mi cantidad de caracteres leidos es 0
	//Hago que inicio se saltee el caracter delimitador para poder copiar despues el string
	//Contador se actualiza para saber que estoy en el siguiente vector de chars

	for (size_t i = 0; i <= largo; i++) {
		if (string[i] == delimitador || string[i] == '\0') {
			nueva_cadena_de_strings[contador] = malloc(
				(cantidad_de_caracteres + 1) * sizeof(char));
			if (nueva_cadena_de_strings[contador] == NULL) {
				for (size_t x = 0; x < contador; x++) {
					free(nueva_cadena_de_strings[x]);
				}
				free(nueva_cadena_de_strings);
				printf("Error al asignar memoria'\n'");
				return NULL;
			}

			strncpy(nueva_cadena_de_strings[contador],
				(string + inicio), cantidad_de_caracteres);
			nueva_cadena_de_strings[contador]
					       [cantidad_de_caracteres] = '\0';

			inicio = i + 1;
			cantidad_de_caracteres = 0;
			contador++;

		} else
			cantidad_de_caracteres++;
	}
	nueva_cadena_de_strings[contador] = NULL;
	return nueva_cadena_de_strings;
}