#ifndef __PRIMLIB_H__
#define __PRIMLIB_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
int gfx_init();
enum color { BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX };
void gfx_pixel(int x, int y, enum color c);
void gfx_line(int x1, int y1, int x2, int y2, enum color c);
void gfx_circle(int x, int y, int r, enum color c);
void gfx_filledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, enum color c);
void gfx_filledRect(int x1, int y1, int x2, int y2, enum color c);
void gfx_filledCircle(int x, int y, int r, enum color c);
void gfx_rect(int x1, int y1, int x2, int y2, enum color c);
void gfx_textout(int x, int y, const char* s, enum color c);
int gfx_screenWidth();
int gfx_screenHeight();
void gfx_updateScreen();
int gfx_pollkey();
int gfx_getkey();
int gfx_isKeyDown(int key);

#endif /* __PRIMLIB_H__ */
