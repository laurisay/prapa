#include "bsq.h"

size_t ft_strlen(char *s)
{
    size_t len = 0;

    while (s[len] != '\0' && s[len] != '\n')
        len++;
    return (len);
}

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

int min3(int a, int b, int c)
{
    if (a <= b && a <= c)
        return (a);
    if (b <= a && b <= c)
        return (b);
    return (c);
}

void free_map(t_map *map)
{
    int i;
    for (i = 0; i < map->rows; i++) {
        if (map->grid && map->grid[i])
            free(map->grid[i]);
    }
    if (map->grid) {
        free(map->grid);
        map->grid = NULL;
    }
}

t_map *read_file(t_map *map, char *file)
{
    FILE    *fp;
    char    *line;
    size_t  len;
    int     i, j, line_len;

    if(file)
        fp = fopen(file, "r")
    else
        pf = stdin;    
    if (!fp)
        return (NULL);
    if (fscanf(fp, "%d %c %c %c\n", &map->rows, &map->empty,
               &map->obstacle, &map->full) != 4) {
        if (file) fclose(fp);
        return (NULL);
    }
    if (map->rows <= 0 ||                              // Número de filas válido
        map->empty == map->obstacle ||                 // Caracteres diferentes
        map->empty == map->full ||
        map->obstacle == map->full ||
        !is_printable(map->empty) ||                   // Caracteres imprimibles
        !is_printable(map->obstacle) ||
        !is_printable(map->full)) {
        if (file) fclose(fp);
        return (NULL);
    }
    map->cols = 0;
    map->grid = calloc(map->rows, sizeof(char *));
    if (!map->grid) {
        if (file) fclose(fp);
        return (NULL);
    }
    for (i = 0; i < map->rows; i++) {
        line = NULL;
        len = 0;
        if (getline(&line, &len, fp) == -1) {
            if (file) fclose(fp);
            return (NULL);
        }
        line_len = ft_strlen(line);
        if (line[line_len] == '\n')
            line[line_len] = '\0';
        if (i == 0) 
        {
            map->cols = line_len;
        } 
        else if (line_len != map->cols) 
        {
            free(line);
            if (file) fclose(fp);
            return (NULL);
        }
        for (j = 0; j < map->cols; j++) 
        {
            if (line[j] != map->empty && line[j] != map->obstacle) {
                free(line);
                if (file) fclose(fp);
                return (NULL);
            }
        }
        map->grid[i] = line;
    }
    /* --- Verificar que no haya líneas extra --- */
    line = NULL;
    len = 0;
    if (getline(&line, &len, fp) != -1) {
        free(line);
        if (file) fclose(fp);
        return (NULL);
    }
    free(line);

    if (file) fclose(fp);
    return (map);
}

/* ========================================================================== */
/*                    SECCIÓN 4: ALGORITMO PRINCIPAL                         */
/* ========================================================================== */

/**
 * Encuentra y marca el cuadrado más grande en el mapa
 * Usa programación dinámica para eficiencia O(n*m)
 * @param map: estructura del mapa a resolver
 */
void solve_bsq(t_map *map)
{
    int **dp;           // Matriz de programación dinámica
    int i, r, c;
    int max_size = 0;   // Tamaño del cuadrado más grande
    int max_row = 0;    // Fila de la esquina inferior derecha
    int max_col = 0;    // Columna de la esquina inferior derecha

    /* --- Inicializar matriz DP --- */
    dp = calloc(map->rows, sizeof(int *));
    for (i = 0; i < map->rows; i++)
        dp[i] = calloc(map->cols, sizeof(int));
    for (r = 0; r < map->rows; r++) {
        for (c = 0; c < map->cols; c++) {
            if (map->grid[r][c] == map->empty) {
                if (r == 0 || c == 0) {
                    dp[r][c] = 1;
                }
                else {
                    dp[r][c] = min3(dp[r - 1][c],      // Arriba
                                   dp[r][c - 1],        // Izquierda
                                   dp[r - 1][c - 1])    // Diagonal
                              + 1;
                }
                if (dp[r][c] > max_size) {
                    max_size = dp[r][c];
                    max_row = r;
                    max_col = c;
                }
            }
        }
    }
    for (r = max_row - max_size + 1; r <= max_row; r++) {
        for (c = max_col - max_size + 1; c <= max_col; c++) {
            map->grid[r][c] = map->full;
        }
    }
    for (i = 0; i < map->rows; i++)
        free(dp[i]);
    free(dp);
}

/* ========================================================================== */
/*                    SECCIÓN 5: ENTRADA/SALIDA                              */
/* ========================================================================== */

/**
 * Imprime el mapa en stdout
 * @param map: mapa a imprimir
 */
void print_map(t_map *map)
{
    int i;

    if (!map || !map->grid)
        return;

    for (i = 0; i < map->rows; i++)
        fprintf(stdout, "%s\n", map->grid[i]);
}

/**
 * Procesa un archivo: lee, resuelve e imprime resultado
 * @param map: estructura para almacenar el mapa
 * @param file: archivo a procesar (NULL para stdin)
 */
void process_input(t_map *map, char *file)
{
    if (read_file(map, file) != NULL) {
        solve_bsq(map);
        print_map(map);
    }
    else {
        fprintf(stderr, "map error\n");
    }
    free_map(map);
}

/* ========================================================================== */
/*                         SECCIÓN 6: FUNCIÓN MAIN                           */
/* ========================================================================== */

/**
 * Función principal del programa
 * Maneja múltiples archivos o entrada por stdin
 */
int main(int argc, char **argv)
{
    t_map   *map;
    int     i;
    map = calloc(1, sizeof(t_map));
    if (!map)
        return (1);
    if (argc < 2) {
        process_input(map, NULL);
    }
    else {
        for (i = 1; i < argc; i++) {
            if (i > 1)
                fprintf(stdout, "\n");
            process_input(map, argv[i]);
        }
    }
    free(map);
    return (0);
}
