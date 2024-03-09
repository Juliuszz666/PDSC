/*Julian Bednarek 250247 2CS3*/
#include "primlib.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define DISC_NO 6
#define DISC_HEIGHT 20
#define PEG_NO 5
#define DISC_WIDTH_MAX (SCREEN_WIDTH / ((3 * PEG_NO) + 1))
#define DISC_WIDTH_MIN (DISC_WIDTH_MAX / 3)
#define DISC_COLOR BLUE
#define PEG_WIDTH 10
#define PEG_SPAWN_H_CO (7.0 / 8)
#define PEG_COLOR YELLOW
#define PEG_HEIGHT (DISC_HEIGHT * (DISC_NO + 2))
#define STACK_SIZE DISC_NO

typedef struct
{
    int x;
    int y;
} point;
typedef struct
{
    point leftUpper;
    point rightDown;
} rect;

static int top[PEG_NO] = {0};
rect pegs[PEG_NO];
rect stacks[PEG_NO][DISC_NO];

void initializePegs(rect pegs[]);
void initializeDiscs(rect pegs[]);
void drawDiscs();
void drawPegs();
rect popDisc(int index);
rect getDisc(int key);
void pushDisc(rect disc, int index);
void handleKeys(int key);

int main(int argc, char *argv[])
{
    if (gfx_init())
    {
        exit(3);
    }

    initializePegs(pegs);
    initializeDiscs(pegs);

    while (1)
    {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
        gfx_filledRect(0, SCREEN_HEIGTH * PEG_SPAWN_H_CO, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, GREEN);
        drawPegs();
        drawDiscs();

        gfx_updateScreen();
        int source = gfx_getkey();
        rect source_disc = getDisc(source);
		int dest = gfx_getkey();
    }

    return 0;
}
void initializePegs(rect pegs[])
{
    for (size_t i = 0; i < PEG_NO; i++)
    {
        pegs[i].rightDown.x = ((SCREEN_WIDTH / (PEG_NO + 1)) * (i + 1)) + PEG_WIDTH / 2;
        pegs[i].leftUpper.x = pegs[i].rightDown.x - PEG_WIDTH;
        pegs[i].rightDown.y = SCREEN_HEIGTH * PEG_SPAWN_H_CO;
        pegs[i].leftUpper.y = pegs[i].rightDown.y - PEG_HEIGHT;
    }
}
void initializeDiscs(rect pegs[])
{
    for (size_t i = 0; i < DISC_NO; i++)
    {
        int disc_width = DISC_WIDTH_MAX - ((DISC_WIDTH_MAX - DISC_WIDTH_MIN) / DISC_NO) * i;
        stacks[0][i].rightDown.x = pegs[0].rightDown.x + disc_width;
        stacks[0][i].leftUpper.x = pegs[0].leftUpper.x - disc_width;
        stacks[0][i].rightDown.y = pegs[0].rightDown.y - (DISC_HEIGHT * i);
        stacks[0][i].leftUpper.y = pegs[0].rightDown.y - (DISC_HEIGHT * (i + 1));
        top[0]++;
    }
}
rect popDisc(int index)
{
    if (top[index] > 0)
    {
        assert(top[index] > 0);
        return stacks[index][--top[index]];
    }
	else return;
}
void pushDisc(rect disc, int index)
{
    if (top[index] < STACK_SIZE)
    {
        assert(top[index] < STACK_SIZE);
        int disc_width = (disc.leftUpper.x - disc.rightDown.x) / 2;
        disc.rightDown.x = pegs[index].rightDown.x + disc_width;
        disc.leftUpper.x = pegs[index].leftUpper.x - disc_width;
        disc.rightDown.y = pegs[index].rightDown.y - (DISC_HEIGHT * top[index]);
        disc.leftUpper.y = pegs[index].rightDown.y - (DISC_HEIGHT * (top[index] + 1));
        stacks[index][top[index]++] = disc;
    }
}
void handleKeys(int key)
{
    rect popped;
    switch (key)
    {
    case SDLK_ESCAPE:
        exit(1);
        break;
    case SDLK_1:
        popped = popDisc(0);
        pushDisc(popped, 1);
        break;
    case SDLK_2:
        popped = popDisc(1);
        pushDisc(popped, 0);
        break;
    default:
        break;
    }
}
void drawDiscs()
{
    short peg_index = 0;
    while (peg_index < PEG_NO)
    {
        short disc_index = 0;
        while (disc_index < top[peg_index])
        {
            gfx_filledRect(stacks[peg_index][disc_index].leftUpper.x, stacks[peg_index][disc_index].leftUpper.y,
                           stacks[peg_index][disc_index].rightDown.x, stacks[peg_index][disc_index].rightDown.y,
                           DISC_COLOR);
            disc_index++;
        }
        peg_index++;
    }
}
void drawPegs()
{
    for (size_t i = 0; i < PEG_NO; i++)
    {
        gfx_filledRect(pegs[i].leftUpper.x, pegs[i].leftUpper.y, pegs[i].rightDown.x, pegs[i].rightDown.y, PEG_COLOR);
    }
}