#include "pieces.h"
#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define GRID_WITDH 20
#define GRID_HEIGHT 30
#define GRID_SQAURE_SIZE 20
#define GRID_X_DISPLACEMENT ((SCREEN_WIDTH - (GRID_WITDH * GRID_SQAURE_SIZE)) / 2)
#define GRID_Y_DISPLACEMENT (SCREEN_HEIGTH - (GRID_HEIGHT * GRID_SQAURE_SIZE))

typedef struct
{
    int x;
    int y;
} point;

typedef struct
{
    point left_upper;
    point right_down;
    enum color rect_color;
} rect;

typedef enum
{
    LEFT,
    UP,
    RIGHT,
    DOWN
} rotation;

typedef struct
{
    rotation rot_state;
} piece;

void initializeGrid(rect grid[GRID_WITDH][GRID_HEIGHT]);
void drawGrid(rect grid[GRID_WITDH][GRID_HEIGHT]);

int main(int argc, char *argv[])
{
    rect grid[GRID_WITDH][GRID_HEIGHT];
    initializeGrid(grid);
    if (gfx_init())
    {
        exit(3);
    }
    while (1)
    {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
        drawGrid(grid);
        gfx_updateScreen();
        gfx_getkey();
    }

    return 0;
}

void initializeGrid(rect grid[GRID_WITDH][GRID_HEIGHT])
{
    for (size_t i = 0; i < GRID_WITDH; i++)
    {
        for (size_t j = 0; j < GRID_HEIGHT; j++)
        {
            grid[i][j].left_upper.x = (i * GRID_SQAURE_SIZE) + GRID_X_DISPLACEMENT;
            grid[i][j].left_upper.y = (j * GRID_SQAURE_SIZE) + GRID_Y_DISPLACEMENT;
            grid[i][j].right_down.x = ((i + 1) * GRID_SQAURE_SIZE) + GRID_X_DISPLACEMENT;
            grid[i][j].right_down.y = ((j + 1) * GRID_SQAURE_SIZE) + GRID_Y_DISPLACEMENT;
            grid[i][j].rect_color = WHITE;
        }
    }
}

void drawGrid(rect grid[GRID_WITDH][GRID_HEIGHT])
{
    for (size_t i = 0; i < GRID_WITDH; i++)
    {
        for (size_t j = 0; j < GRID_HEIGHT; j++)
        {
            gfx_filledRect(grid[i][j].left_upper.x, grid[i][j].left_upper.y, grid[i][j].right_down.x,
                           grid[i][j].right_down.y, grid[i][j].rect_color);
        }
    }
}