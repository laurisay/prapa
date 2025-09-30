#include "bsq.h"

/* ========================================================================== */
/*                        SECCIÓN 1: FUNCIONES UTILIDAD                      */
/* ========================================================================== */

/**
 * Calcula la longitud de una cadena hasta '\0' o '\n'
 * @param s: cadena a medir
 * @return: longitud de la cadena
 */
size_t ft_strlen(char *s)
{
    size_t len = 0;
    
    while (s[len] != '\0' && s[len] != '\n')
        len++;
    return (len);
}

/**
 * Verifica si un carácter es imprimible (ASCII 32-126)
 * @param c: carácter a verificar
 * @return: 1 si es imprimible, 0 si no
 */
int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

/**
 * Encuentra el mínimo entre tres números
 * @param a, b, c: números a comparar
 * @return: el menor de los tres
 */
int min3(int a, int b, int c)
{
    if (a <= b && a <= c)
        return (a);
    if (b <= a && b <= c)
        return (b);
    return (c);
}

/* ========================================================================== */
/*                    SECCIÓN 2: GESTIÓN DE MEMORIA                          */
/* ========================================================================== */

/**
 * Libera toda la memoria asignada al mapa
 * @param map: estructura del mapa a liberar
 */
void free_map(t_map *map)
{
    int i;
    
    // Liberar cada línea del grid
    for (i = 0; i < map->rows; i++) {
        if (map->grid && map->grid[i])
            free(map->grid[i]);
    }
    
    // Liberar el array de punteros
    if (map->grid) {
        free(map->grid);
        map->grid = NULL;
    }
}

/* ========================================================================== */
/*                    SECCIÓN 3: LECTURA Y VALIDACIÓN                        */
/* ========================================================================== */

/**
 * Lee y valida un mapa desde un archivo o stdin
 * @param map: estructura donde almacenar el mapa
 * @param file: nombre del archivo (NULL para stdin)
 * @return: puntero al mapa si es válido, NULL si hay error
 */
t_map *read_file(t_map *map, char *file)
{
    FILE    *fp;
    char    *line;
    size_t  len;
    int     i, j, line_len;
    
    // Abrir archivo o usar stdin
    fp = file ? fopen(file, "r") : stdin;
    if (!fp)
        return (NULL);
    
    /* --- Leer y validar primera línea (parámetros) --- */
    if (fscanf(fp, "%d %c %c %c\n", &map->rows, &map->empty, 
               &map->obstacle, &map->full) != 4) {
        if (file) fclose(fp);
        return (NULL);
    }
    
    // Validar parámetros
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
    
    /* --- Inicializar grid --- */
    map->cols = 0;
    map->grid = calloc(map->rows, sizeof(char *));
    if (!map->grid) {
        if (file) fclose(fp);
        return (NULL);
    }
    
    /* --- Leer y validar cada línea del mapa --- */
    for (i = 0; i < map->rows; i++) {
        line = NULL;
        len = 0;
        
        // Leer línea
        if (getline(&line, &len, fp) == -1) {
            if (file) fclose(fp);
            return (NULL);
        }
        
        // Procesar longitud de línea
        line_len = ft_strlen(line);
        if (line[line_len] == '\n')
            line[line_len] = '\0';
        
        // Validar longitud consistente
        if (i == 0) {
            map->cols = line_len;
        } else if (line_len != map->cols) {
            free(line);
            if (file) fclose(fp);
            return (NULL);
        }
        
        // Validar caracteres permitidos
        for (j = 0; j < map->cols; j++) {
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
    
    /* --- Algoritmo de Programación Dinámica --- */
    /*
     * dp[r][c] = tamaño del cuadrado más grande con esquina
     *            inferior derecha en la posición (r, c)
     * 
     * Fórmula:
     * - Si hay obstáculo: dp[r][c] = 0
     * - Si es borde (r=0 o c=0): dp[r][c] = 1 (si está vacío)
     * - Caso general: dp[r][c] = min(arriba, izq, diagonal) + 1
     */
    for (r = 0; r < map->rows; r++) {
        for (c = 0; c < map->cols; c++) {
            // Solo procesar celdas vacías
            if (map->grid[r][c] == map->empty) {
                // Caso base: bordes
                if (r == 0 || c == 0) {
                    dp[r][c] = 1;
                }
                // Caso recursivo
                else {
                    dp[r][c] = min3(dp[r - 1][c],      // Arriba
                                   dp[r][c - 1],        // Izquierda
                                   dp[r - 1][c - 1])    // Diagonal
                              + 1;
                }
                
                // Actualizar máximo (prioridad: arriba-izquierda)
                if (dp[r][c] > max_size) {
                    max_size = dp[r][c];
                    max_row = r;
                    max_col = c;
                }
            }
        }
    }
    
    /* --- Marcar el cuadrado solución en el mapa --- */
    /*
     * El cuadrado va desde:
     * - Fila: (max_row - max_size + 1) hasta max_row
     * - Columna: (max_col - max_size + 1) hasta max_col
     */
    for (r = max_row - max_size + 1; r <= max_row; r++) {
        for (c = max_col - max_size + 1; c <= max_col; c++) {
            map->grid[r][c] = map->full;
        }
    }
    
    /* --- Liberar memoria de la matriz DP --- */
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
    // Intentar leer y validar el mapa
    if (read_file(map, file) != NULL) {
        // Si es válido: resolver e imprimir
        solve_bsq(map);
        print_map(map);
    }
    else {
        // Si es inválido: mostrar error
        fprintf(stderr, "map error\n");
    }
    
    // Siempre liberar memoria
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
    
    // Asignar memoria para la estructura del mapa
    map = calloc(1, sizeof(t_map));
    if (!map)
        return (1);
    
    /* --- Sin argumentos: leer de stdin --- */
    if (argc < 2) {
        process_input(map, NULL);
    }
    /* --- Con argumentos: procesar cada archivo --- */
    else {
        for (i = 1; i < argc; i++) {
            // Separar soluciones con línea en blanco
            if (i > 1)
                fprintf(stdout, "\n");
            
            process_input(map, argv[i]);
        }
    }
    
    // Liberar estructura principal
    free(map);
    return (0);
}
