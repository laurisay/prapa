#include "bsq.h"

size_t ft_strlen(char *s){ size_t i=0; while(s[i]&&s[i]!='\n') i++; return i; }
int min3(int a,int b,int c){ return a<b?(a<c?a:c):(b<c?b:c); }

void free_map(t_map *map){
    if(!map->grid) return;
    for(int i=0;i<map->rows;i++) if(map->grid[i]) free(map->grid[i]);
    free(map->grid);
    map->grid = NULL;
}

t_map *read_file(t_map *map,char *file){
    FILE *fp=file?fopen(file,"r"):stdin;
    if(!fp) return NULL;
    if(fscanf(fp,"%d %c %c %c\n",&map->rows,&map->empty,&map->obstacle,&map->full)!=4){ if(file) fclose(fp); return NULL; }
    if(map->rows<=0 || map->empty==map->obstacle || map->empty==map->full || map->obstacle==map->full){ if(file) fclose(fp); return NULL;}
    map->grid = calloc(map->rows,sizeof(char*));
    if(!map->grid){ if(file) fclose(fp); return NULL; }

    for(int i=0;i<map->rows;i++){
        char *line=NULL; size_t len=0;
        if(getline(&line,&len,fp)==-1){
            for(int j=0;j<i;j++) free(map->grid[j]);
            free(map->grid);
            if(file) fclose(fp);
            return NULL;
        }
        int line_len = ft_strlen(line);
        if(line[line_len]=='\n') line[line_len]=0;
        if(i==0) map->cols=line_len; else if(line_len!=map->cols){ free(line); for(int j=0;j<i;j++) free(map->grid[j]); free(map->grid); if(file) fclose(fp); return NULL; }
        for(int j=0;j<map->cols;j++) if(line[j]!=map->empty && line[j]!=map->obstacle){ free(line); for(int k=0;k<i;k++) free(map->grid[k]); free(map->grid); if(file) fclose(fp); return NULL; }
        map->grid[i]=line;
    }
    if(file) fclose(fp);
    return map;
}

void solve_bsq(t_map *map){
    int **dp=calloc(map->rows,sizeof(int*)), max=0,rmax=0,cmax=0;
    for(int i=0;i<map->rows;i++) dp[i]=calloc(map->cols,sizeof(int));
    for(int r=0;r<map->rows;r++){
        for(int c=0;c<map->cols;c++){
            if(map->grid[r][c]==map->empty){
                dp[r][c]=(r==0||c==0)?1:1+min3(dp[r-1][c],dp[r][c-1],dp[r-1][c-1]);
                if(dp[r][c]>max){ max=dp[r][c]; rmax=r; cmax=c; }
            }
        }
    }
    for(int r=rmax-max+1;r<=rmax;r++) for(int c=cmax-max+1;c<=cmax;c++) map->grid[r][c]=map->full;
    for(int i=0;i<map->rows;i++){ free(dp[i]); }
    free(dp);
}

void print_map(t_map *map){ for(int i=0;i<map->rows;i++) printf("%s\n",map->grid[i]); }

void process_input(t_map *map,char *file){
    if(read_file(map,file)){ solve_bsq(map); print_map(map); }
    else fprintf(stderr,"map error\n");
    free_map(map);
}

int main(int argc,char **argv){
    t_map *map=calloc(1,sizeof(t_map));
    if(argc<2) process_input(map,NULL);
    else for(int i=1;i<argc;i++){ if(i>1) printf("\n"); process_input(map,argv[i]); }
    free(map);
    return 0;
}

