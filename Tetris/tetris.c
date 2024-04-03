#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "pieces.inl"
#include "primlib.h"

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define GRID_WITDH 14
#define GRID_HEIGHT 30
#define GRID_SQAURE_SIZE 20
#define GRID_X_DISPLACEMENT ((SCREEN_WIDTH - (GRID_WITDH * GRID_SQAURE_SIZE)) / 2)
#define GRID_Y_DISPLACEMENT (SCREEN_HEIGTH - (GRID_HEIGHT * GRID_SQAURE_SIZE))
#define PIECE_SIZE 4
#define TEXT_X_CONST 120
#define TEXT_Y_CONST 20
#define ROW_FLAG 1
#define COL_FLAG 2
#define BUFFER_SIZE 100
#define BASE_DELAY 250
#define KEY_DELAY 10
#define NEXT_X_CO (GRID_WITDH * 1.5)
#define NEXT_Y_CO (GRID_HEIGHT / 2)
#define PIECE_KIND_NO 7
#define PIECE_ROT_NO 4

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

typedef enum
{
    MV_RIGHT,
    MV_DOWN,
    MV_LEFT
} direction;

typedef struct
{
    rect piece_layout[PIECE_SIZE][PIECE_SIZE];
    rotation rot_state;
    point piece_position;
    short piece_type;
} piece_template;

rect grid[GRID_WITDH][GRID_HEIGHT] = {0};
const point dir[3] = {{1, 0}, {0, 1}, {-1, 0}};

piece_template initializePiece(piece_template next);
piece_template initializeNext();
int fallPiece(piece_template *falling_piece);
int findPieceBound(piece_template *piece, short flag);
int rowToDelete();
bool checkRotCollision(piece_template *piece);
bool checkMoveCollision(piece_template *piece, point div_vector);
bool checkCollision(piece_template *piece);
bool isRowColumnEmpty(int flag, int index, piece_template *piece);
bool isGameOver();
bool isRowFull(int index);
void drawBoard();
void removeRow(int row_to_delete, int *score);
void updatePiecePos(piece_template *piece);
void movePiece(point dir, piece_template *piece);
void rotatePiece(piece_template *piece);
void updatePiece(piece_template *piece);
void handleKeys(piece_template *piece);
void drawRect(rect piece_rect);
void updateRectPos(piece_template *piece_ptr, int x_cord, int y_cord);
void welcomeMenu();
void drawPiece(piece_template *piece);
void updateRectColor(piece_template *piece_ptr, int x_cord, int y_cord, char piece_color);
void fastFall(piece_template *piece);
void dumpPiece(piece_template *dumped_piece, int *score);
void drawGrid();
void initializeGrid();
void gameOverMenu(int score);

