#ifndef BSQ_H
#define BSQ_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* ========================================================================== */
/*                              ESTRUCTURAS                                  */
/* ========================================================================== */

typedef struct s_map {
    int     rows;           // Número de filas del mapa
    int     cols;           // Número de columnas del mapa
    char    empty;          // Carácter que representa espacio vacío
    char    obstacle;       // Carácter que representa obstáculo
    char    full;           // Carácter para rellenar el cuadrado solución
    char    **grid;         // Matriz 2D que contiene el mapa
} t_map;

/* ========================================================================== */
/*                          PROTOTIPOS DE FUNCIONES                          */
/* ========================================================================== */

/* --- Funciones de Utilidad --- */
size_t  ft_strlen(char *s);
int     is_printable(char c);
int     min3(int a, int b, int c);

/* --- Funciones de Gestión de Memoria --- */
void    free_map(t_map *map);

/* --- Funciones de Entrada/Salida --- */
t_map   *read_file(t_map *map, char *file);
void    print_map(t_map *map);
void    process_input(t_map *map, char *file);

/* --- Función Principal del Algoritmo --- */
void    solve_bsq(t_map *map);

#endif
