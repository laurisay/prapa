#include "bsq.h"

/* ========================================================================== */
/*                          FONCTIONS UTILITAIRES                             */
/* ========================================================================== */

size_t ft_strlen(char *s)
{
    size_t len = 0;
    while (s[len] && s[len] != '\n')
        len++;
    return len;
}

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

int min3(int a, int b, int c)
{
    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    return c;
}

/* ========================================================================== */
/*                          FONCTIONS MÉMOIRE                                 */
/* ========================================================================== */

void free_map(t_map *map)
{
    if (!map || !map->grid)
        return;
    for (int i = 0; i < map->rows; i++)
        free(map->grid[i]);
    free(map->grid);
    map->grid = NULL;
}

/* ========================================================================== */
/*                          SOUS-FONCTIONS DE READ_FILE                       */
/* ========================================================================== */

FILE *open_input(char *file)
{
    return (file ? fopen(file, "r") : stdin);
}

int read_map_params(FILE *fp, t_map *map)
{
    return (fscanf(fp, "%d %c %c %c\n", &map->rows, &map->empty,
                   &map->obstacle, &map->full) == 4);
}

int validate_map_params(t_map *map)
{
    return (map->rows > 0 &&
            map->empty != map->obstacle &&
            map->empty != map->full &&
            map->obstacle != map->full &&
            is_printable(map->empty) &&
            is_printable(map->obstacle) &&
            is_printable(map->full));
}

int allocate_grid(t_map *map)
{
    map->grid = calloc(map->rows, sizeof(char *));
    return (map->grid != NULL);
}

int validate_line(char *line, t_map *map, int i)
{
    int len = ft_strlen(line);
    if (i == 0)
        map->cols = len;
    else if (len != map->cols)
        return 0;
    for (int j = 0; j < len; j++)
        if (line[j] != map->empty && line[j] != map->obstacle)
            return 0;
    return 1;
}

int read_map_lines(FILE *fp, t_map *map)
{
    char *line = NULL;
    size_t len = 0;

    for (int i = 0; i < map->rows; i++) {
        if (getline(&line, &len, fp) == -1)
            return 0;
        if (!validate_line(line, map, i))
            return 0;
        map->grid[i] = line;
        line = NULL;
    }
    free(line);
    return 1;
}

int check_extra_lines(FILE *fp)
{
    char *line = NULL;
    size_t len = 0;
    int res = (getline(&line, &len, fp) == -1);
    free(line);
    return res;
}

/* ========================================================================== */
/*                          READ_FILE SIMPLIFIÉ                               */
/* ========================================================================== */

t_map *read_file(t_map *map, char *file)
{
    FILE *fp = open_input(file);
    if (!fp)
        return NULL;
    if (!read_map_params(fp, map) || !validate_map_params(map) || !allocate_grid(map))
        return fclose(fp), NULL;
    if (!read_map_lines(fp, map) || !check_extra_lines(fp))
        return fclose(fp), NULL;
    if (file) fclose(fp);
    return map;
}

/* ========================================================================== */
/*                          SOUS-FONCTIONS DE SOLVE_BSQ                       */
/* ========================================================================== */

int **create_dp_matrix(t_map *map)
{
    int **dp = calloc(map->rows, sizeof(int *));
    for (int i = 0; i < map->rows; i++)
        dp[i] = calloc(map->cols, sizeof(int));
    return dp;
}

void compute_dp(t_map *map, int **dp, int *max_size, int *max_row, int *max_col)
{
    for (int r = 0; r < map->rows; r++) {
        for (int c = 0; c < map->cols; c++) {
            if (map->grid[r][c] == map->empty) {
                dp[r][c] = (r == 0 || c == 0)
                    ? 1
                    : min3(dp[r-1][c], dp[r][c-1], dp[r-1][c-1]) + 1;
                if (dp[r][c] > *max_size) {
                    *max_size = dp[r][c];
                    *max_row = r;
                    *max_col = c;
                }
            }
        }
    }
}

void mark_square(t_map *map, int max_size, int max_row, int max_col)
{
    for (int r = max_row - max_size + 1; r <= max_row; r++)
        for (int c = max_col - max_size + 1; c <= max_col; c++)
            map->grid[r][c] = map->full;
}

void free_dp(int **dp, int rows)
{
    for (int i = 0; i < rows; i++)
        free(dp[i]);
    free(dp);
}

/* ========================================================================== */
/*                          SOLVE_BSQ SIMPLIFIÉ                               */
/* ========================================================================== */

void solve_bsq(t_map *map)
{
    int **dp;
    int max_size = 0, max_row = 0, max_col = 0;

    dp = create_dp_matrix(map);
    compute_dp(map, dp, &max_size, &max_row, &max_col);
    mark_square(map, max_size, max_row, max_col);
    free_dp(dp, map->rows);
}

/* ========================================================================== */
/*                          ENTRÉE / SORTIE                                   */
/* ========================================================================== */

void print_map(t_map *map)
{
    for (int i = 0; i < map->rows; i++)
        printf("%s", map->grid[i]);
}

void process_input(t_map *map, char *file)
{
    if (read_file(map, file))
    {
        solve_bsq(map);
        print_map(map);
    }
    else
        fprintf(stderr, "map error\n");
    free_map(map);
}

/* ========================================================================== */
/*                          MAIN                                              */
/* ========================================================================== */

int main(int argc, char **argv)
{
    t_map *map = calloc(1, sizeof(t_map));

    if (!map)
        return 1;
    if (argc < 2)
        process_input(map, NULL);
    else
        for (int i = 1; i < argc; i++) {
            if (i > 1) printf("\n");
            process_input(map, argv[i]);
        }
    free(map);
    return 0;
}

