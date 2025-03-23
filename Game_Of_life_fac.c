#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} stack;

/**
 * sort_list - sorts the list after finishing writing it for task 3 and ease of 4
 * @node: pointer to the current stack node
 */
void sort_list(stack* node)
{
    for(int i = 0; i < node->nr_cell ; i++)
        for(int j = 0; j < node->nr_cell; j++)  
            if(node->cell[i].l < node->cell[j].l || (node->cell[i].l == node->cell[j].l && node->cell[i].c < node->cell[j].c)){
                coord aux=node->cell[i];
                node->cell[i]=node->cell[j];
                node->cell[j]=aux;
            }
}

/**
 * add_in_list - adds @l and @c at the end value of the node, it's unsorted
 * @l: line coordonate of the cell
 * @c: column coordonate of the cell
 * @node: pointer to the current stack node
 */
void add_in_list(int l, int c, stack *node)
{
    node->cell = (coord*) realloc(node->cell, (node->nr_cell + 1) * sizeof(coord));
    node->cell[node->nr_cell].l = l;
    node->cell[node->nr_cell].c = c;

    (node->nr_cell)++;
}

/**
 * in_list - checks if a cell is in a certain generation's list of cell coordonates
 * @l: line coordonate of the cell
 * @c: column coordonate of the cell
 * @node: pointer to the current stack node
 *
 * returns: 1 if the list contains that cell, 0 otherwise
 */
int in_list(int l, int c, const stack *node)
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
 * 
 * returns: 1 if the sum is odd (cell alive), 0 otherwise;
 */
int is_alive(int l, int c, const stack *node , int generation)
{
    int value = 0;
    for(int i = 0; i <= generation && node != NULL ; i++, node = node->next)
        value += in_list(l, c, node);

    return value % 2;
}

/**
 * neighbour_count - counts the number of alive nrighbours of a certain cell in a certain generation
 * @l: line coordonate of the cell
 * @c: column coordonate of the cell
 * @size_l: number of lines in the grid
 * @size_c: number of columns in the grid
 * @node_start: pointer to the starting stack node
 * @generation: generation number towards to check
 * 
 * returns: number (0-8), the number of alive neighbours
 */
int neighbour_count(int l, int c, int size_l, int size_c, const stack *node_start, int generation)
{
    const char neighbours[8][2] = {
        {-1, -1}, {-1, 0}, {-1, +1},
        { 0, -1},          { 0, +1},
        {+1, -1}, {+1, 0}, {+1, +1}
    };
    int count = 0;

    for(int i = 0; i < 8; i++)
    {
        if(l + neighbours[i][0] < 0 || l + neighbours[i][0] == size_l  ||            // size_c - 1 because there is a surrounding thing around the board on the right side
           c + neighbours[i][1] < 0 || c + neighbours[i][1] == size_c)
            continue;
        count += is_alive(l + neighbours[i][0], c + neighbours[i][1], node_start, generation);
    }

    return count;
}

/**
 * printf_cells - prints the current state of the board based on the @task
 * @output_file: pointer towards the output file in which the writing will take pleace
 * @size_l: number of lines in the grid
 * @size_c: number of columns in the grid
 * @node_start: pointer to the starting stack node
 * @node: pointer to the current stack node
 * @task: value defining the current task
 * @generation: generation number towards to check
 *
 * if the task is 1 it prints the current board, except the generation 0 board
 * if the task is 2 it prints the generation and coordonates of each node in the stack, except for the starting generation 0 board
 * if the task if 3 it prints the current board for each node, similar to task 1
 * /if the task if 4 it doesn't anything yet, the deadline is far away :)
 */
void printf_cells(FILE* output_file, int size_l, int size_c, const stack *node_start, stack *node, int task, int generation)
{
    if(task == 1 || task == 3){
        for(int i = 0; i < size_l; i++){
            for(int j = 0; j < size_c; j++){
                if(is_alive(i, j, node_start, generation)) {
                    fprintf(output_file, "X");
                } else {
                    fprintf(output_file, "+");
                }
            }
            fprintf(output_file, "\n");
        }
        fprintf(output_file, "\n");
    }else if (task == 2 && generation != 0){
        fprintf(output_file, "%d", generation);
        sort_list(node);
        for(int i = 0; i < node->nr_cell; i++)
            fprintf(output_file, " %d %d", node->cell[i].l, node->cell[i].c);

        fprintf(output_file, "\n");
    }

}

/**
 * setup - gets the data from the DataInput file and creates the list for generation 0
 * @data_file: pointer to the file for the input data
 * @size_l: number of lines in the grid
 * @size_c: number of columns in the grid
 * @nr_iterations: total number of iterations
 * @node_start: pointer to the starting stack node
 * @task: the task number (1-4)
 */
