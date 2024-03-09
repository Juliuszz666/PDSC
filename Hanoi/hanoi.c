/*Julian Bednarek 250247 2CS3*/
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "primlib.h"

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define DISC_NO 4
#define DISC_HEIGHT 20
#define PEG_NO 3
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

short top[PEG_NO] = {0};
rect pegs[PEG_NO];
rect stacks[PEG_NO][DISC_NO];
rect null_rect = {{0, 0}, {0, 0}};

void initializePegs(rect pegs[]);
void initializeDiscs(rect pegs[]);
void drawDiscs();
void drawPegs();
rect popDisc(int index);
rect getDisc(int key);
void pushDisc(rect disc, int index);
void putDisc(rect disc, int key);
void action(int src, int dest);
bool isKeyUsed(int key);
bool notNullRect(rect disc);
bool isLegalMove(int index, rect disc);

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
        int dest = gfx_getkey();
        action(source, dest);
    }

    return 0;
}
bool isLegalMove(int index, rect disc)
{
    int on_stack_width;
    if (top[index] == 0)
        on_stack_width = SCREEN_WIDTH;
    else
        on_stack_width = abs(stacks[index][top[index] - 1].rightDown.x - stacks[index][top[index] - 1].leftUpper.x);
    int disc_witdh = abs(disc.rightDown.x - disc.leftUpper.x);
    if (disc_witdh < on_stack_width)
        return true;
    return false;
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
        int peg_center = pegs[0].rightDown.x - (PEG_WIDTH / 2);
        stacks[0][i].rightDown.x = peg_center + disc_width;
        stacks[0][i].leftUpper.x = peg_center - disc_width;
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
    else
    {
        return null_rect;
    }
}
void pushDisc(rect disc, int index)
{
    if ((top[index] < STACK_SIZE) && notNullRect(disc))
    {
        assert(top[index] < STACK_SIZE);
        int disc_width = abs((disc.leftUpper.x - disc.rightDown.x) / 2);
        int peg_center = pegs[index].rightDown.x - (PEG_WIDTH / 2);
        disc.rightDown.x = peg_center + disc_width;
        disc.leftUpper.x = peg_center - disc_width;
        disc.rightDown.y = pegs[index].rightDown.y - (DISC_HEIGHT * top[index]);
        disc.leftUpper.y = pegs[index].rightDown.y - (DISC_HEIGHT * (top[index] + 1));
        stacks[index][top[index]++] = disc;
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
bool isKeyUsed(int key)
{
    if ((PEG_NO == 10) && (key > 47) && (key < 58))
        return true;
    else if ((key > 48) && (key <= 48 + PEG_NO))
        return true;
    else
        return false;
}
bool notNullRect(rect disc)
{
    if (disc.leftUpper.x != 0)
        return true;
    return false;
}
int checkKey(int key)
{
    switch (key)
    {
    case SDLK_1:
    case SDLK_2:
    case SDLK_3:
    case SDLK_4:
    case SDLK_5:
    case SDLK_6:
    case SDLK_7:
    case SDLK_8:
    case SDLK_9:
    case SDLK_0:
        if (isKeyUsed(key))
            return (key - 48);
        else
            return -1;
        break;
    case SDLK_ESCAPE:
        exit(1);
        break;
    case SDLK_RETURN:
        printf("Placeholder\n");
        break;
    default:
        return -1;
        break;
    }
    return -1;
}
void action(int src, int dest)
{
    int pop_index = checkKey(src);
    int push_index = checkKey(dest);
    if (pop_index != -1 && push_index != -1)
    {
        pop_index == 0 ? pop_index = PEG_NO - 1 : pop_index--;
        push_index == 0 ? push_index = PEG_NO - 1 : push_index--;
        rect popped = popDisc(pop_index);
        if (isLegalMove(push_index, popped))
            pushDisc(popped, push_index);
        else
            pushDisc(popped, pop_index);
    }
}