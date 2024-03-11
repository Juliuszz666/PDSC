#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "pieces.h"
#include "primlib.h"

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define GRID_WITDH 20
#define GRID_HEIGHT 30
#define GRID_SQAURE_SIZE 20
#define GRID_X_DISPLACEMENT ((SCREEN_WIDTH - (GRID_WITDH * GRID_SQAURE_SIZE)) / 2)
#define GRID_Y_DISPLACEMENT (SCREEN_HEIGTH - (GRID_HEIGHT * GRID_SQAURE_SIZE))
#define PIECE_SIZE 4

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
} rotation_enum;

typedef struct
{
    rect piece_layout[PIECE_SIZE][PIECE_SIZE];
    rotation_enum rot_state;
} piece_struct;

rect null_rect = {{0, GRID_SQAURE_SIZE}, {0, GRID_SQAURE_SIZE}, 0};

void initializeGrid(rect grid[GRID_WITDH][GRID_HEIGHT]);
void drawGrid(rect grid[GRID_WITDH][GRID_HEIGHT]);
void drawPiece(piece_struct *piece);
void spawnPiece(piece_struct *block, rect grid[GRID_WITDH][GRID_HEIGHT]);
piece_struct initializePiecie();
void initializePieceLayout(piece_struct *block, int x, int y, int code);
bool isNotNullRect(rect piece_rect)
{
    return piece_rect.rect_color == BLACK ? false : true;
}

int main(int argc, char *argv[])
{
    rect grid[GRID_WITDH][GRID_HEIGHT];
    initializeGrid(grid);
    if (gfx_init())
    {
        exit(3);
    }
    piece_struct current_piecie = initializePiecie();
    while (1)
    {
        gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
        drawGrid(grid);
        drawPiece(&current_piecie);
        gfx_updateScreen();
        gfx_pollkey();
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

piece_struct initializePiecie()
{
    piece_struct init;
    srand(time(NULL));
    int piece = rand() % 7;
    int rotation = rand() % 4;
    printf("piece %d  rot %d\n", piece, rotation);
    char(*piece_array)[PIECE_SIZE][PIECE_SIZE] = &pieces[piece][rotation];
    for (size_t i = 0; i < PIECE_SIZE; i++)
    {
        for (size_t j = 0; j < PIECE_SIZE; j++)
        {
            switch (*piece_array[i][j])
            {
            case 0:
                initializePieceLayout(&init, i, j, 0); // basicly null_rect
                break;
            case 1:
                initializePieceLayout(&init, i, j, 1);
                break;
            case 2:
                break;
                initializePieceLayout(&init, i, j, 2);
            default:
                printf("Undefined behaviour\n");
                exit(3);
                break;
            }
        }
    }
    return init;
}

void initializePieceLayout(piece_struct *block, int x, int y, int code)
{
    enum color rect_color;
    switch (code)
    {
    case 0:
        rect_color = BLACK;
        break;
    case 1:
        rect_color = YELLOW;
        break;
    case 2:
        rect_color = RED;
        break;
    default:
        break;
    }
    rect block_rect = {{x * GRID_SQAURE_SIZE, y * GRID_SQAURE_SIZE},
                       {(x+1) * GRID_SQAURE_SIZE, (y + 1) * GRID_SQAURE_SIZE},
                       rect_color};
    block->piece_layout[x][y] = block_rect;
}
void spawnPiece(piece_struct *block, rect grid[GRID_WITDH][GRID_HEIGHT])
{
    int width_index = (GRID_WITDH - PIECE_SIZE) / 2;
    for (size_t i = 0; i < PIECE_SIZE; i++)
    {
        for (size_t j = 0; j < PIECE_SIZE; j++)
        {
            block->piece_layout[i][j].left_upper = grid[width_index + i][j].left_upper;
            block->piece_layout[i][j].right_down = grid[width_index + i][j].right_down;
        }
    }
}
void drawPiece(piece_struct *piece)
{
    for (size_t i = 0; i < PIECE_SIZE; i++)
    {
        for (size_t j = 0; j < PIECE_SIZE; j++)
        {
            gfx_filledRect(piece->piece_layout[i][j].left_upper.x, piece->piece_layout[i][j].left_upper.y,
                           piece->piece_layout[i][j].right_down.x, piece->piece_layout[i][j].right_down.y,
                           piece->piece_layout[i][j].rect_color);
        }
    }
}