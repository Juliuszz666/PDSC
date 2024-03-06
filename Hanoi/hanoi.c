/*Julian Bednarek 250247 2CS3*/
/*INCLUDES BEGIN*/
#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
/*INCLUDES END*/
/*DEFINES BEGIN*/
#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define DISC_NO 8
#define DISC_HEIGHT 20
#define PEG_NO 3
#define DISC_WIDTH_MAX SCREEN_WIDTH/((2*PEG_NO)+1)
#define DISC_WIDTH_MIN DISC_WIDTH_MAX/2
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
/*FUNCTIONS DECLARATIONS BEGIN*/

/*Function computes position of pegs on screen*/
/*@param pegs structures where position of pegs is stored*/
void initializePegs(rect pegs[]);
/*Function compuets initial poisiton of dics on screen (leftmost peg)*/
/*@param disc structures where position of discs is stored*/
void initializeDiscs(rect discs[]);
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
	initializeDiscs(discs);

	
	while (1)
	{
		gfx_filledRect(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGTH-1, BLACK);
		gfx_filledRect(0, SCREEN_HEIGTH*PEG_SPAWN_H_CO, SCREEN_WIDTH-1, SCREEN_HEIGTH-1, GREEN);
		for (size_t i = 0; i < PEG_NO; i++)
		{
			gfx_filledRect(pegs[i].leftUpper.x, pegs[i].leftUpper.y, pegs[i].rightDown.x, pegs[i].rightDown.y, PEG_COLOR);
		}
		gfx_updateScreen();
		gfx_pollkey();
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
void initializeDiscs(rect discs[])
{
	
}
/*FUNCTION BODY END*/