void setup(FILE* data_file, int *size_l, int *size_c, int *nr_iterations, stack *node_start, int *task)
{
    if(fscanf(data_file,"%d%d%d%d", task, size_l, size_c, nr_iterations) != 4)
        exit(1);    //read data from file
    fgetc(data_file);

    node_start->cell = (coord*) malloc(sizeof(coord));
    node_start->nr_cell = 0;
    node_start->next = NULL;


    for(int i = 0; i < (*size_l); i++){
        for(int j = 0; j < (*size_c); j++){

            char board_value;
            if(fscanf(data_file, "%c", &board_value) != 1)
                exit(1);
            if(board_value == 'X')
                add_in_list(i, j, node_start);

        }
        fgetc(data_file);
    }
}

/**
 * update_cells - loops over every neighbour in the current @node's cells and calculates if it should be alive or dead the next generation
 * @size_l: number of lines in the grid
 * @size_c: number of columns in the grid
 * @node_start: pointer to the starting stack node
 * @node: pointer to the current stack node
 * @next_node: pointer to the next node after the current @node
 * @generation: generation number towards to check
 * @direction:  the direction of tree path, choosing what rules are applied
 */
void update_cells(int size_l, int size_c,const stack *node_start, stack *node, stack *next_node, int generation, int direction)
{
    char neighbours[9][2] = {
        {-1, -1}, {-1, 0}, {-1, +1},
        { 0, -1}, { 0, 0}, { 0, +1},
        {+1, -1}, {+1, 0}, {+1, +1}
    };

    next_node->cell = (coord*) malloc(sizeof(coord));
    next_node->next = NULL;
    next_node->nr_cell=0;


    for(int i = 0; i < node->nr_cell; i++){



        for(int j = 0; j < 9; j++)
        {
            int nl = node->cell[i].l + neighbours[j][0];
            int nc = node->cell[i].c + neighbours[j][1];

            if(nl < 0 || nl  == size_l ||
               nc  < 0 || nc  == size_c)
                continue;

            int n_count = neighbour_count(nl, nc, size_l, size_c, node_start, generation);
            int i_alive = is_alive(nl, nc, node_start, generation);

            if(in_list(nl, nc, next_node) == 1)
                continue;

            //printf("%d %d %d %d ", nl, nc, n_count, i_alive);

            if(direction == 0) {
                if(n_count == 2 && i_alive == 0)
                    add_in_list(nl, nc, next_node);

            } else if(direction == 1) {
                if(i_alive == 1 && (n_count < 2 || n_count > 3)) {
                    add_in_list(nl, nc, next_node);
                } else if(i_alive == 0 && n_count == 3) {
                    add_in_list(nl, nc, next_node);
                }
            }
            //printf("\n");
        }
    }
}

/**
 * task_cells - 
 * @output_file: pointer towards the output file in which the writing will take pleace
 * @size_l: number of lines in the grid
 * @size_c: number of columns in the grid
 * @nr_iterations: total number of iterations
 * @node_start: pointer to the starting stack node
 * @node: pointer to the current stack node
 * @generation: generation number towards to check
 * @task:  the task number (1-4)
 */
void task_cells(FILE* output_file, int size_l, int size_c, int nr_iterations, stack *node_start, stack *node, int task, int generation)
{
    printf_cells(output_file, size_l, size_c, node_start, node, task, generation);

    if(generation == nr_iterations)
        return ;

    if(task >= 3){
        node->next = (stack*) malloc(sizeof(stack));
        update_cells(size_l, size_c, node_start, node, node->next, generation, 0);
        task_cells(output_file, size_l, size_c, nr_iterations, node_start, node->next, task, generation+1);
        free(node->next);
    }

    node->next = (stack*) malloc(sizeof(stack));
    update_cells(size_l, size_c, node_start, node, node->next, generation, 1);
    task_cells(output_file, size_l, size_c, nr_iterations, node_start, node->next, task, generation+1);
    free(node->next);

}

int main(int argc, const char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
        FILE *input_file = fopen(argv[i], "r");
        
        char OutputFile[30];                    //example:
        strcpy(OutputFile+1, argv[i]);          // InputData/data1.in
        strcpy(OutputFile, "Out");              //OutputData/data1.in
        
        int len = strlen(OutputFile);           
        
        strcpy(OutputFile[len - 2], "out");     //OutputData/data1.out????
        OutputFile[len + 1] = '\0';             //OutputData/data1.out


        FILE *output_file = fopen(OutputFile, "w");

        if(input_file == NULL || output_file == NULL) {

            printf("File could not open");

            if(input_file == NULL) {
                fclose(output_file);
            } else if (output_file == NULL) {
                fclose(input_file);
            }

            return 1;
        }

        int task, size_l, size_c, nr_iterations, generation = 0;

        stack *node_start = (stack*) malloc(sizeof(stack));

        setup(input_file, &size_l, &size_c, &nr_iterations, node_start, &task);

        task_cells(output_file, size_l, size_c, nr_iterations, node_start, node_start, task, generation);

        free(node_start);
        fclose(input_file);
        fclose(output_file);
    }
    return 0;
}