/*JULIAN BEDNAREK 250247 2CS3*/
#include "primlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define CENTER_X (gfx_screenWidth() / 2)
#define CENTER_Y (gfx_screenHeight() / 2)
#define ROT_STEP 0.01
#define SIZE_STEP 0.005
#define VERTICES 5
#define CENTER_ANGLE (2 * M_PI / VERTICES)
#define MIN_SIZE 50
#define MAX_SIZE 300
#define BG_COLOR BLACK
#define MAX_COLOR 7
#define MIN_COLOR 1
#define DELAY 10

typedef struct
{
    double x;
    double y;
} point;

typedef enum
{
    INCREASING,
    DECREASING
} growing_state;

void initializeVertices(point vertice[]);
void updateVertices(point vertice[], growing_state growth);
void drawVertices(point vertice[], enum color line_color);
enum color updateColor(enum color line_color);
point updateSize(point vertice);
point rotateVertice(point vertice);

int main(int argc, char *argv[])
{
    if (gfx_init())
    {
        exit(3);
    }

    point vertice[VERTICES];
    growing_state growth = INCREASING;
    enum color line_color = 1;

    initializeVertices(vertice);

    while (1)
    {

        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BG_COLOR);
        drawVertices(vertice, line_color);
        gfx_updateScreen();
        SDL_Delay(DELAY);

        int current_size = (int)hypot((vertice[0].x - CENTER_X), (vertice[0].y - CENTER_Y));
        if (current_size > MAX_SIZE)
        {
            growth = DECREASING;
            line_color = updateColor(line_color);
        }
        if (current_size < MIN_SIZE)
        {
            growth = INCREASING;
            line_color = updateColor(line_color);
        }

        updateVertices(vertice, growth);

        int key = gfx_pollkey();
        if (key == SDLK_ESCAPE)
            break;
    }
    return 0;
}
void updateVertices(point vertice[], growing_state growth)
{
    for (size_t i = 0; i < VERTICES; i++)
    {
        point size_growth = updateSize(vertice[i]);
        switch (growth)
        {
        case INCREASING:
            vertice[i].x += size_growth.x;
            vertice[i].y += size_growth.y;
            break;
        case DECREASING:
            vertice[i].x -= size_growth.x;
            vertice[i].y -= size_growth.y;
            break;
        default:
            printf("Undefined behaviour\n");
            exit(1);
            break;
        }
        vertice[i] = rotateVertice(vertice[i]);
    }
}
enum color updateColor(enum color line_color)
{
    if (line_color < MAX_COLOR)
        line_color++;
    else
        line_color = MIN_COLOR;
    return line_color;
}
point updateSize(point vertice)
{
    point growth;
    growth.x = SIZE_STEP * (vertice.x - CENTER_X);
    growth.y = SIZE_STEP * (vertice.y - CENTER_Y);
    return growth;
}
point rotateVertice(point vertice)
{
    point old_value = vertice;
    vertice.x = CENTER_X + ((old_value.x - CENTER_X) * cos(ROT_STEP)) - ((old_value.y - CENTER_Y) * sin(ROT_STEP));
    vertice.y = CENTER_Y + ((old_value.x - CENTER_X) * sin(ROT_STEP)) + ((old_value.y - CENTER_Y) * cos(ROT_STEP));
    return vertice;
}
void initializeVertices(point vertice[])
{
    for (size_t i = 0; i < VERTICES; i++)
    {
        vertice[i].x = CENTER_X + cos(i * CENTER_ANGLE) * MIN_SIZE;
        vertice[i].y = CENTER_Y + sin(i * CENTER_ANGLE) * MIN_SIZE;
    }
}
void drawVertices(point vertice[], enum color line_color)
{
    for (size_t i = 0; i < VERTICES; i++)
    {
        switch (i)
        {
        case VERTICES - 1:
            gfx_line((int)vertice[i].x, (int)vertice[i].y, (int)vertice[0].x, (int)vertice[0].y, line_color);
            break;
        default:
            gfx_line((int)vertice[i].x, (int)vertice[i].y, (int)vertice[i + 1].x, (int)vertice[i + 1].y, line_color);
            break;
        }
    }
}