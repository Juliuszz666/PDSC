/*Julian Bednarek 250247 2CS3*/
#include "primlib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define DISC_NO 8
#define DISC_HEIGHT 20
#define PEG_NO 3
#define DISC_WIDTH_MAX (SCREEN_WIDTH / ((3 * PEG_NO) + 1))
#define DISC_WIDTH_MIN (DISC_WIDTH_MAX / 3)
#define DISC_COLOR BLUE
#define PEG_WIDTH 10
#define PEG_SPAWN_H_CO (15.0 / 16)
#define PEG_COLOR YELLOW
#define PEG_HEIGHT (DISC_HEIGHT * (DISC_NO + 2))
#define ANIMATION_UP_DOWN_HEIGHT ((int)(SCREEN_HEIGTH * PEG_SPAWN_H_CO) - PEG_HEIGHT - 100)
#define STACK_SIZE DISC_NO
#define ANIMATION_STEP 1
#define KEY_CONST 48
#define FINAL_DELAY 3000
#define ANIMATION_DELAY 5

typedef struct
{
    int x;
    int y;
} point;
typedef struct
{
    point left_upper;
    point right_down;
} rect;

int top[PEG_NO] = {0};
rect pegs[PEG_NO];
rect stacks[PEG_NO][DISC_NO];
rect null_rect = {{0, 0}, {0, 0}};

