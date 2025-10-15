#ifndef BSQ_H
#define BSQ_H

#include <stdio.h>
#include <stdlib.h>

typedef struct s_map {
    int rows, cols;
    char empty, obstacle, full;
    char **grid;
} t_map;

size_t ft_strlen(char *s);
int min3(int a,int b,int c);
void free_map(t_map *map);
t_map *read_file(t_map *map,char *file);
void solve_bsq(t_map *map);
void print_map(t_map *map);
void process_input(t_map *map,char *file);

#endif

