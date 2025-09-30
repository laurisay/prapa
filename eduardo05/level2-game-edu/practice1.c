#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int **cb(int w, int h)
{
    int **b = calloc(h,sizeof(int*)); // recordar el *
        for(int i = 0; i< h; i++)
            b[i] = calloc(w, sizeof(int));
        return b;
}

void fb(int **b, int h)
{
    for(int i = 0; i< h ; i++)
    {
        if(b[i])
            free(b[i]);
    }
    if(b)
        free(b);
}

void pb(int **b, int w, int h)
{
    for(int i = 0; i< h ; i++)
    {
        for(int j = 0; j<w;j++)
        {
            if(b[i][j])
                putchar('0');
            else
                putchar(' ');
        }
        putchar('\n');
    }

}

void ri(int **b, int w, int h)
{
    int x = 0;
    int y = 0;
    int draw = 0;
    char c;
    while(read(0,&c,1)>0)
    {
        if(c == 'x')
        {
            draw = !draw;
            if(draw)
                b[y][x] = 1; //Cuidado aca es y,x--> no x,y!
        }
        else if (c =='w')
        {
            y--;
            if(draw)
                b[y][x] = 1;
        }
        else if (c =='a')
        {
            x--;
            if(draw)
                b[y][x] = 1;
        }
        else if (c =='s')
        {
            y++;
            if(draw)
                b[y][x] = 1;
        }
        else if (c =='d')
        {
            x++;
            if(draw)
                b[y][x] = 1;
        }
    }
}


int cn(int **b, int w, int h, int x, int y)
{
    int count = 0;
    for(int i = -1; i<=1; i++ )
    {
        for(int j = -1; j<=1; j++)
        {
        if( (i||j) // si no esta en la posicion.
            && y + i >= 0 // Limites de los parametros.
            && y + i < h  //
            && x + j >= 0 //
            && x + j < w) //
            if(b[y+i][x+j])
            count++;
        }
    }
    return count;
}

int ** ub(int **b, int w, int h)
{
    int **new = cb(w,h);
    for(int i = 0; i<h ; i++ )
    {
        for(int j = 0; j<h ; j++)
        {
            int n = cn(b,w,h,i,j);
            if(b[i][j] && (n==2 || n==3))
                new[i][j] = 1;
            else if(!b[i][j] && n==3)
                new[i][j] = 1;
            else
                new[i][j] = 0;
        }
    }
    fb(b,h); // Recordar despues del update, liberar el mapa viejo.
    return new;
}

int main(int ac, char**av)
{
    if(ac!=4)
        return 1;
    int w = atoi(av[1]);
    int h = atoi(av[2]);
    int interations = atoi(av[3]);
    if (w<=0 || h<= 0 || interations <= 0)
        return 1;

    int **b = cb(w,h);
    ri(b,w,h);
    for(int i =0; i<interations; i++)
        b = ub(b,w,h);
    pb(b,w,h);
    fb(b,h);

    return 0;
}