/*Function checks if ESC was pressed*/
void checkForQuit(int key);
/*Funtion return sign of num1 - num2*/
int signum(int num1, int num2);
int checkKey(int key);
void initializePegs(rect pegs[]);
void initializeDiscs(rect pegs[]);
void drawDiscs();
void drawPegs();
void renderGame();
rect popDisc(int index);
rect getTop(int index);
void pushDisc(rect disc, int index);
/*Function handles input keys and handles disc movement and game logic*/
void action(int src, int dest);
void animateMovement(rect disc, int start, int end);
void printMessage(const char *message[]);
bool isKeyUsed(int key);
bool notNullRect(rect disc);
bool isLegalMove(int index, rect disc);
/*Funtion returns true when player succesfully moved all discs otherwise returns false*/
bool isWonOrLost();

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
        renderGame();
        gfx_updateScreen();
        int source = gfx_pollkey();
        int dest = -1;
        if (source != -1)
        {
            source = checkKey(source);
            if (source == SDLK_RETURN)
                break;
            dest = gfx_getkey();
            dest = checkKey(dest);
            if (dest == SDLK_RETURN)
                break;
            action(source, dest);
        }
    }
    const char *message[] = {"You won", "You lost"};
    renderGame();
    printMessage(message);
    gfx_updateScreen();
    SDL_Delay(FINAL_DELAY);
    exit(3);

    return 0;
}
void action(int src, int dest)
{
    int pop_index = src;
    int push_index = dest;
    if (pop_index != -1 && push_index != -1)
    {
        pop_index == 0 ? pop_index = PEG_NO - 1 : --pop_index;
        push_index == 0 ? push_index = PEG_NO - 1 : --push_index;
        rect to_be_popped = getTop(pop_index);
        if (isLegalMove(push_index, to_be_popped))
        {
            rect popped = popDisc(pop_index);
            animateMovement(popped, pop_index, push_index);
            pushDisc(popped, push_index);
        }
    }
}
void animateMovement(rect disc, int start, int end)
{
    while (disc.left_upper.y != ANIMATION_UP_DOWN_HEIGHT)
    {
        renderGame();
        gfx_filledRect(disc.left_upper.x, disc.left_upper.y, disc.right_down.x, disc.right_down.y, DISC_COLOR);
        gfx_updateScreen();
        SDL_Delay(ANIMATION_DELAY);
        int key = gfx_pollkey();
        checkForQuit(key);
        disc.left_upper.y -= ANIMATION_STEP;
        disc.right_down.y -= ANIMATION_STEP;
    }
    int direction = signum(start, end);
    int disc_center = disc.right_down.x - ((abs(disc.left_upper.x - disc.right_down.x)) / 2);
    int end_peg_center = pegs[end].right_down.x - (PEG_WIDTH / 2);
    while (disc_center != end_peg_center)
    {
        renderGame();
        gfx_filledRect(disc.left_upper.x, disc.left_upper.y, disc.right_down.x, disc.right_down.y, DISC_COLOR);
        gfx_updateScreen();
        SDL_Delay(ANIMATION_DELAY);
        int key = gfx_pollkey();
        checkForQuit(key);
        disc.left_upper.x += (ANIMATION_STEP * direction);
        disc.right_down.x += (ANIMATION_STEP * direction);
        disc_center += (ANIMATION_STEP * direction);
    }
    while (disc.left_upper.y != pegs[end].right_down.y - (DISC_HEIGHT * (top[end] + 1)))
    {
        renderGame();
        gfx_filledRect(disc.left_upper.x, disc.left_upper.y, disc.right_down.x, disc.right_down.y, DISC_COLOR);
        gfx_updateScreen();
        SDL_Delay(ANIMATION_DELAY);
        int key = gfx_pollkey();
        checkForQuit(key);
        disc.left_upper.y += ANIMATION_STEP;
        disc.right_down.y += ANIMATION_STEP;
    }
}
void checkForQuit(int key)
{
    if (key == SDLK_ESCAPE)
        exit(3);
}
bool isLegalMove(int index, rect disc)
{
    int on_stack_width;
    if (!notNullRect(disc))
        return false;
    if (top[index] == 0)
        on_stack_width = SCREEN_WIDTH;
    else
        on_stack_width = abs(stacks[index][top[index] - 1].right_down.x - stacks[index][top[index] - 1].left_upper.x);
    int disc_witdh = abs(disc.right_down.x - disc.left_upper.x);
    if (disc_witdh < on_stack_width)
        return true;
    return false;
}
void initializePegs(rect pegs[])
{
    for (size_t i = 0; i < PEG_NO; i++)
    {
        pegs[i].right_down.x = ((SCREEN_WIDTH / (PEG_NO + 1)) * (i + 1)) + PEG_WIDTH / 2;
        pegs[i].left_upper.x = pegs[i].right_down.x - PEG_WIDTH;
        pegs[i].right_down.y = SCREEN_HEIGTH * PEG_SPAWN_H_CO;
        pegs[i].left_upper.y = pegs[i].right_down.y - PEG_HEIGHT;
    }
}
void initializeDiscs(rect pegs[])
{
    for (size_t i = 0; i < DISC_NO; i++)
    {
        int disc_width = DISC_WIDTH_MAX - ((DISC_WIDTH_MAX - DISC_WIDTH_MIN) / DISC_NO) * i;
        int peg_center = pegs[0].right_down.x - (PEG_WIDTH / 2);
        stacks[0][i].right_down.x = peg_center + disc_width;
        stacks[0][i].left_upper.x = peg_center - disc_width;
        stacks[0][i].right_down.y = pegs[0].right_down.y - (DISC_HEIGHT * i);
        stacks[0][i].left_upper.y = pegs[0].right_down.y - (DISC_HEIGHT * (i + 1));
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
        int disc_width = abs((disc.left_upper.x - disc.right_down.x) / 2);
        int peg_center = pegs[index].right_down.x - (PEG_WIDTH / 2);
        disc.right_down.x = peg_center + disc_width;
        disc.left_upper.x = peg_center - disc_width;
        disc.right_down.y = pegs[index].right_down.y - (DISC_HEIGHT * top[index]);
        disc.left_upper.y = pegs[index].right_down.y - (DISC_HEIGHT * (top[index] + 1));
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
            gfx_filledRect(stacks[peg_index][disc_index].left_upper.x, stacks[peg_index][disc_index].left_upper.y,
                           stacks[peg_index][disc_index].right_down.x, stacks[peg_index][disc_index].right_down.y,
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
        gfx_filledRect(pegs[i].left_upper.x, pegs[i].left_upper.y, pegs[i].right_down.x, pegs[i].right_down.y,
                       PEG_COLOR);
    }
}
bool isKeyUsed(int key)
{
    if ((PEG_NO == 10) && (key > 47) && (key < 58))
        return true;
    else if ((key > KEY_CONST) && (key <= KEY_CONST + PEG_NO))
        return true;
    else
        return false;
}
bool notNullRect(rect disc)
{
    if (disc.left_upper.x != 0)
        return true;
    return false;
}
int checkKey(int key)
{
    switch (key)
    {
    case SDLK_ESCAPE:
        exit(3);
        break;
    case SDLK_RETURN:
        return key;
        break;
    default:
        if (isKeyUsed(key))
            return (key - KEY_CONST);
        else
            return -1;
        break;
    }
    return -1;
}
rect getTop(int index)
{
    if (top[index] > 0)
        return stacks[index][top[index] - 1];
    else
        return null_rect;
}
void renderGame()
{
    gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
    gfx_filledRect(0, SCREEN_HEIGTH * PEG_SPAWN_H_CO, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, GREEN);
    drawPegs();
    drawDiscs();
}
int signum(int num1, int num2)
{
    if (num1 - num2 > 0)
        return -1;
    if (num1 - num2 < 0)
        return 1;
    else
        return 0;
}
bool isWonOrLost()
{
    if (top[0] != 0)
        return false;
    for (size_t i = 1; i < PEG_NO; i++)
    {
        if (top[i] == DISC_NO)
        {
            return true;
        }
    }
    return false;
}
void printMessage(const char *message[])
{
    if (isWonOrLost())
        gfx_textout(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2, message[0], WHITE);
    else
        gfx_textout(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2, message[1], WHITE);
}