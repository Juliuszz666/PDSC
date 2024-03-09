/*JULIAN BEDNAREK 250247 2CS3*/
/*INCLUDES BEGIN*/
#include "primlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/*INCLUDES END*/
/*DEFINE BEGIN*/

/*Center of screen*/
#define CENTER_X gfx_screenWidth() / 2
#define CENTER_Y gfx_screenHeight() / 2
/*Rotation step in radians*/
#define ROT_STEP 0.01
/*Scalar to change size of polygon*/
#define SIZE_STEP 0.005
/*Range of vertices*/
#define MIN_VERTICES 3
#define MAX_VERTICES 9
#define CENTER_ANGLE(v) 2 * M_PI / (v)
/*Min and max distance from vertice to center*/
#define MIN_SIZE 50
#define MAX_SIZE 300
/*Colors used in program*/
#define BG_COLOR BLACK
/*These values are result of enum color type defined in "primlib.h"*/
#define MAX_COLOR 7
#define MIN_COLOR 1
/*1000/delay = FPS*/
#define DELAY 10
/*DEFINE END*/
/*STRUCTS BEGIN*/

/*Stuct for wrapping up vertices*/
typedef struct
{
    double x;
    double y;
} point;
/*Enum type for defining the growth state of the polygon*/
typedef enum
{
    INCREASING,
    DECREASING
} growing_state;

/*STRUCTS END*/
/*FUNCTION DECLARATIONS BEGIN*/

/* Function updates the position of vertices*/
/* @param vertice[] array of point structures to be updated*/
/* @param growth wheter function needs to increase or decrease size of polygon*/
/* @param vertice_count number of vertices to be updated*/
void updateVerticePos(point vertice[], growing_state growth, int vertice_count);

/* Function sets initial position of new polygon's vertices*/
/* @param vertice[] array of vertices*/
/* @param vertice_count number of vertices to be updated*/
void initializeNewPolygon(point vertice[], int vertics_count);

/* Function updates number of vertices when polygon reacher MIN or MAX*/
/* @param growth wheter number of vertices decreases of increased*/
/* @param vertice_count current vertice count*/
int updateVerticeCount(growing_state growth, int vertice_count);

/*FUNCTION DECLARATIONS END*/
/*MAIN FUNCTION BEGIN*/
int main(int argc, char *argv[])
{
    /*Initializing graphics*/
    if (gfx_init())
    {
        exit(3);
    }
    /*Initialization of vertices*/
    int vertice_count = MIN_VERTICES;
    point vertice[MAX_VERTICES];
    for (size_t i = 0; i < vertice_count; i++)
    {
        vertice[i].x = CENTER_X + cos(i * CENTER_ANGLE(vertice_count)) * MIN_SIZE;
        vertice[i].y = CENTER_Y + sin(i * CENTER_ANGLE(vertice_count)) * MIN_SIZE;
    }
    growing_state size_growth = INCREASING;
    growing_state vert_growth = INCREASING;
    enum color line_color = 1;

    /*INFINITE LOOP BEGIN*/
    while (1)
    {
        /*Clearing screen*/
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BG_COLOR);
        /*Drawing vertices*/
        for (size_t i = 0; i < vertice_count; i++)
        {
            if (i == vertice_count - 1)
                gfx_line((int)vertice[i].x, (int)vertice[i].y, (int)vertice[0].x, (int)vertice[0].y, line_color);
            else
                gfx_line((int)vertice[i].x, (int)vertice[i].y, (int)vertice[i + 1].x, (int)vertice[i + 1].y,
                         line_color);
        }
        gfx_updateScreen();
        SDL_Delay(DELAY);

        /*Setting growth of number of vertices*/
        if (vertice_count == MAX_VERTICES)
            vert_growth = DECREASING;
        if (vertice_count == MIN_VERTICES)
            vert_growth = INCREASING;
        /*Setting proper growth depending of size of the polygon*/
        if (hypot((vertice[0].x - CENTER_X), (vertice[0].y - CENTER_Y)) > MAX_SIZE - 1)
        {
            size_growth = DECREASING;
            if (line_color < MAX_COLOR)
                line_color++;
            else
                line_color = MIN_COLOR;
            vertice_count = updateVerticeCount(vert_growth, vertice_count);
            initializeNewPolygon(vertice, vertice_count);
        }
        if (hypot((vertice[0].x - CENTER_X), (vertice[0].y - CENTER_Y)) < MIN_SIZE - 1)
        {
            size_growth = INCREASING;
            if (line_color < MAX_COLOR)
                line_color++;
            else
                line_color = MIN_COLOR;
            vertice_count = updateVerticeCount(vert_growth, vertice_count);
            initializeNewPolygon(vertice, vertice_count);
        }

        updateVerticePos(vertice, size_growth, vertice_count);

        gfx_pollkey();
    }
    /*INFINITE LOOP END*/
    return 0;
}
/*MAIN FUNCTION END*/
/*FUNCTIONS BODIES BEGIN*/
void updateVerticePos(point vertice[], growing_state growth, int vertice_count)
{
    for (size_t i = 0; i < vertice_count; i++)
    {
        /*Increasing/decreasing size of polygon*/
        switch (growth)
        {
        case INCREASING:
            vertice[i].x += SIZE_STEP * (vertice[i].x - CENTER_X);
            vertice[i].y += SIZE_STEP * (vertice[i].y - CENTER_Y);
            break;
        case DECREASING:
            vertice[i].x -= SIZE_STEP * (vertice[i].x - CENTER_X);
            vertice[i].y -= SIZE_STEP * (vertice[i].y - CENTER_Y);
            break;
        default:
            printf("Undefined behaviour\n");
            exit(1);
            break;
        }

        /*Saving old values for proper rotation*/
        double oldX = vertice[i].x;
        double oldY = vertice[i].y;

        /*Rotating the vertices relative to center*/
        vertice[i].x = CENTER_X + ((oldX - CENTER_X) * cos(ROT_STEP)) - ((oldY - CENTER_Y) * sin(ROT_STEP));
        vertice[i].y = CENTER_Y + ((oldX - CENTER_X) * sin(ROT_STEP)) + ((oldY - CENTER_Y) * cos(ROT_STEP));
    }
}
void initializeNewPolygon(point vertice[], int vertice_count)
{
    for (size_t i = 0; i < vertice_count; i++)
    {
        /*2n+1 polygon are going from MIN to MAX and 2n from MAX to MIN*/
        switch (vertice_count % 2)
        {
        case 0:
            vertice[i].x = CENTER_X + cos(i * CENTER_ANGLE(vertice_count)) * (MAX_SIZE - 1);
            vertice[i].y = CENTER_Y + sin(i * CENTER_ANGLE(vertice_count)) * (MAX_SIZE - 1);
            break;
        case 1:
            vertice[i].x = CENTER_X + cos(i * CENTER_ANGLE(vertice_count)) * (MIN_SIZE - 1);
            vertice[i].y = CENTER_Y + sin(i * CENTER_ANGLE(vertice_count)) * (MIN_SIZE - 1);
            break;

        default:
            break;
        }
    }
}

int updateVerticeCount(growing_state vert_growth, int vertice_count)
{
    switch (vert_growth)
    {
    case INCREASING:
        return ++vertice_count;
        break;
    case DECREASING:
        return --vertice_count;
        break;
    default:
        printf("Undefined behaviour\n");
        exit(1);
        break;
    }
}
/*FUNCTIONS BODIES END*/