#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "pieces.inl"
#include "primlib.h"

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define GRID_WITDH 20
#define GRID_HEIGHT 30
#define GRID_SQAURE_SIZE 20
#define GRID_X_DISPLACEMENT ((SCREEN_WIDTH - (GRID_WITDH * GRID_SQAURE_SIZE)) / 2)
#define GRID_Y_DISPLACEMENT (SCREEN_HEIGTH - (GRID_HEIGHT * GRID_SQAURE_SIZE))
#define PIECE_SIZE 4
#define TEXT_X_CONST 120
#define TEXT_Y_CONST 20

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
    point piece_position;
    short piece_type;
} piece_struct;

void rotatePiece(piece_struct *piece);
void updatePiece(piece_struct *piece);
void handleKeys(piece_struct *piece);
void fallPiece(piece_struct *falling_piece);
void drawRect(rect piece_rect);
void updateRectPos(piece_struct *piece_ptr, int x_cord, int y_cord);
void welcomeMenu();
piece_struct initializePiece();
void drawPiece(piece_struct *piece);
void updateRectColor(piece_struct *piece_ptr, int x_cord, int y_cord, char piece_color);

int main(int argc, char *argv[])
{
    if (gfx_init())
    {
        exit(3);
    }
    welcomeMenu();
    piece_struct current_piece = initializePiece();
    while (1)
    {
        drawPiece(&current_piece);
        gfx_updateScreen();
        fallPiece(&current_piece);
        SDL_Delay(300);
        handleKeys(&current_piece);
    }

    return 0;
}
void welcomeMenu()
{
    gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
    gfx_textout(SCREEN_WIDTH / 2 - TEXT_X_CONST, SCREEN_HEIGTH / 2 - TEXT_Y_CONST,
                "Press any key to start a game", WHITE);
    gfx_updateScreen();
    gfx_getkey();
}
piece_struct initializePiece()
{
    piece_struct init;
    init.piece_position.x = SCREEN_WIDTH / 2;
    init.piece_position.y = 0;
    srand(time(NULL));
    init.piece_type = rand() % 7;
    init.rot_state = rand() % 4;
    updatePiece(&init);

    return init;
}
void updateRectColor(piece_struct *piece_ptr, int x_cord, int y_cord, char piece_color)
{
    switch (piece_color)
    {
    case 0:
        piece_ptr->piece_layout[x_cord][y_cord].rect_color = BLACK;
        break;
    case 1:
        piece_ptr->piece_layout[x_cord][y_cord].rect_color = YELLOW;
        break;
    case 2:
        piece_ptr->piece_layout[x_cord][y_cord].rect_color = GREEN;
        break;
    default:
        break;
    }
}
void fallPiece(piece_struct *falling_piece)
{
    falling_piece->piece_position.y += GRID_SQAURE_SIZE;
    for (size_t i = 0; i < PIECE_SIZE; i++)
    {
        for (size_t j = 0; j < PIECE_SIZE; j++)
        {
            updateRectPos(falling_piece, i, j);
        }
    }
}
void updateRectPos(piece_struct *piece_ptr, int x_cord, int y_cord)
{
    piece_ptr->piece_layout[x_cord][y_cord].left_upper.x =
        piece_ptr->piece_position.x + (x_cord * GRID_SQAURE_SIZE);
    piece_ptr->piece_layout[x_cord][y_cord].left_upper.y =
        piece_ptr->piece_position.y + (y_cord * GRID_SQAURE_SIZE);
    piece_ptr->piece_layout[x_cord][y_cord].right_down.x =
        piece_ptr->piece_position.x + ((x_cord + 1) * GRID_SQAURE_SIZE);
    piece_ptr->piece_layout[x_cord][y_cord].right_down.y =
        piece_ptr->piece_position.y + ((y_cord + 1) * GRID_SQAURE_SIZE);
}
void drawPiece(piece_struct *piece)
{
    for (size_t i = 0; i < PIECE_SIZE; i++)
    {
        for (size_t j = 0; j < PIECE_SIZE; j++)
        {
            drawRect(piece->piece_layout[i][j]);
        }
    }
}
void drawRect(rect piece_rect)
{
    gfx_filledRect(piece_rect.left_upper.x, piece_rect.left_upper.y, piece_rect.right_down.x,
                   piece_rect.right_down.y, piece_rect.rect_color);
}
void handleKeys(piece_struct *piece)
{
    switch (gfx_pollkey())
    {
    case SDLK_ESCAPE:
        exit(0);
        break;
    case SDLK_SPACE:
        rotatePiece(piece);
    }
}
void updatePiece(piece_struct *piece)
{
    for (size_t i = 0; i < PIECE_SIZE; i++)
    {
        for (size_t j = 0; j < PIECE_SIZE; j++)
        {
            updateRectPos(piece, i, j);
            updateRectColor(piece, i, j, pieces[piece->piece_type][piece->rot_state][i][j]);
        }
    }
}
void rotatePiece(piece_struct *piece)
{
    piece->rot_state++;
    if (piece->rot_state == 4)
    {
        piece->rot_state = 0;
    }
    updatePiece(piece);
}