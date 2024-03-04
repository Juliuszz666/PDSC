/*JULIAN BEDNAREK 250247 2CS3*/
/*INCLUDES BEGIN*/
#include "primlib.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
/*INCLUDES END*/
/*DEFINE BEGIN*/

/*Center of screen*/
#define CENTER_X gfx_screenWidth()/2
#define CENTER_Y gfx_screenHeight()/2
/*Rotation step in radians*/
#define ROT_STEP 0.01
/*Scalar to change size of polygon*/
#define SIZE_STEP 0.005
#define VERTICES 5
#define CENTER_ANGLE 2*M_PI/VERTICES
/*Min and max distance from vertice to center*/
#define MIN_SIZE 50
#define MAX_SIZE 300
/*Colors used in program*/
#define BG_COLOR BLACK
#define MAX_COLOR 7
#define MIN_COLOR 1
/*1000/delay = FPS*/
#define DELAY 10
/*DEFINE END*/
/*STRUCTS BEGIN*/

/*Stuct for wrapping up vertices*/
typedef struct {
	double x;
	double y;
}point;
/*Enum type for defining the growth state of the polygon*/
typedef enum
{
	INCREASING,
	DECREASING
}growing_state;

/*STRUCTS END*/
/*FUNCTION DECLARATIONS BEGIN*/

/* Function updates the position of vertices*/
/* @param vertice[] array of point structures to be updated*/
/* @param growth wheter function needs to increase or decrease size of polygon*/
void updateVertices(point vertice[], growing_state growth);
/*FUNCTION DECLARATIONS END*/
/*MAIN FUNCTION BEGIN*/
int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}
	/*Initialization of vertices*/
	point vertice[VERTICES];
	for (size_t i = 0; i < VERTICES; i++)
	{
		vertice[i].x = CENTER_X + cos(i*CENTER_ANGLE)*MIN_SIZE;
		vertice[i].y = CENTER_Y + sin(i*CENTER_ANGLE)*MIN_SIZE;
	}
	growing_state growth = INCREASING;
	enum color line_color = 1;

	while (1)
	{
		/*Clearing screen*/
		gfx_filledRect(0, 0, gfx_screenWidth()-1, gfx_screenHeight()-1, BG_COLOR);
		/*Drawing vertices*/
		for (size_t i = 0; i < VERTICES; i++)
		{
			switch (i)
			{
			case VERTICES-1:
				gfx_line((int)vertice[i].x, (int)vertice[i].y, (int)vertice[0].x, (int)vertice[0].y, line_color);
				break;
			default:
				gfx_line((int)vertice[i].x, (int)vertice[i].y, (int)vertice[i+1].x, (int)vertice[i+1].y, line_color);
				break;
			}
		}
		gfx_updateScreen();
		SDL_Delay(DELAY);

		/*Setting proper growth depending of size of the polygon*/
		if(hypot((vertice[0].x-CENTER_X), (vertice[0].y-CENTER_Y))>MAX_SIZE)
		{
			growth = DECREASING;
			if(line_color<MAX_COLOR) line_color++;
			else line_color = MIN_COLOR;
		}
		if(hypot((vertice[0].x-CENTER_X), (vertice[0].y-CENTER_Y))<MIN_SIZE)
		{
			growth = INCREASING;
			if(line_color<MAX_COLOR) line_color++;
			else line_color = MIN_COLOR;
		}

		updateVertices(vertice, growth);		

		gfx_pollkey();
	}
	return 0;
}
/*MAIN FUNCTION END*/
/*FUNCTIONS BODIES BEGIN*/
void updateVertices(point vertice[], growing_state growth)
{
	for (size_t i = 0; i < VERTICES; i++)
		{
			/*Increasing/decreasing size of polygon*/
			switch (growth)
			{
				case INCREASING:
					vertice[i].x += SIZE_STEP * (vertice[i].x-CENTER_X);
					vertice[i].y += SIZE_STEP * (vertice[i].y-CENTER_Y);
					break;
				case DECREASING:
					vertice[i].x -= SIZE_STEP * (vertice[i].x-CENTER_X);
					vertice[i].y -= SIZE_STEP * (vertice[i].y-CENTER_Y);
					break;
				default:
					printf("Undefined behaviour\n");
					exit(1);
					break;
			}

			/*Saving old values for proper rotation*/
			double oldX = vertice[i].x;
			double oldY = vertice[i].y;

			/*Rotating the vertices relative to center*/
			vertice[i].x = CENTER_X + ((oldX-CENTER_X) * cos(ROT_STEP)) - ((oldY-CENTER_Y) * sin(ROT_STEP));
			vertice[i].y = CENTER_Y + ((oldX-CENTER_X) * sin(ROT_STEP)) + ((oldY-CENTER_Y) * cos(ROT_STEP));
		}
}
/*FUNCTIONS BODIES END*/