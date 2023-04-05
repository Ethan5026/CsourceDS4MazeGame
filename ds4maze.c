
//./ds4rd.exe -d 054c:05c4 -D DS4_USB -t -g | ./lab8_2 30

#include <stdio.h>
#include <math.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


/* Mathmatical constants */
#define PI 3.14159

/* 	Screen geometry
    Use ROWS and COLUMNS for the screen height and width (set by system)
    MAXIMUMS */
#define COLUMNS 100
#define ROWS 80

/* 	Character definitions taken from the ASCII table */
#define AVATAR 'A'
#define WALL '*'
#define EMPTY_SPACE ' '

/*	Number of samples taken to form an moving average for the gyroscope data
    Feel free to tweak this. */
#define NUM_SAMPLES 10
#define MAXPOINTS 10000



/* 2D character array which the maze is mapped into */
char MAZE[COLUMNS][ROWS];
int MEMORY[100];



void generate_maze(int difficulty);

void draw_maze(void);

void draw_character(int x, int y, char use);

double calc_roll(double mag);

double m_avg(double buffer[], int avg_size, double new_item);


void main(int argc, char* argv[])
{   
	double x[MAXPOINTS], y[MAXPOINTS], z[MAXPOINTS];
    double new_x, new_y, new_z;
    double avg_x, avg_y, avg_z;
	int i;
	double t;
	double timer=0;
	int currentX=50;
	int currentY=0;
	int mazeX, mazeY;
	int end=0;
	srand(time(NULL));
	int difficulty;
	int repeat =0;
	sscanf(argv[1], "%d", &difficulty);
	int lastx=50;
	int lasty=0;
  if (argc != 2 )
  {
      printf("You must enter the difficulty level on the command line.");
      refresh();
      return;
  }
  else
  {
    /* 	Setup screen for Ncurses
        The initscr functionis used to setup the Ncurses environment
        The refreash function needs to be called to refresh the outputs
        to the screen */
    initscr();
    refresh();


	for(i =0; i < 80; i++){
		for(int j =0; j<difficulty; j++){
			
			mazeX = rand() % 101;
			repeat = 0;
			for(int k=0; k<101; k++){
				if(mazeX==MEMORY[k]){
					repeat = 2;
				}
			}
			if(repeat==2){
				continue;
			}
			MEMORY[mazeX];
			MAZE[mazeX][i] = '*';
			draw_character(mazeX, i, WALL);
		}
		for(int g=0; g<101;g++){
			MEMORY[g]=0;
		}
		
	}

    /* Read gyroscope data and fill the buffer before continuing */
	for(i = 0; i < NUM_SAMPLES; i++){
			scanf("%lf, %lf, %lf, %lf", &t, &new_x, &new_y, &new_z);
			x[i] = new_x;
			y[i] = new_y;
			z[i] = new_z;
		}	
    /* Event loop */
    do
    {

        /* Read data, update average */
		scanf("%lf, %lf, %lf, %lf", &t, &new_x, &new_y, &new_z);
        avg_x = m_avg(x, NUM_SAMPLES, new_x);
        avg_y = m_avg(y, NUM_SAMPLES, new_y);
        avg_z = m_avg(z, NUM_SAMPLES, new_z);
        /* Is it time to move?  if so, then move avatar */
		if(t>timer){
			timer = t + 200;
			if(avg_x>.75){
				currentX-=1;
			}
			else if(avg_x<-.75){
				currentX+=1;
			}
			currentY+=1;
			if(currentY==80){
				end=1;
				break;
			}
			if(currentX<0){
				currentX=0;
			}
			if(currentX>100){
				currentX=100;
			}
			
			for(i=0;i<difficulty;i++){
				if(MAZE[currentX][currentY]=='*'){
					if(avg_x>.75){
						currentX+=1;
					}
					else if(avg_x<.75){
						currentX-=1;
					}
					currentY-=1;
					if((MAZE[currentX-1][currentY]=='*')&&(MAZE[currentX+1][currentY]=='*')){
						end=2;
					}
					break;
				}
				
			}
			draw_character(lastx,lasty, ' ');
			draw_character(currentX, currentY, AVATAR);
			lastx=currentX;
			lasty=currentY;
			
		}
    } while(end==0); // Change this to end game at right time

    /* Print the win message */
	

    /* This function is used to cleanup the Ncurses environment.
    Without it, the characters printed to the screen will persist
    even after the program terminates */
    endwin();

  }
	if (end==1){
		printf("YOU WIN!\n");
	}
	else if (end ==2 ){
		printf("YOU LOSE!\n");
	}
}

double m_avg(double buffer[], int avg_size, double new_item)
{
	int i;
	double counter=0;
	for(i=1;i<avg_size;i++){
		buffer[i-1]=buffer[i];
	}
	
	buffer[avg_size-1]=new_item;
	for(i=0; i<avg_size;i++){
		counter+=buffer[i];
	}
	double movingAvg= counter/avg_size;
		
    return movingAvg;
}


void draw_character(int x, int y, char use)
{
    mvaddch(y,x,use);
    refresh();
}
