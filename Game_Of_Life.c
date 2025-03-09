#include <stdio.h>
#include <stdlib.h>     //used for system("cls"), that will "delete" the previous terminal output so each frame will stutter less
#include <string.h>     //used to print all the cells faster so it stutters less each print_cells function call
#include <time.h>       //used for the time since start
#include <windows.h>    //used for milliseconds sleep on windows system (doesnt work on linux)
#include <conio.h>      //used for _kbhit and getch for key input
//formula so 0 is ' ' and 1 is 'O':
// value*('O'-' ')+' '
// if its 0 the first part will be 0+' ' = ' '
// if it 1 the first part will be 'O' - ' ' + ' ' = 'O'
// now i can use int8 
#define width  (800)        //800
#define height (500)        //500
#define steps 1             //nr of steps to do each simulation before displaying it 
void get_input(char cells[][width], int *total_cells, int *fps)       //receives input from the keyboard for adding cells, changing fps, it also pauses and stops the simulation
{
    cells[0][0] = ' ';
    *total_cells += 0;
    int key = 0, pause = 0;
    static int step = 0;
    if(step==1) printf("\nSTEP");
    do
    {
        for(int i=1;i<=100; i++)
        {
            if(_kbhit())
            {
                key=getch();
                if (key == 'q') exit(0);
                else if (key == ' ')
                {
                    if (step == 1)
                        step = 0;
                    else
                        pause=(pause+1)%2;

                    if (pause == 1) printf("\nPAUSED");
                }
                else if (key == 'k')
                {
                    step = 1;
                    return ;
                }
                else if (key == 'f')
                {
                    printf("\nfps=");
                    scanf("%d", fps);
                    while (*fps < 0)
                    {
                        printf("\nDont put a negative or 0 as a number, try again and try not to break this please. fps=");
                        scanf("%d", fps);
                    }
                    
                    key = 0;
                }
            }
        }
    } while ((key==' ' && pause == 1) || step == 1 || *fps==0);
}
void update_cells(char cells[][width], int *gen, int *total_cells, int time_start, int *current_time, int *fps)
{
    *total_cells=0;
    clock_t start=clock(), stop;
    for(int i=1;i<=steps;i++) 
    {
        char cells_step[height][width];
        for(int i=0; i<(height); i++)
            for(int j=0; j<(width); j++)
            {
                int nr_cells;
                int left=0, right=0, top=0, bottom=0;
                if(i-1>=0) top=1;
                if(j-1>=0) left=1;
                if(i+1<(height)) bottom=1;
                if(j+1<(width)) right=1;
                

                nr_cells=0;
                
                if(left && top)
                    nr_cells+=(cells[i-1][j-1]/'O');
                
                if(top)
                    nr_cells+=(cells[i-1][j+0]/'O');

                if(top && right)
                    nr_cells+=(cells[i-1][j+1]/'O');

                if(left)
                    nr_cells+=(cells[i+0][j-1]/'O');

                if(right)
                    nr_cells+=(cells[i+0][j+1]/'O');

                if(left && bottom)
                    nr_cells+=(cells[i+1][j-1]/'O');

                if(bottom)
                    nr_cells+=(cells[i+1][j+0]/'O');

                if(bottom && right)
                    nr_cells+=(cells[i+1][j+1]/'O');

                if(nr_cells<2) cells_step[i][j]=' ';
                

                else if(nr_cells==2) cells_step[i][j]=cells[i][j];
                else if(nr_cells==3) cells_step[i][j]='O';
                else if(nr_cells>3) cells_step[i][j]=' ';
            }
        
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
            {
                cells[i][j]=cells_step[i][j];
                if(cells[i][j]=='O') *total_cells=*total_cells+1;
            }

        *gen+=1;
    }
    stop=clock();
    *current_time=stop-time_start;
    
    int sleep_time;
    if(*fps!=0) sleep_time = 1000 / (*fps) - (stop - start);
    if (sleep_time > 0 && *fps!=0) Sleep(sleep_time);              //sleep for some time so the fps is more fluid and it doesnt run to quickly in the beginning
}
void print_cells(char cells[][width], int gen, int total_cells, int current_time, int fps)
{
    char printed_cells[((width+2)*(height+3))]={};
    int k=0;
    char top_left_corner=218;                 //print ┌
    char top_right_corner=191;                //print ┐
    char bottom_left_corner=192;              //print └
    char bottom_right_corner=217;             //print ┘
    char lr_edge=179;                         //print │
    char line_terminator='\n';                //new line
    char tb_edge=196;                         //print ─


    printed_cells[k++]=top_left_corner;       //print ┌

    for(int i=0; i<width; i++)      
        printed_cells[k++]=tb_edge;           //print ─

    printed_cells[k++]=top_right_corner;      //print ┐
    printed_cells[k++]=line_terminator;
    

    for(int i=0; i<(height); i++)
    {
        printed_cells[k++]=lr_edge;           //print │

        for(int j=0; j<(width); j++)
            printed_cells[k++]=cells[i][j];
        
            
        printed_cells[k++]=lr_edge;           //print │
        printed_cells[k++]=line_terminator;
    }
    
    printed_cells[k++]=bottom_left_corner;    //print └
    
    for(int i=0; i<width; i++)      
        printed_cells[k++]=tb_edge;           //print ─
    

    printed_cells[k++]=bottom_right_corner;   //print ┘
    printed_cells[k++]='\0';
    COORD coord = { 0 , 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    puts(printed_cells);
    
    printf("Gen: %d    Cells: %d    Time: %d    Fps: %d\n", gen, total_cells, current_time/1000, fps);
    printf("Press q to exit, k to step one frame, space to pause/resume and f to change the fps");
}
int main(void)
{
    const clock_t time_start=clock();
    
    char cells[height][width];
    int gen=0, total_cells=0, fps=0, current_time=0;

    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++)      //makes all the cells empty (' ')
            cells[i][j]=' ';
    
    //flyer
    
    cells[10][60]='O';
    cells[10][61]='O';
    cells[10][62]='O';
    cells[9][62]='O';
    cells[8][61]='O';
    
    //line
    
    for(int i=5; i<=width-5; i++)
        cells[(height/2)][i]='O';

        
    
    
    print_cells(cells, gen, total_cells, current_time, fps);
    
    while(1)    //infinite loop, it stops when the q key is hit
    {
        get_input(cells, &total_cells, &fps);
        update_cells(cells, &gen, &total_cells, time_start, &current_time, &fps);
        print_cells(cells, gen, total_cells, current_time, fps);
    }
    return 0;
}