#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "primlib.h"

static SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;

struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

static struct RGB colors[COLOR_MAX] = {
	{0, 0, 0},	   {255, 0, 0},	  {0, 255, 0},	 {0, 0, 255},
	{0, 255, 255}, {255, 0, 255}, {255, 255, 0}, {255, 255, 255}};

// Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 700;

void gfx_pixel(int x, int y, enum color c)
{
	pixelRGBA(renderer, x, y, colors[c].r, colors[c].g, colors[c].b, 255);
}

void gfx_line(int x1, int y1, int x2, int y2, enum color c)
{
	assert(c < COLOR_MAX);
	lineRGBA(renderer, x1, y1, x2, y2, colors[c].r, colors[c].g, colors[c].b,
			 255);
}

void gfx_filledTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
						enum color c)
{
	assert(c < COLOR_MAX);
	Sint16 xtab[] = {x1, x2, x3};
	Sint16 ytab[] = {y1, y2, y3};
	filledPolygonRGBA(renderer, xtab, ytab, 3, colors[c].r, colors[c].g,
					  colors[c].b, 255);
}

void gfx_rect(int x1, int y1, int x2, int y2, enum color c)
{
	assert(c < COLOR_MAX);
	rectangleRGBA(renderer, x1, y1, x2, y2, colors[c].r, colors[c].g,
				  colors[c].b, 255);
}

void gfx_filledRect(int x1, int y1, int x2, int y2, enum color c)
{
	assert(c < COLOR_MAX);
	boxRGBA(renderer, x1, y1, x2, y2, colors[c].r, colors[c].g, colors[c].b,
			255);
}

void gfx_circle(int x, int y, int r, enum color c)
{
	assert(c < COLOR_MAX);
	circleRGBA(renderer, x, y, r, colors[c].r, colors[c].g, colors[c].b, 255);
}

void gfx_filledCircle(int x, int y, int r, enum color c)
{
	assert(c < COLOR_MAX);
	filledCircleRGBA(renderer, x, y, r, colors[c].r, colors[c].g, colors[c].b,
					 255);
}

int gfx_screenWidth()
{
	return SCREEN_WIDTH;
}

int gfx_screenHeight()
{
	return SCREEN_HEIGHT;
}

void gfx_updateScreen()
{
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

void gfx_textout(int x, int y, const char* s, enum color c)
{
	assert(c < COLOR_MAX);
	stringRGBA(renderer, x, y, s, colors[c].r, colors[c].g, colors[c].b, 255);
}

int gfx_pollkey()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			return event.key.keysym.sym;
		case SDL_QUIT:
			exit(3);
		}
	}
	return -1;
}

int gfx_getkey()
{
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYDOWN)
			break;
		if (event.type == SDL_QUIT)
			exit(3);
	};
	return event.key.keysym.sym;
}

int gfx_isKeyDown(int key)
{
	const Uint8* keytable;
	int numkeys;
	SDL_PumpEvents();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(3);
		}
	}
	keytable = SDL_GetKeyboardState(&numkeys);
	SDL_Scancode code = SDL_GetScancodeFromKey(key);
	assert(code < numkeys);
	return keytable[code];
}

static void gfx_close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int gfx_init()
{

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(gfx_close);

	window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
							  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "Window could not be created! SDL Error: %s\n",
				SDL_GetError());
		return 2;
	}
	else {
		// Create renderer for window
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (renderer == NULL) {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
		}

		if (renderer == NULL) {
			fprintf(stderr, "Renderer could not be created! SDL Error: %s\n",
					SDL_GetError());
			return 3;
		}
	}

	SDL_Delay(10);
	return 0;
}
