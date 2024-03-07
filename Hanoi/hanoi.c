/*Julian Bednarek 250247 2CS3*/
/*INCLUDES BEGIN*/
#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
/*INCLUDES END*/
/*DEFINES BEGIN*/
#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define DISC_NO 4
#define DISC_HEIGHT 20
#define PEG_NO 3
#define DISC_WIDTH_MAX SCREEN_WIDTH/((3*PEG_NO)+1)
#define DISC_WIDTH_MIN DISC_WIDTH_MAX/3
#define DISC_COLOR BLUE
#define PEG_WIDTH 10
#define PEG_SPAWN_H_CO 7.0/8
#define PEG_COLOR YELLOW
#define PEG_HEIGHT DISC_HEIGHT*(DISC_NO+2)
/*DEFINES END*/
/*STRUCTS BEGIN*/
typedef struct 
{
	int x;
	int y;	
}point;
typedef struct
{
	point leftUpper;
	point rightDown;
}rect;
/*STRUCTS END*/
/*GLOBAL VARIABLES BEGIN*/
static int top[PEG_NO];
/*Matrix used to store actucal position of discs*/
rect stacks[PEG_NO][DISC_NO];
/*GLOBAL VARIABLES END*/
/*FUNCTIONS DECLARATIONS BEGIN*/

/*Function computes position of pegs on screen*/
/*@param pegs structures where position of pegs is stored*/
void initializePegs(rect pegs[]);
/*Function compuets initial poisiton of dics on screen (leftmost peg)*/
/*@param disc structures where position of discs is stored*/
/*@param pegs position of pegs*/
void initializeDiscs(rect discs[], rect pegs[]);
/*Funtion pops discs from a stack*/
/*@param index number of peg on which we operate*/
rect popDisc(int index);
/*FUNCTIONS DECLARATIONS END*/
/*MAIN FUNCTION BEGIN*/
int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	rect pegs[PEG_NO];
	rect discs[DISC_NO];
	initializePegs(pegs);
	initializeDiscs(discs, pegs);

	
	while (1)
	{
		gfx_filledRect(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGTH-1, BLACK);
		gfx_filledRect(0, SCREEN_HEIGTH*PEG_SPAWN_H_CO, SCREEN_WIDTH-1, SCREEN_HEIGTH-1, GREEN);
		for (size_t i = 0; i < PEG_NO; i++)
		{
			gfx_filledRect(pegs[i].leftUpper.x, pegs[i].leftUpper.y, pegs[i].rightDown.x, pegs[i].rightDown.y, PEG_COLOR);
		}
		for (size_t i = 0; i < DISC_NO; i++)
		{
			gfx_filledRect(discs[i].leftUpper.x, discs[i].leftUpper.y, discs[i].rightDown.x, discs[i].rightDown.y, DISC_COLOR);
		}
		
		gfx_updateScreen();
		int key = gfx_getkey();
		switch (key)
		{
		case SDLK_ESCAPE:
			exit(1);
			break;
		case SDLK_1:
			printf("Cos\n");
			break;
		case SDLK_2:
			printf("Cos\n");
			break;
		case SDLK_3:
			printf("Cos\n");
			break;
		case SDLK_4:
			printf("Cos\n");
			break;
		case SDLK_5:
			printf("Cos\n");
			break;
		case SDLK_6:
			printf("Cos\n");
			break;
		case SDLK_7:
			printf("Cos\n");
			break;
		case SDLK_8:
			printf("Cos\n");
			break;
		case SDLK_9:
			printf("Cos\n");
			break;
		case SDLK_0:
			printf("Cos\n");
			break;
		default:
			printf("Undefined behavior\n");
			exit(1);
			break;
		}
	}

	return 0;
}
/*MAIN FUNCTION END*/
/*FUNCTION BODY BEGIN*/
void initializePegs(rect pegs[])
{
	for (size_t i = 0; i < PEG_NO; i++)
	{
		pegs[i].rightDown.x = ((SCREEN_WIDTH/(PEG_NO+1))*(i+1)) + PEG_WIDTH/2;
		pegs[i].leftUpper.x = pegs[i].rightDown.x - PEG_WIDTH;
		pegs[i].rightDown.y = SCREEN_HEIGTH*PEG_SPAWN_H_CO;
		pegs[i].leftUpper.y = pegs[i].rightDown.y - PEG_HEIGHT;
	}
}
void initializeDiscs(rect discs[], rect pegs[])
{
	for (size_t i = 0; i < DISC_NO; i++)
	{
		int disc_width = DISC_WIDTH_MAX - ((DISC_WIDTH_MAX-DISC_WIDTH_MIN)/DISC_NO)*i;
		discs[i].rightDown.x = pegs[0].rightDown.x + disc_width;
		discs[i].leftUpper.x = pegs[0].leftUpper.x - disc_width;
		discs[i].rightDown.y = pegs[0].rightDown.y - (DISC_HEIGHT*i);
		discs[i].leftUpper.y = pegs[0].rightDown.y - (DISC_HEIGHT*(i+1));
	}
}
rect popDisc(int index)
{
	assert(top[index]>0);
	return stacks[index][--top[index]];
}
/*FUNCTION BODY END*/