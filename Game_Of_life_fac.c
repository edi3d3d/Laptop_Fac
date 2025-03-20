#include <stdio.h>
#include <stdlib.h>
#define file_input_name "D:\\vsc\\Game_Of_Life_Tasks_File.txt"         //introduceti adresa si numele fisierului (exemplu): "D:\\vsc\\Game_Of_Life_Tasks_File.txt"
#define file_output_name "D:\\vsc\\Game_Of_Life_Output_File.txt"       //introduceti adresa si numele fisierului in care sa se scrie outputul

/**
 * struct coord - represents a cell coordinate in the grid.
 * @l: row index.
 * @c: column index.
 */
typedef struct coord{
    int l;
    int c;
} coord;

/**
 * struct stack - represents a stack node for storing changed cells on each generation
 * @cell: pointer to an dynamic array of cell coordonates
 * @nr_cell: number of changed cells present in the @cell array
 * @next: pointer to the next generation's stack node
 */
typedef struct stack{
    coord *cell;
    int nr_cell;
    struct stack *next;
}stack;

/**
 * in_list - checks if a cell is inside a certain generation's list of cell coordonates
 * @l: line coordonate of the cell
 * @c: column coordonate of the cell
 * @node: pointer to the respective stack node
 * 
 * returns: 1 if the list contains that cell, 0 otherwise
 */
int in_list(int l, int c, stack *node)
{
    for(int i = 0; i < node->nr_cell; i++)
        if(node->cell[i].l == l && node->cell[i].c == c)
            return 1;

    return 0;
}
/**
 * is_alive - checks if a certain coordonate's cell is alive or not by counting the number of changes
 * @l: line coordonate of the cell
 * @c: column coordonate of the cell
 * @node: pointer to the starting stack node
 * @generation: generation number towards to check
 * returns: 1 if the sum is odd (cell alive), 0 otherwise;
 */
int is_alive(int l, int c, stack *node , int generation)
{
    int value = 0;
    for(int i = 0; i <= generation && node != NULL ; i++, node = node->next)
        value += in_list(l, c, node);
        
    return value % 2;
}

/**
 * neighbou_count - counts the number of alive nrighbours of a certain cell in a certain generation
 * @l: line coordonate of the cell
 * @c: column coordonate of the cell
 * @start_node: pointer to the starting stack node
 * @generation: generation number towards to check
 * returns: number (0-8), the number of alive neighbours
 */
int neighbour_count(int l, int c, stack *start_node, int generation)
{
    char neighbours[8][2] = {
        {-1, -1}, {-1, 0}, {-1, +1},
        { 0, -1},          { 0, +1},
        {+1, -1}, {+1, 0}, {+1, +1}
    };
    int count = 0;

    for(int i = 0; i < 8; i++)
        count += is_alive(l + neighbours[i][0], c + neighbours[i][1], start_node, generation);

    return count;
}
/**
 * printf_cells - prints the current state of the board based on the @task
 * @output_file: pointer towards the output file in which the writing will take pleace
 * @size_l: number of lines in the grid
 * @size_c: number of columns in the grid
 * @start_node: pointer to the starting stack node
 * @task: value defining the current task
 * @generation: generation number towards to check
 * 
 * /if the task is 1 it prints the current board, except the generation 0 board
 * /if the task is 2 it prints the generation and coordonates of each node in the stack
 * if the task if 3 it prints the current board for each node, similar to task 1
 * /if the task if 4 it doesn't anything yet, the deadline is far away :)
 */
void printf_cells(FILE* output_file, int size_l, int size_c, stack *start_node, int task, int generation)
{
    for(int i = 0; i < size_l; i++){
        for(int j = 0; j < size_c; j++){

            if(is_alive(i, j, start_node, generation)) {
                fprintf(output_file, "X");
            } else {
                fprintf(output_file, "+");
            }
        }
        fprintf(output_file, "\n");
    }
}

/**
 * 
 */
void setup(FILE* data_file, int *size_l, int *size_c, int *nr_iteratii, stack *node_start, int *task)
{
    fscanf(data_file,"%d%d%d%d", task, size_l, size_c, nr_iteratii);    //read data from file
    fgetc(data_file);

    node_start->cell = (coord*) malloc(sizeof(coord));
    node_start->nr_cell = 0;
    node_start->next = NULL;
    
    for(int i = 0; i < (*size_l); i++){
        for(int j = 0; j < (*size_c); j++){
            
            char board_value;
            fscanf(data_file, "%c", &board_value);
            if(board_value == 'X'){

                int index = node_start->nr_cell;
                node_start->nr_cell++;
                node_start->cell = (coord*) realloc(node_start->cell, sizeof(coord) * (node_start->nr_cell));

                node_start->cell[index].l = i;
                node_start->cell[index].c = j;
            } 
        }
        fgetc(data_file);
    }
}
/*void node_processing (int size_l, int size_c, bTree *tree_node, char **board, char direction)
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
void task_cells(FILE* output, int size_l, int size_c, int nr_iteratii, bTree *tree_node, int task, int depth)
{
    afis(output, size_l, size_c, tree_node, task, board, depth);
    if(depth == nr_iteratii) return ;
    if(task==3)
    {
        (*tree_node).son[0]= (bTree*) malloc(sizeof(bTree));
        node_processing(size_l, size_c, (*tree_node).son[0], board, 0);
        task_cells(output, size_l, size_c, nr_iteratii, (*tree_node).son[0], task, board, depth+1);
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
    }

    (*tree_node).son[1]= (bTree*) malloc(sizeof(bTree));
    node_processing(size_l, size_c, (*tree_node).son[1], board, 1);
    task_cells(output, size_l, size_c, nr_iteratii, (*tree_node).son[1], task, board, depth+1);
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
                board[ cell_l + neighbours[j][0] ][ cell_c + neighbours[j][1] ] += state;
    }
    
    free((*tree_node).cell);
    free(tree_node);
}
*/

/**
 * 
 */
void task_cells(FILE* output_file, int size_l, int size_c, int nr_iteratii, stack *start_node, stack *node, int task, int generation)
{
    printf_cells(output_file, size_l, size_c, start_node, task, generation);
    
    if(generation == nr_iteratii)
        return ;

    node->next = (stack*) malloc(sizeof(stack));
    
    
    free(node->next);



    //print_cells of the whole matrix
    //create new node
    //update the new node and add it to the next node if it isnt already in it
    //
}
int main()
{
    FILE *input_file = fopen(file_input_name, "r");
    FILE *output_file = fopen(file_output_name, "w");

    if(input_file == NULL || output_file == NULL) {
        
        printf("File could not open");

        if(input_file == NULL) {
            fclose(output_file);
        } else if (output_file == NULL) {
            fclose(input_file);
        }

        return 1;
    }

    stack *node_start=(stack*) malloc(sizeof(stack));

    int task, size_l, size_c, nr_iteratii, generation=0;

    setup(input_file, &size_l, &size_c, &nr_iteratii, node_start, &task);

    task_cells(output_file, size_l, size_c, nr_iteratii, node_start, node_start, task, generation);

    free(node_start);
    fclose(input_file);
    fclose(output_file);
    return 0;
}