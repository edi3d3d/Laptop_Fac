#include <stdio.h>
#include <stdlib.h>
#define file_name "D:\\vsc\\Game_Of_Life_Tasks_File.txt"         //introduceti adresa si numele fisierului (exemplu): "D:\\vsc\\Game_Of_Life_Tasks_File.txt"

typedef struct coord{
    int l;
    int c;
} coord;

typedef struct binary_tree{
    int depth;
    int passes;
    coord *cell;
    struct binary_tree *father;
    struct binary_tree *(son[2]);
}bTree;
void read_from_file(FILE* data_file, int *size_l, int *size_c, int *nr_iteratii, bTree *tree_start)
{
    fscanf(data_file,"%d%d%d", size_l, size_c, nr_iteratii);
    fgetc(data_file);
    
    (*tree_start).cell= (coord*) malloc(sizeof(coord));
    (*tree_start).depth=0;
    (*tree_start).passes=1;                                            //change the 0 to a 1 for task 2 so it takes the Right most direction at the start
    (*tree_start).father=NULL;
    (*tree_start).son[0]=NULL;
    (*tree_start).son[1]=NULL;
    char board_value;
    int k=0;
    for(int i=0; i<(*size_l); i++)
    {
        for(int j=0; j<(*size_c); j++)
        {
            fscanf(data_file, "%c", &board_value);
            
             
            if(board_value=='X') 
            {
                (*tree_start).cell[k].l=i;
                (*tree_start).cell[k].c=j;
                k++;
                (*tree_start).cell= (coord*) realloc((*tree_start).cell,sizeof(coord) * (k+1));
                //printf("%d %d\n", i, j);
            } 
        }
        fgetc(data_file);
    }
    (*tree_start).cell[k].l=-1;
    (*tree_start).cell[k].c=-1;
}
void task_cells(FILE* data_file, int size_l, int size_c, int nr_iteratii, bTree *tree_start)
{
    bTree *tree_current=tree_start;
    char **board= (char**) malloc (size_l * sizeof(char*));
    int origin_passes=0, direction=1;           //direction=0 LEFT              //make this 1 for task 2
                                                //direction=1 RIGHT
    for(int i=0;i<size_c; i++)
    {
        board[i]= (char*) malloc(size_c);
    }

    for(int i=0; i<size_c; i++)
    {
        for(int j=0; j<size_c; j++)
            board[i][j]=0;
    }
    
            
    for(int i=0; (*tree_start).cell[i].c!=-1; i++)
    {
        board
            [(*tree_start).cell[i].l]
            [(*tree_start).cell[i].c]
            =0b00010000;       //bit 4 is the cell state, bite 5-8 tell the number of neighbours
        //printf("%d %d\n", (*tree_start).cell[i].l, (*tree_start).cell[i].c);
    }
    
    for(int i=0;i<size_l; i++)
    {
        for(int j=0; j<size_c; j++)
        {
                int top=0, left=0, bottom=0, right=0, nr_neighbours=0;
                if(i-1>=0) top=1;
                if(j-1>=0) left=1;
                if(i+1<size_l) bottom=1;
                if(j+1<size_c) right=1;

            if(left && top)
                nr_neighbours+=(board[i-1][j-1]>>4);
                
            if(top)
                nr_neighbours+=(board[i-1][j+0]>>4);

            if(top && right)
                nr_neighbours+=(board[i-1][j+1]>>4);

            if(left)
                nr_neighbours+=(board[i+0][j-1]>>4);

            if(right)
                nr_neighbours+=(board[i+0][j+1]>>4);

            if(left && bottom)
                nr_neighbours+=(board[i+1][j-1]>>4);

            if(bottom)
                nr_neighbours+=(board[i+1][j+0]>>4);

            if(bottom && right)
                nr_neighbours+=(board[i+1][j+1]>>4);
            
            board[i][j]+=nr_neighbours;
        }
    }
    int aux=0;
    //printf("nod   dir   nod\n");    //nod_start, direction, new_node;   0 left, 1 right
    while(origin_passes<3)        
    {                                                                               //nr of pases <2 do in the node or terminal node, else leave
        int add=0;
        aux++;
        //printf("%d     ", (*tree_current).depth);
        if((*tree_current).father==NULL) origin_passes++;
        int last_direction=direction;

        if((*tree_current).depth<nr_iteratii && (*tree_current).passes<2)
        {
            direction=(*tree_current).passes;
            //printf("%d     ", direction);
            (*tree_current).son[direction] = (bTree*) malloc(sizeof(bTree));
            ((*tree_current).passes)+=1;                                            
            (*((*tree_current).son[direction])).father=tree_current;                //tree branch swapping, dont change, it took 2 hours for me to implement this logic 
            tree_current=(*tree_current).son[direction];                            
            (*tree_current).passes=1;                                               //for task 2 change the 0 to a 1 so it takes the Right most direction (normal rules)
            (*tree_current).depth=(*((*tree_current).father)).depth+1;
            (*tree_current).cell= (coord*) malloc(sizeof(coord));
            add=1;
        }
        else
        {
            //printf("      ");
            tree_current=(*tree_current).father;
            free((*tree_current).son[direction]);           //dont rewrite this, it works beautifully
            (*tree_current).son[direction]=NULL;            //i want to free the value after reaching its end/ all paths after it
        }

        //printf("%d \n", (*tree_current).depth);
        
        
        /* for(int i=0; i<size_l; i++)
        {
            for(int j=0; j<size_c; j++)
                printf("%d ", (board[i][j] & 0b00001111));
            
            printf("    ");

            for(int j=0; j<size_c; j++)
                printf("%d ", ((board[i][j] & 0b00010000) >> 4));

            printf("\n");
        } */

        int k=0;

        if(add==1)
        {
            for(int i=0; i<size_l; i++)
            {
                for(int j=0; j<size_c; j++)
                {
                    int top=0, left=0, bottom=0, right=0, state;
                    if(i-1>=0) top=1;
                    if(j-1>=0) left=1;
                    if(i+1<size_l) bottom=1;
                    if(j+1<size_c) right=1;


                    /* if(last_direction==0)
                    {

                    }
                    else  */
                    if(last_direction==1)
                    {
                        if( (board[i][j] & 0b00010000) >> 4 == 1 && ( (board[i][j] & 0b00001111) < 2 ||  (board[i][j] & 0b00001111) > 3) )    //daca are 0,1,4,5,6,7,8 vecini si celula este vie, atunci ea moare
                        {
                            printf("%d %d\n", i, j);
                            (*tree_current).cell[k].l=i;
                            (*tree_current).cell[k].c=j;
                            k++;
                            (*tree_current).cell = (coord*) realloc((*tree_current).cell, (k+1)*sizeof(coord));

                            state=-1; //new dead cell

                            //board[i][j]-=0b00010000; //cell type dead
                            
                            // add the changes in the tree.cell then update the board based on the values
                            // this will be the only one in this loop, the rest will be after based on a condition if a node was added or removed
                            // the removal shall be done before deleting the node so the added values can still be used to change the board to the previous node's configuration
                            
                            // if i go down, add the values to the board
                            // if i go up, remove the values from the board, the board will resemble the node's board
                                                    
                        }
                        else if( (board[i][j] & 0b00001111) == 3 && (board[i][j] & 0b00010000) >> 4 == 0)  //daca celula are 3 vecici si celula este moarta, atunci ea invie
                        {
                            (*tree_current).cell[k].l=i;
                            (*tree_current).cell[k].c=j;
                            k++;
                            (*tree_current).cell = (coord*) realloc((*tree_current).cell, (k+1)*sizeof(coord));

                            state=1; //new alive cell
                        }
                    }
                    if(top && left)
                        board[i-1][j-1]+=state;
                    if(top)
                        board[i-1][j+0]+=state;

                    if(top && right)
                        board[i-1][j+1]+=state;

                    if(left)
                        board[i+0][j-1]+=state;

                    if(right)
                        board[i+0][j+1]+=state;

                    if(bottom && left)
                        board[i+1][j-1]+=state;

                    if(bottom)
                        board[i+1][j+0]+=state;

                    if(bottom && right)
                        board[i+1][j+1]+=state;
                }
            }
            (*tree_current).cell[k].l=-1;
            (*tree_current).cell[k].c=-1;
        }

        //for(int i=0; i<k; i++)
        //    printf("%d %d\n", (*tree_current).cell[i].l, (*tree_current).cell[i].c);
        
        /* for(int i=0; i<size_l; i++)
        {
            for(int j=0; j<size_c; j++)
            {
                printf(board[i][j]>>4*('X' - '+')+'+');
            }
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

    for(int i = 0; i < nr_tot_task; i ++){

        int size_l, size_c, nr_iteratii;
        read_from_file(data_file, &size_l, &size_c, &nr_iteratii, &tree_start);
        task_cells(data_file, size_l, size_c, nr_iteratii, &tree_start);
    }
    
    fclose(data_file);
    return 0;
}