int main(int argc, char *argv[])
{
    if (gfx_init())
    {
        exit(3);
    }
    welcomeMenu();
    initializeGrid();
    int score = 0;
    piece_template next_piece = initializeNext();
    piece_template current_piece = initializePiece(next_piece);
    next_piece = initializeNext();
    while (1)
    {
        drawGrid();
        drawPiece(&current_piece);
        drawPiece(&next_piece);
        drawBoard();
        gfx_updateScreen();
        if (fallPiece(&current_piece))
        {
            dumpPiece(&current_piece, &score);
            if (isGameOver())
            {
                break;
            }
            current_piece = initializePiece(next_piece);
            next_piece = initializeNext();
        }
        int row_to_delete = rowToDelete();
        if (row_to_delete >= 0)
        {
            removeRow(row_to_delete, &score);
        }
        handleKeys(&current_piece);
        SDL_Delay(BASE_DELAY);
    }
    gameOverMenu(score);
    return 0;
}
piece_template initializePiece(piece_template next)
{
    piece_template init = next;
    init.piece_position.x = GRID_WITDH / 2;
    init.piece_position.y = 0;
    updatePiece(&init);

    return init;
}
piece_template initializeNext()
{
    piece_template next;
    next.piece_position.x = NEXT_X_CO;
    next.piece_position.y = NEXT_Y_CO;
    srand(time(NULL));
    next.piece_type = rand() % PIECE_KIND_NO;
    next.rot_state = rand() % PIECE_ROT_NO;
    updatePiece(&next);

    return next;
}
void updateRectColor(piece_template *piece_ptr, int x_cord, int y_cord, char piece_color)
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
int fallPiece(piece_template *falling_piece)
{
    if (checkMoveCollision(falling_piece, dir[MV_DOWN]))
    {
        falling_piece->piece_position.y += 1;
        updatePiecePos(falling_piece);
        return 0;
    }
    return 1;
}
void updateRectPos(piece_template *piece_ptr, int x_cord, int y_cord)
{
    piece_ptr->piece_layout[x_cord][y_cord].left_upper.x = piece_ptr->piece_position.x + x_cord;
    piece_ptr->piece_layout[x_cord][y_cord].left_upper.y = piece_ptr->piece_position.y + y_cord;
    piece_ptr->piece_layout[x_cord][y_cord].right_down.x = piece_ptr->piece_position.x + x_cord + 1;
    piece_ptr->piece_layout[x_cord][y_cord].right_down.y = piece_ptr->piece_position.y + y_cord + 1;
}
void drawPiece(piece_template *piece)
{
    for (int i = 0; i < PIECE_SIZE; i++)
    {
        for (int j = 0; j < PIECE_SIZE; j++)
        {
            if (piece->piece_layout[i][j].rect_color != BLACK)
            {
                drawRect(piece->piece_layout[i][j]);
            }
        }
    }
}
void rotatePiece(piece_template *piece)
{
    piece->rot_state = (piece->rot_state + 1) % PIECE_ROT_NO;
    updatePiece(piece);
    SDL_Delay(KEY_DELAY);
}
int findPieceBound(piece_template *piece, short flag)
{
    int index = 0;
    while (index < PIECE_SIZE)
    {
        if (isRowColumnEmpty(flag, index, piece))
        {
            return index;
        }
        index++;
    }
    return PIECE_SIZE;
}
bool isRowColumnEmpty(int flag, int index, piece_template *piece)
{
    for (int i = 0; i < PIECE_SIZE; i++)
    {
        switch (flag)
        {
        case ROW_FLAG:
            if (piece->piece_layout[i][index].rect_color != BLACK)
            {
                return false;
            }
            break;
        case COL_FLAG:
            if (piece->piece_layout[index][i].rect_color != BLACK)
            {
                return false;
            }
            break;

        default:
            printf("Undefined behaviour\n");
            exit(1);
            break;
        }
    }
    return true;
}
void fastFall(piece_template *piece)
{
    while (checkMoveCollision(piece, dir[MV_DOWN]))
    {
        fallPiece(piece);
    }
    SDL_Delay(KEY_DELAY);
}
void dumpPiece(piece_template *dumped, int *score)
{
    int col_no = findPieceBound(dumped, COL_FLAG);
    int row_no = findPieceBound(dumped, ROW_FLAG);
    for (int i = 0; i < col_no; i++)
    {
        for (int j = 0; j < row_no; j++)
        {
            if (dumped->piece_layout[i][j].rect_color != BLACK)
            {
                grid[dumped->piece_position.x + i][dumped->piece_position.y + j].rect_color = RED;
            }
        }
    }
    *score += 1;
}
bool checkCollision(piece_template *test)
{
    int col_no = findPieceBound(test, COL_FLAG);
    int row_no = findPieceBound(test, ROW_FLAG);
    if (test->piece_position.x < 0 || test->piece_position.x + col_no > GRID_WITDH)
    {
        return false;
    }
    if (test->piece_position.y + row_no > GRID_HEIGHT)
    {
        return false;
    }
    for (int i = 0; i < col_no; i++)
    {
        for (int j = 0; j < row_no; j++)
        {
            if (test->piece_layout[i][j].rect_color != BLACK)
            {
                if (grid[test->piece_position.x + i][test->piece_position.y + j].rect_color == RED)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
void updatePiece(piece_template *piece)
{
    for (int i = 0; i < PIECE_SIZE; i++)
    {
        for (int j = 0; j < PIECE_SIZE; j++)
        {
            updateRectPos(piece, i, j);
            updateRectColor(piece, i, j, pieces[piece->piece_type][piece->rot_state][i][j]);
        }
    }
}
void welcomeMenu()
{
    gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
    gfx_textout(SCREEN_WIDTH / 2 - TEXT_X_CONST, SCREEN_HEIGTH / 2 - TEXT_Y_CONST,
                "Press any key to start a game", WHITE);
    gfx_updateScreen();
    gfx_getkey();
}
void drawRect(rect piece_rect)
{
    gfx_filledRect((piece_rect.left_upper.x * GRID_SQAURE_SIZE) + GRID_X_DISPLACEMENT,
                   (piece_rect.left_upper.y * GRID_SQAURE_SIZE) + GRID_Y_DISPLACEMENT,
                   (piece_rect.right_down.x * GRID_SQAURE_SIZE) + GRID_X_DISPLACEMENT,
                   (piece_rect.right_down.y * GRID_SQAURE_SIZE) + GRID_Y_DISPLACEMENT,
                   piece_rect.rect_color);
}
void drawGrid()
{
    gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
    for (int i = 0; i < GRID_WITDH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            drawRect(grid[i][j]);
        }
    }
}
void initializeGrid()
{
    for (int i = 0; i < GRID_WITDH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            grid[i][j] = (rect){{i, j}, {i + 1, j + 1}, BLACK};
        }
    }
}
void drawBoard()
{
    gfx_line((SCREEN_WIDTH / 2) - (GRID_WITDH / 2 * GRID_SQAURE_SIZE),
             SCREEN_HEIGTH - (GRID_HEIGHT * GRID_SQAURE_SIZE),
             (SCREEN_WIDTH / 2) - (GRID_WITDH / 2 * GRID_SQAURE_SIZE), SCREEN_HEIGTH, CYAN);
    gfx_line((SCREEN_WIDTH / 2) + (GRID_WITDH / 2 * GRID_SQAURE_SIZE),
             SCREEN_HEIGTH - (GRID_HEIGHT * GRID_SQAURE_SIZE),
             (SCREEN_WIDTH / 2) + (GRID_WITDH / 2 * GRID_SQAURE_SIZE), SCREEN_HEIGTH, CYAN);
}
void handleKeys(piece_template *piece)
{
    int key = gfx_pollkey();
    while (key != -1)
    {
        switch (key)
        {
        case SDLK_ESCAPE:
            exit(0);
            break;
        case SDLK_SPACE:
            if (checkRotCollision(piece))
            {
                rotatePiece(piece);
            }
            break;
        case SDLK_RIGHT:
            movePiece(dir[MV_RIGHT], piece);
            break;
        case SDLK_LEFT:
            movePiece(dir[MV_LEFT], piece);
            break;
        case SDLK_DOWN:
            fastFall(piece);
            break;
        }
        key = gfx_pollkey();
    }
}
void updatePiecePos(piece_template *piece)
{
    for (int i = 0; i < PIECE_SIZE; i++)
    {
        for (int j = 0; j < PIECE_SIZE; j++)
        {
            updateRectPos(piece, i, j);
        }
    }
}
void movePiece(point dir, piece_template *piece)
{
    if (checkMoveCollision(piece, dir))
    {
        switch (dir.x)
        {
        case -1:
            piece->piece_position.x -= 1;
            break;
        case 1:
            piece->piece_position.x += 1;
            break;
        default:
            break;
        }
    }
    updatePiecePos(piece);
    SDL_Delay(KEY_DELAY);
}
int rowToDelete()
{
    for (int i = GRID_HEIGHT - 1; i >= 0; i--)
    {
        if (isRowFull(i))
        {
            return i;
        }
    }
    return -1;
}
bool isRowFull(int index)
{
    for (int j = 0; j < GRID_WITDH; j++)
    {
        if (grid[j][index].rect_color != RED)
        {
            return false;
        }
    }
    return true;
}
void removeRow(int row_to_delete, int *score)
{
    for (int i = row_to_delete; i >= 0; i--)
    {
        for (int j = 0; j < GRID_WITDH; j++)
        {
            switch (i)
            {
            case 0:
                grid[j][i].rect_color = BLACK;
                break;
            default:
                grid[j][i].rect_color = grid[j][i - 1].rect_color;
                break;
            }
        }
    }
    *score += 10;
}
bool isGameOver()
{
    for (size_t i = 0; i < GRID_WITDH; i++)
    {
        if (grid[i][0].rect_color == RED)
            return true;
    }
    return false;
}
void gameOverMenu(int score)
{
    while (gfx_pollkey() == -1)
    {
        drawGrid();
        drawBoard();
        char score_message[BUFFER_SIZE];
        sprintf(score_message, "GAME OVER YOU SCORE IS: %d", score);
        gfx_textout(SCREEN_WIDTH / 2 - TEXT_X_CONST, SCREEN_HEIGTH / 2 - TEXT_Y_CONST,
                    score_message, WHITE);
        gfx_updateScreen();
    }
}
bool checkMoveCollision(piece_template *piece, point dir_vector)
{
    piece_template test = *piece;
    test.piece_position.x += dir_vector.x;
    test.piece_position.y += dir_vector.y;
    updatePiece(&test);

    return checkCollision(&test);
}
bool checkRotCollision(piece_template *piece)
{
    piece_template test = *piece;
    rotatePiece(&test);

    return checkCollision(&test);
}
