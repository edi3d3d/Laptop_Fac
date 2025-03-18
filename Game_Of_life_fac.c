#include <stdio.h>
#include <stdlib.h>
#define file_name "D:\\vsc\\Game_Of_Life_Tasks_File.txt"         //introduceti adresa si numele fisierului (exemplu): "D:\\vsc\\Game_Of_Life_Tasks_File.txt"
typedef struct coord{
    int l;
    int c;
} coord;
typedef struct binary_tree{
    coord *cell;
    struct binary_tree *(son[2]);
}bTree;
void setup(FILE* data_file, int *size_l, int *size_c, int *nr_iteratii, bTree *tree_start, int *task, char ***board)
{
    fscanf(data_file,"%d%d%d%d", task, size_l, size_c, nr_iteratii);    //read data from file
    fgetc(data_file);

    (*board) = (char**) malloc ((*size_l) * sizeof(char*));             //initiate the board with 0
    for(int i=0;i<(*size_c); i++)                                       //the same board will be used by each generation
    {                                                                   //it will be used as the board to calculate the respective rule
        (*board)[i]= (char*) malloc((*size_c));                         //based on the position in the B. tree
        for(int j=0; j<(*size_c); j++)
            (*board)[i][j]=0;
    }
    
    (*tree_start).cell= (coord*) malloc(sizeof(coord));
    (*tree_start).son[0]=NULL;
    (*tree_start).son[1]=NULL;
    
    int k=0;
    for(int i=0; i<(*size_l); i++)
    {
        for(int j=0; j<(*size_c); j++)
        {
            char board_value;
            fscanf(data_file, "%c", &board_value);                  //record the values of X and add the coordonates in the 
            if(board_value=='X')                                    //cell array of each node of the tree/list
            {
                (*tree_start).cell[k].l=i;
                (*tree_start).cell[k].c=j;
                k++;
                (*tree_start).cell= (coord*) realloc((*tree_start).cell,sizeof(coord) * (k+1));
            } 
        }
        fgetc(data_file);
    }
    (*tree_start).cell[k].l=-1;
    (*tree_start).cell[k].c=-1;

    for(int i=0; (*tree_start).cell[i].c!=-1; i++)
    {
        (*board)
            [(*tree_start).cell[i].l]                       //each coordonate gets added to the array as 0b00010000 or 16.
            [(*tree_start).cell[i].c]                       //it gets added on the same coordonates in the matrix as the array coordonates
            =0b00010000;                                    //bit 4 is the cell state, bite 5-8 tell the number of neighbours  
    }

    for(int i=0;i<(*size_l); i++)
    {
        for(int j=0; j<(*size_c); j++)
        {
            int neighbours[8][2]={
                {-1, -1}, {-1, 0}, {-1, +1},
                { 0, -1},          { 0, +1},
                {+1, -1}, {+1, 0}, {+1, +1}
                };
            int nr_neighbours=0;
            
            for(int k=0; k<8; k++)
                if( ( i + neighbours[k][0] ) >= 0 &&
                    ( i + neighbours[k][0] ) < (*size_l) &&
                    ( j + neighbours[k][1] ) >= 0 &&
                    ( j + neighbours[k][1] ) < (*size_c))
                    nr_neighbours+=((*board)[i + neighbours[k][0]][j + neighbours[k][1]] >> 4);
            
            (*board)[i][j]+=nr_neighbours;                            //the sum gets added to the cell
        }                                                             //it cannot be bigger then 8 so it uses 4 out of the value's 8 bits
    }
}
void afis(FILE* data_file, int size_l, int size_c, bTree *tree_node, int task, char **board, int depth)
{
    if((task==1 && depth>=0) || task==3)
    {
        for(int i=0; i<size_l; i++)
        {
            for(int j=0; j<size_c; j++)
            printf("%c ", (board[i][j] >> 4) * ('X' - '+')+'+');
            printf("\n");
        }
        printf("\n");
    }
    else if(task==2)
    {
        printf("%d ", depth);
        for(int i=0; (*tree_node).cell[i].c != -1; i++)
            printf("%d %d ", (*tree_node).cell[i].l, (*tree_node).cell[i].c);
        printf("\n");
    }
    else if(task==4)
    {
        exit(0); //hamiltonian, idk
    }
}
void node_processing (int size_l, int size_c, bTree *tree_node, char **board, char direction)
{
    int k=0;
    (*tree_node).cell= (coord*) malloc (sizeof(coord));  
    for(int i=0; i<size_l; i++)                     //update node cells pointer of the current node
    {
        for(int j=0; j<size_c; j++)
        {
            if(direction==0)
            {

            }
            else  
            if(direction==1)
            {
                if( (board[i][j] & 0b00010000) >> 4 == 1 && ( (board[i][j] & 0b00001111) < 2 ||  (board[i][j] & 0b00001111) > 3) )    //daca are 0,1,4,5,6,7,8 vecini si celula este vie, atunci ea moare
                {
                    //printf("%d %d  ", i, j);
                    //printf("%d %d\n", (board[i][j] & 0b00010000), (board[i][j] & 0b00001111) );
                    (*tree_node).cell[k].l=i;
                    (*tree_node).cell[k].c=j;
                    k++;
                    (*tree_node).cell = (coord*) realloc((*tree_node).cell, (k+1)*sizeof(coord));

                    //board[i][j]-=0b00010000; //cell type dead
                    
                    // add the changes in the tree.cell then update the board based on the values
                    // this will be the only one in this loop, the rest will be after based on a condition if a node was added or removed
                    // the removal shall be done before deleting the node so the added values can still be used to change the board to the previous node's configuration
                    
                    // if i go down, add the values to the board
                    // if i go up, remove the values from the board, the board will resemble the node's board
                                            
                }
                else if( (board[i][j] & 0b00001111) == 3 && (board[i][j] & 0b00010000) >> 4 == 0)  //daca celula are 3 vecici si celula este moarta, atunci ea invie
                {
                    (*tree_node).cell[k].l=i;
                    (*tree_node).cell[k].c=j;
                    k++;
                    (*tree_node).cell = (coord*) realloc((*tree_node).cell, (k+1)*sizeof(coord));
                }
            }
        }
    }
    
    (*tree_node).cell[k].c=-1;
    (*tree_node).cell[k].l=-1;
    for(int i=0; (*tree_node).cell[i].l!=-1; i++)
    {
        int neighbours[8][2]={
            {-1, -1}, {-1, 0}, {-1, +1},
            { 0, -1},          { 0, +1},
            {+1, -1}, {+1, 0}, {+1, +1}
        };
        
        int cell_l=(*tree_node).cell[i].l;
        int cell_c=(*tree_node).cell[i].c;
        int state;
        if( ((board[cell_l][cell_c] & 0b00010000) >> 4 ) == 1 ) state = -1;
        else state = 1;
        
        board[cell_l][cell_c]+=state*16; //16 being the 5th bit from right to left
        
        for(int j=0; j<8; j++)
            if( ( cell_l + neighbours[j][0] ) >= 0 &&
                ( cell_l + neighbours[j][0] ) < size_l &&
                ( cell_c + neighbours[j][1] ) >= 0 &&
                ( cell_c + neighbours[j][1] ) < size_c)
                board[ cell_l + neighbours[j][0] ][ cell_c + neighbours[j][1] ] += state;
                
                    
    }
}
void task_cells(FILE* data_file, int size_l, int size_c, int nr_iteratii, bTree *tree_node, int task, char **board, int depth)
{
    afis(data_file, size_l, size_c, tree_node, task, board, depth);
    if(depth == nr_iteratii) return ;
    if(task==3)
    {
        (*tree_node).son[0]= (bTree*) malloc(sizeof(bTree));
        node_processing(size_l, size_c, (*tree_node).son[0], board, 0);
        task_cells(data_file, size_l, size_c, nr_iteratii, (*tree_node).son[0], task, board, depth+1);
        //deupdate matrix
        for(int i=0; (*((*tree_node).son[0])).cell[i].c != -1; i++)
        {
            int neighbours[8][2]={
                {-1, -1}, {-1, 0}, {-1, +1},
                { 0, -1},          { 0, +1},
                {+1, -1}, {+1, 0}, {+1, +1}
            };
            int cell_l=(*((*tree_node).son[0])).cell[i].l;
            int cell_c=(*((*tree_node).son[0])).cell[i].c;
            int state;
            if( ((board [cell_l] [cell_c] & 0b00010000) >> 4 ) == 1 ) state = -1;
            else state = 1;

            board[cell_l][cell_c]+=state*16; //16 being the 5th bit from right to left
        
            for(int j=0; j<8; j++)
                if(
                    cell_l>=0 &&
                    cell_l<size_l &&
                    cell_c>=0 &&
                    cell_c<size_c)
                    board[ cell_l + neighbours[i][0] ][ cell_c + neighbours[i][1] ] += state;
        }

        free((*(*tree_node).son[0]).cell);
        free((*tree_node).son[0]);
    }

    (*tree_node).son[1]= (bTree*) malloc(sizeof(bTree));
        node_processing(size_l, size_c, (*tree_node).son[1], board, 1);
        task_cells(data_file, size_l, size_c, nr_iteratii, (*tree_node).son[1], task, board, depth+1);
        //deupdate matrix
        for(int i=0; (*((*tree_node).son[1])).cell[i].c != -1; i++)
        {
            int neighbours[8][2]={
                {-1, -1}, {-1, 0}, {-1, +1},
                { 0, -1},          { 0, +1},
                {+1, -1}, {+1, 0}, {+1, +1}
            };
            int cell_l=(*((*tree_node).son[1])).cell[i].l;
            int cell_c=(*((*tree_node).son[1])).cell[i].c;
            int state;
            if( ((board [cell_l] [cell_c] & 0b00010000) >> 4 ) == 1 ) state = -1;
            else state = 1;

            board[cell_l][cell_c]+=state*16; //16 being the 5th bit from right to left
        
            for(int j=0; j<8; j++)
                if(
                    cell_l>=0 &&
                    cell_l<size_l &&
                    cell_c>=0 &&
                    cell_c<size_c)
                    board[ cell_l + neighbours[i][0] ][ cell_c + neighbours[i][1] ] += state;
        }

        free((*(*tree_node).son[1]).cell);
        free((*tree_node).son[1]);
    
    free(tree_node);
}
int main()
{
    FILE *data_file=fopen(file_name,"r");

    if(data_file==NULL){
        printf("File could not open");
        return 1;
    }

    bTree *tree_start=(bTree*) malloc(sizeof(bTree));

    int task, size_l, size_c, nr_iteratii, depth=0;
    char **board;

    setup(data_file, &size_l, &size_c, &nr_iteratii, tree_start, &task, &board);

    task_cells(data_file, size_l, size_c, nr_iteratii, tree_start, task, board, depth);
    
    fclose(data_file);
    return 0;
}