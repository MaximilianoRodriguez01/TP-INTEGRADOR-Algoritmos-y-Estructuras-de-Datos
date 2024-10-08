VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc

all: clean valgrind-juego

valgrind-alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

tp: src/*.c tp.c
	$(CC) $(CFLAGS) src/*.c tp.c -o tp

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

valgrind-juego: juego
	valgrind $(VALGRIND_FLAGS) ./juego

juego: src/*.c juego.c
	$(CC) $(CFLAGS) src/*.c juego.c -o juego

valgrind_juego_correr: juego
	valgrind $(VALGRIND_FLAGS) ./juego "ejemplo/pokemones.txt"

clean:
	rm -f pruebas_alumno pruebas_chanutron juego
