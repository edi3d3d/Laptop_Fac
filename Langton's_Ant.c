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
#define screen_multiplier 1                  //7     multiplier for bigger/smaller dimensions
#define width  (150*screen_multiplier)       //208   width 10px                             100
#define height (150*screen_multiplier)        //75    height 24px per h=2.4w                 42 //one small screen square
#define steps 10                              //nr of steps to do each simulation before displaying it 
#define R 3
#define L 1
void get_input(int *fps)       //receives input from the keyboard for adding cells, changing fps, it also pauses and stops the simulation
{
    int key = 0, pause = 0;
    static int step = 0;
    if(step==1) printf("   STEP");
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
void update_ant(char cells[][width], int *ant_x, int *ant_y, int *ant_direction, int *gen, int time_start, int *current_time, int *fps)
{
    clock_t start=clock(), stop;

    for(int k=1;k<=steps;k++)
    {
        int ant_state, nr_states=2, turn;

        char states[12]        ={' ','0','1','2','3','4','5','6','7','8','9','@'};
        int states_rotation[12]={ R , L , R , R , R , L , R , L , R , L , L , R };      //CHANGE THE NUMBERS INTO R OR L FOR THAT RESPECTIVE STEP TO TURN RIGHT OR LEFT

        for(int i=0; i<10; i++)
            if(cells[*ant_y][*ant_x] == states[i]) ant_state=i;
        
        turn=states_rotation[ant_state];

        *ant_direction=(*ant_direction+turn)%4;                 //rotate ant
        

        

        cells[*ant_y][*ant_x]=states[(ant_state+1)%nr_states];  

        
        if(*ant_direction==0) (*ant_x)+=1;                  //move ant
        else if(*ant_direction==1) (*ant_y)-=1;
        else if(*ant_direction==2) (*ant_x)-=1;
        else if(*ant_direction==3) (*ant_y)+=1;
        printf("Direction: %d, Position: (%d, %d)\n", *ant_direction, *ant_x, *ant_y);

        

        if(*ant_x>=width)                                   //check out of bounds
        {
            printf("Ant out of bounds (right)\n");
            exit(1);
        } else if(*ant_y<0)
        {
            printf("Ant out of bounds (up)\n");
            exit(1);
        } else if(*ant_x<0)
        {
            printf("Ant out of bounds (left)\n");
            exit(1);
        } else if(*ant_y>=height)
        {
            printf("Ant out of bounds (down)\n");
            exit(1);
        }
        
        *gen+=1;
    }
    stop=clock();
    *current_time=stop-time_start;
    
    int sleep_time;
    if(*fps!=0) sleep_time = 1000 / (*fps) - (stop - start);
    if (sleep_time > 0 && *fps!=0) Sleep(sleep_time);              //sleep for some time so the fps is more fluid and it doesnt run to quickly in the beginning
}
void print_cells(char cells[][width], int ant_x, int ant_y, int gen, int current_time, int fps)
{
    char printed_cells[((width+2)*(height+3))]={};
    int k=0;
    //printf("---------------------------\n");
    for(int i=0; i<(height); i++)
    {
        for(int j=0; j<(width); j++)
            printed_cells[k++]=cells[i][j];
        printed_cells[k++]='\n';
    }

    printed_cells[ant_x+ant_y*(width+1)]='A';

    printed_cells[k++]='\0';

    //system("cls");

    puts(printed_cells);
    
    printf("Gen: %d    Time: %d    Max Fps: %d\nPress q to exit, k to step one frame, space to pause/resume and f to change the fps", gen, current_time/1000, fps);
}
int main(void)
{
    const clock_t time_start=clock();
    
    char cells[height][width];
    int gen=0, fps=0, current_time=0;
    int ant_x=(width)/2, ant_y=(height)/2, ant_direction=2; //starting orientation in order: Right, Up, Left, Down 0 1 2 3

    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++)      //makes all the cells empty (' ')
            cells[i][j]=' ';
    
    
    
    print_cells(cells, ant_x, ant_y, gen, current_time, fps);
    
    while(1)    //infinite loop, it stops when the q key is hit
    {
        get_input(&fps);
        update_ant(cells, &ant_x, &ant_y, &ant_direction, &gen, time_start, &current_time, &fps);
        print_cells(cells, ant_x, ant_y, gen, current_time, fps);
    }
    return 0;
}