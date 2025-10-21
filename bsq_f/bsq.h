#ifndef BSQ_H
#define BSQ_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* ========================================================================== */
/*                              STRUCTURES                                   */
/* ========================================================================== */

typedef struct s_map {
    int     rows;
    int     cols;
    char    empty;
    char    obstacle;
    char    full;
    char  **grid;
} t_map;

/* ========================================================================== */
/*                          FONCTIONS UTILITAIRES                            */
/* ========================================================================== */

size_t  ft_strlen(char *s);
int     is_printable(char c);
int     min3(int a, int b, int c);

/* ========================================================================== */
/*                          FONCTIONS MÉMOIRE                                */
/* ========================================================================== */

void    free_map(t_map *map);

/* ========================================================================== */
/*                          FONCTIONS LECTURE                                */
/* ========================================================================== */

t_map   *read_file(t_map *map, char *file);
FILE    *open_input(char *file);
int     read_map_params(FILE *fp, t_map *map);
int     validate_map_params(t_map *map);
int     allocate_grid(t_map *map);
int     read_map_lines(FILE *fp, t_map *map);
int     validate_line(char *line, t_map *map, int i);
int     check_extra_lines(FILE *fp);

/* ========================================================================== */
/*                          FONCTIONS ALGORITHME                             */
/* ========================================================================== */

void    solve_bsq(t_map *map);
int   **create_dp_matrix(t_map *map);
void    compute_dp(t_map *map, int **dp, int *max_size, int *max_row, int *max_col);
void    mark_square(t_map *map, int max_size, int max_row, int max_col);
void    free_dp(int **dp, int rows);

/* ========================================================================== */
/*                          FONCTIONS I/O                                    */
/* ========================================================================== */

void    print_map(t_map *map);
void    process_input(t_map *map, char *file);

#endif

