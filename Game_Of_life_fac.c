#include <stdio.h>
#include <stdlib.h>
#define file_name "D:\\vsc\\Game_Of_Life_Tasks_File.txt"         //introduceti adresa si numele fisierului (exemplu): "D:\\vsc\\Game_Of_Life_Tasks_File.txt"
typedef struct 
{
    int l;
    int c;
}cell;
typedef struct binary_tree
{
    int depth;
    cell *value;
    struct binary_tree* left_value;
    struct binary_tree* right_value;
};
void read_from_file(FILE* data_file, int *size_x, int *size_y, int *nr_iteratii, char **grid)
{
    fscanf(data_file,"%d%d%d", size_y, size_x, nr_iteratii);

    fgetc(data_file);
    (*size_x)++;       //se adauga caracterul \n la finalul fiecarui rand pentru a putea face printarea instanta
    
    *grid=(char*) malloc((*size_y)*(*size_x));

    for(int j=0; j<(*size_y); j++){
        fgets((*grid)+j*(*size_x), sizeof(*grid), data_file);
        (*grid)[(j+1)*(*size_x)]='\n';
    }
    (*grid)[(*size_y)*(*size_x)]='\0';        
    
}
void update_cells(FILE* data_file, int size_x, int size_y, int nr_iteratii, char *grid)
{
    char *aux_grid=NULL;
    aux_grid= (char*) malloc (size_y*size_x);
    int total_size=size_x*size_y;
    for(int i=1; i<=nr_iteratii; i++)
    {
        
        for(int j=1; j<=total_size; j++)
        {
            //update sim on the grid
            
        }
        fputs(grid, data_file);
        fprintf(data_file, "\n");
    }
    free(aux_grid);
}
int main()
{
    FILE *data_file=fopen(file_name,"r");

    if(data_file==NULL){
        printf("File could not open");
        return 1;
    }

    char *grid=NULL;
    int nr_tot_task;
    fscanf(data_file, "%d", &nr_tot_task);

    for(int i=0; i<nr_tot_task; i++){

        int size_x, size_y, nr_iteratii;
        read_from_file(data_file, &size_x, &size_y, &nr_iteratii, &grid);
        
        update_cells(data_file, size_x, size_y, nr_iteratii, grid);


        
    }


    free(grid);
    fclose(data_file);
    return 0;
}