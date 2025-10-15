#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Crea un tablero de w columnas por h filas
// Aloca memoria dinámica e inicializa todo a 0 (células muertas)
// Retorna un puntero a puntero (matriz 2D)
int **create_board(int w, int h) {
    int **b = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++)
        b[i] = calloc(w, sizeof(int));
    return (b);
}

// Libera toda la memoria alocada para el tablero
// Primero libera cada fila individualmente, luego el array de punteros
void free_board(int **b, int h) {
    for (int i = 0; i < h; i++)
    {
        if (b[i])
            free(b[i]);
    }
    if (b)
        free(b);
}

// Imprime el tablero en la salida estándar
// Células vivas se muestran como '0', células muertas como espacio ' '
// Cada fila termina con un salto de línea
void print_board(int **b, int w, int h) {
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (b[i][j])
                putchar('0');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}

// Lee comandos del stdin para dibujar el patrón inicial
// Simula un "pen" que se mueve con WASD y se activa/desactiva con X
// w=arriba, a=izquierda, s=abajo, d=derecha, x=toggle pen
// Cuando el pen está activo (draw=1), marca las células como vivas
void read_input(int **b, int w, int h) {
    int x = 0, y = 0, draw = 0;
    char c;
    
    while (read(0, &c, 1) > 0)
    {
        if (c == 'x')
        {#include "life.h"

static int count(char **b, int w, int h, int y, int x)
{
	int n = 0, i, j;
	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++)
			if ((i || j) && y+i >= 0 && y+i < h && x+j >= 0 && x+j < w && b[y+i][x+j] == '0')
				n++;
	return n;
}

static void draw(char **b, int w, int h)
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
			putchar(b[y][x]);
		putchar('\n');
	}
}

int main(int ac, char **av)
{
	if (ac != 4)
		return (1);
	int w = atoi(av[1]), h = atoi(av[2]), it = atoi(av[3]);
	char **b = calloc(h, sizeof(char*));
	for (int i = 0; i < h; i++)
		b[i] = calloc(w, sizeof(char));

	int y = 0, x = 0, pen = 0;
	char c;
	while (read(0, &c, 1) > 0)
	{
		if (c == 'x') pen = !pen;
		else if (c == 'w' && y > 0) y--;
		else if (c == 's' && y < h-1) y++;
		else if (c == 'a' && x > 0) x--;
		else if (c == 'd' && x < w-1) x++;
		if (pen) b[y][x] = '0';
	}

	while (it--)
	{
		char **n = calloc(h, sizeof(char*));
		for (int i = 0; i < h; i++)
			n[i] = calloc(w, sizeof(char));
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
			{
				int c = count(b, w, h, y, x);
				if (b[y][x] == '0' && (c == 2 || c == 3))
					n[y][x] = '0';
				else if (b[y][x] != '0' && c == 3)
					n[y][x] = '0';
			}
		for (int i = 0; i < h; i++) { free(b[i]); b[i] = n[i]; }
		free(n);
	}
	draw(b, w, h);
	for (int i = 0; i < h; i++) free(b[i]);
	free(b);
	return 0;
}

            draw = !draw;
            if (draw)
                b[y][x] = 1;
        }
        else if (c == 'w' && y > 0)
        {
            y--;
            if (draw)
                b[y][x] = 1;
        }
        else if (c == 'a' && x > 0)
        {
            x--;
            if (draw)
                b[y][x] = 1;
        }
        else if (c == 's' && y < h - 1)
        {
            y++;
            if (draw)
                b[y][x] = 1;
        }
        else if (c == 'd' && x < w - 1)
        {
            x++;
            if (draw)
                b[y][x] = 1;
        }
    }
}

// Cuenta cuántos vecinos vivos tiene una célula en posición (x,y)
// Revisa las 8 células adyacentes (arriba, abajo, lados y diagonales)
// No cuenta la célula misma, solo sus vecinos
// Células fuera del tablero se consideran muertas
int count_neighbors(int **b, int h, int w, int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if ((i || j) && y + i >= 0 && y + i < h && x + j >= 0 && x + j < w)  //No contar la celda donde esta, y solo alrededor.
                if (b[y + i][x + j])
                    count++;
        }
    }
    return (count);
}

// Calcula la siguiente generación del tablero según las reglas del Game of Life
// Célula viva con 2-3 vecinos sobrevive, con menos o más muere
// Célula muerta con exactamente 3 vecinos nace
// Crea un nuevo tablero, aplica las reglas, libera el viejo y retorna el nuevo
int **update_board(int **b, int w, int h) {
    int **new = create_board(w, h);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int n = count_neighbors(b, h, w, y, x);
            if (b[y][x] && (n == 2 || n == 3))
                new[y][x] = 1;
            else if (!b[y][x] && n == 3)
                new[y][x] = 1;
            else
                new[y][x] = 0;
        }
    }
    free_board(b, h);
    return (new);
}

// Función principal: valida argumentos, crea tablero, lee patrón inicial,
// ejecuta las iteraciones del Game of Life y muestra el resultado final
// Argumentos: ./life ancho alto iteraciones
int main(int ac, char **av) {
    if (ac != 4)
        return 1;
    
    int width = atoi(av[1]);
    int height = atoi(av[2]);
    int iterations = atoi(av[3]);
    
    if (width <= 0 || height <= 0 || iterations < 0)
        return 1;
    
    int **board = create_board(width, height);
    read_input(board, width, height);
    
    for (int i = 0; i < iterations; i++)
    {
        board = update_board(board, width, height);
    }
    
    print_board(board, width, height);
    free_board(board, height);
    return 0;
}

