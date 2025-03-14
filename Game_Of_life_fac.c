#include <stdio.h>
#include <stdlib.h>
#define file_name "D:\\vsc\\Game_Of_Life_Tasks_File.txt"         //introduceti adresa si numele fisierului (exemplu): "D:\\vsc\\Game_Of_Life_Tasks_File.txt"
typedef struct coord{
    int l;
    int c;
} coord;
typedef struct binary_tree{
    int depth;
    int passed;
    coord *cell;
    struct binary_tree* father;
    struct binary_tree *(son[2]);
}bTree;
void read_from_file(FILE* data_file, int *size_l, int *size_c, int *nr_iteratii, bTree tree_start)
{
    fscanf(data_file,"%d%d%d", size_l, size_c, nr_iteratii);
    fgetc(data_file);
    
    tree_start.cell= (coord*) malloc(sizeof(coord*));
    tree_start.depth=0;
    tree_start.father=NULL;
    tree_start.son[0]=NULL;
    tree_start.son[1]=NULL;
    char x;
    int k=0;
    for(int i=0; i<(*size_l); i++)
    {
        for(int j=0; j<(*size_c); j++)
        {
            fscanf(data_file, "%c", &x);
            
             
            if(x=='X') 
            {
                tree_start.cell[k].l=i;
                tree_start.cell[k].c=j;
                k++;
                tree_start.cell= (coord*) realloc(tree_start.cell,sizeof(coord*) * k);
                printf("%d %d\n", i, j);
            } 
        }
        fgetc(data_file);
    }
    tree_start.cell[k].l=0;
    tree_start.cell[k].c=0;
}
void task_cells(FILE* data_file, int size_l, int size_c, int nr_iteratii, bTree *tree_start)
{
    bTree *tree_current=tree_start, *next=NULL;
    char **board= (char**) malloc (size_l);
    int origin_passes=0, direction=0;           //direction=0 LEFT
                                                //direction=1 RIGHT
    for(int i=0;i<size_l; i++)
    {
        board[i]= (char*) malloc(size_c);
        for(int j=0; j<size_c; j++)
            board[i][j]='+';
    }

    for(int i=0; (*tree_start).cell[i].c!=0; i++)
        board
            [(*tree_start).cell[i].c]
            [(*tree_start).cell[i].l]
            ='X';
    while(origin_passes<3)        //gandeste o conditie, (nr_iteratii *tree)
    {
        if((*tree_current).father==NULL) origin_passes++;


        if((*tree_current).depth<nr_iteratii)
        {
            (*tree_current).son[direction] = (bTree*) malloc(sizeof(bTree));
            tree_current=(*tree_current).son[direction];
            direction=(direction+1)%2;

            

        }


        /* 
        
        if(direction==0) next=(*tree_current).left;
        if(direction==1) next=(*tree_current).right;

        if(next==NULL)
        {
            tree_current=(*tree_current).father;
            direction=1;
        } */

    }
    
    for(int i=0;i<size_l; i++)
        free(board[i]);
    free(board);
}
int main()
{
    FILE *data_file=fopen(file_name,"r");

    if(data_file==NULL){
        printf("File could not open");
        return 1;
    }

    bTree tree_start;

    int nr_tot_task;
    fscanf(data_file, "%d", &nr_tot_task);

    for(int i=0; i<nr_tot_task; i++){

        int size_l, size_c, nr_iteratii;
        read_from_file(data_file, &size_l, &size_c, &nr_iteratii, tree_start);
        task_cells(data_file, size_l, size_c, nr_iteratii, &tree_start);
    }
    
    fclose(data_file);
    return 0;
}