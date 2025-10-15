#include "life.h"

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

