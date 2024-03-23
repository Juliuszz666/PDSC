#include "primlib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGTH gfx_screenHeight()
#define DISC_NO 5
#define DISC_HEIGHT ((500 / DISC_NO) > 20 ? 20 : (500 / DISC_NO))
#define PEG_NO 10
#define MAX_PEG_NO 10
#define DISC_WIDTH_MAX (SCREEN_WIDTH / ((3 * PEG_NO) + 1))
#define DISC_WIDTH_MIN (DISC_WIDTH_MAX / 3)
#define DISC_COLOR BLUE
#define PEG_WIDTH 10
#define PEG_SPAWN_H_CO (15.0 / 16)
#define PEG_COLOR YELLOW
#define PEG_HEIGHT (DISC_HEIGHT * (DISC_NO + 2))
#define ANIMATION_UP_DOWN_HEIGHT                                               \
	((int)(SCREEN_HEIGTH * PEG_SPAWN_H_CO) - PEG_HEIGHT - 100)
#define STACK_SIZE DISC_NO
#define ANIMATION_STEP 1
#define KEY_CONST 48
#define ANIMATION_DELAY 3

typedef struct {
	double x;
	double y;
} point;
typedef struct {
	point left_upper;
	point right_down;
} rect;

int top[PEG_NO] = {0};
rect pegs[PEG_NO];
rect stacks[PEG_NO][DISC_NO];
rect null_rect = {{0, 0}, {0, 0}};

void animation(rect disc);
void checkForQuit();
int signOfDifference(int num1, int num2);
int checkKey(int key);
void initializePegs();
void initializeDiscs();
void drawDiscs();
void drawPegs();
void renderGame();
rect popDisc(int index);
rect getTop(int index);
void pushDisc(rect disc, int index);
void handleMovement(int src, int dest);
void animateMovement(rect disc, int start, int end);
void moveUp(rect* disc);
void moveLeftRight(rect* disc, int direction, int end);
void moveDown(rect* disc, int end);
void printMessage();
bool isKeyUsed(int key);
bool notNullRect(rect disc);
void gameLoop();
bool isLegalMove(int index, rect disc);
bool isWonOrLost();

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	initializePegs();
	initializeDiscs();
	gameLoop();
	printMessage();

	return 0;
}
void handleMovement(int src, int dest)
{
	int pop_index = src;
	int push_index = dest;
	if (pop_index != -1 && push_index != -1) {
		pop_index == 0 ? pop_index = PEG_NO - 1 : --pop_index;
		push_index == 0 ? push_index = PEG_NO - 1 : --push_index;
		rect to_be_popped = getTop(pop_index);
		if (isLegalMove(push_index, to_be_popped)) {
			rect popped = popDisc(pop_index);
			animateMovement(popped, pop_index, push_index);
			pushDisc(popped, push_index);
		}
	}
}
void animateMovement(rect disc, int start, int end)
{
	rect* moving_disc = &disc;
	moveUp(moving_disc);
	int direction = signOfDifference(start, end);
	moveLeftRight(moving_disc, direction, end);
	moveDown(moving_disc, end);
}
void moveUp(rect* disc)
{
	while ((int)disc->left_upper.y != ANIMATION_UP_DOWN_HEIGHT) {
		animation(*disc);
		checkForQuit();
		disc->left_upper.y -= ANIMATION_STEP;
		disc->right_down.y -= ANIMATION_STEP;
	}
}
void moveLeftRight(rect* disc, int direction, int end)
{
	int disc_center =
		disc->right_down.x - ((fabs(disc->left_upper.x - disc->right_down.x)) / 2);
	int end_peg_center = (int)pegs[end].right_down.x - (PEG_WIDTH / 2);
	while (disc_center != end_peg_center) {
		animation(*disc);
		checkForQuit();
		disc->left_upper.x += (ANIMATION_STEP * direction);
		disc->right_down.x += (ANIMATION_STEP * direction);
		disc_center += (ANIMATION_STEP * direction);
	}
}
void moveDown(rect* disc, int end)
{
	while ((int)disc->left_upper.y !=
		   (int)pegs[end].right_down.y - (DISC_HEIGHT * (top[end] + 1))) {
		animation(*disc);
		checkForQuit();
		disc->left_upper.y += ANIMATION_STEP;
		disc->right_down.y += ANIMATION_STEP;
	}
}
void checkForQuit()
{
	if (gfx_pollkey() == SDLK_ESCAPE) {
		exit(3);
	}
}
bool isLegalMove(int index, rect disc)
{
	double on_stack_width;
	if (!notNullRect(disc)) {
		return false;
	}
	on_stack_width = (top[index] == 0)
						 ? SCREEN_WIDTH
						 : fabs(stacks[index][top[index] - 1].right_down.x -
								stacks[index][top[index] - 1].left_upper.x);
	double disc_witdh = fabs(disc.right_down.x - disc.left_upper.x);
	return (disc_witdh < on_stack_width) ? true : false;
}
void initializePegs()
{
	for (size_t i = 0; i < PEG_NO; i++) {
		pegs[i].right_down.x =
			((SCREEN_WIDTH / (PEG_NO + 1)) * (i + 1)) + PEG_WIDTH / 2;
		pegs[i].left_upper.x = pegs[i].right_down.x - PEG_WIDTH;
		pegs[i].right_down.y = SCREEN_HEIGTH * PEG_SPAWN_H_CO;
		pegs[i].left_upper.y = pegs[i].right_down.y - PEG_HEIGHT;
	}
}
void initializeDiscs()
{
	for (size_t i = 0; i < DISC_NO; i++) {
		double disc_width =
			DISC_WIDTH_MAX -
			((double)(DISC_WIDTH_MAX - DISC_WIDTH_MIN) / DISC_NO) * i;
		double peg_center = pegs[0].right_down.x - (PEG_WIDTH / 2);
		stacks[0][i].right_down.x = peg_center + disc_width;
		stacks[0][i].left_upper.x = peg_center - disc_width;
		stacks[0][i].right_down.y = pegs[0].right_down.y - (DISC_HEIGHT * i);
		stacks[0][i].left_upper.y =
			pegs[0].right_down.y - (DISC_HEIGHT * (i + 1));
		top[0]++;
	}
}
rect popDisc(int index)
{
	return (top[index] > 0) ? stacks[index][--top[index]] : null_rect;
}
void pushDisc(rect disc, int index)
{
	if ((top[index] < STACK_SIZE) && notNullRect(disc)) {
		double disc_width = fabs((disc.left_upper.x - disc.right_down.x) / 2);
		int peg_center = pegs[index].right_down.x - (PEG_WIDTH / 2);
		disc.right_down.x = peg_center + disc_width;
		disc.left_upper.x = peg_center - disc_width;
		disc.right_down.y =
			pegs[index].right_down.y - (DISC_HEIGHT * top[index]);
		disc.left_upper.y =
			pegs[index].right_down.y - (DISC_HEIGHT * (top[index] + 1));
		stacks[index][top[index]++] = disc;
	}
}
void drawDiscs()
{
	short peg_index = 0;
	while (peg_index < PEG_NO) {
		short disc_index = 0;
		while (disc_index < top[peg_index]) {
			gfx_filledRect(stacks[peg_index][disc_index].left_upper.x,
						   stacks[peg_index][disc_index].left_upper.y,
						   stacks[peg_index][disc_index].right_down.x,
						   stacks[peg_index][disc_index].right_down.y,
						   DISC_COLOR);
			disc_index++;
		}
		peg_index++;
	}
}
void drawPegs()
{
	for (size_t i = 0; i < PEG_NO; i++) {
		gfx_filledRect(pegs[i].left_upper.x, pegs[i].left_upper.y,
					   pegs[i].right_down.x, pegs[i].right_down.y, PEG_COLOR);
	}
}
bool isKeyUsed(int key)
{
	switch (PEG_NO) {
	case MAX_PEG_NO:
		return (key > KEY_CONST - 1 && key < KEY_CONST + PEG_NO);
		break;
	default:
		return (key > KEY_CONST && key <= KEY_CONST + PEG_NO);
		break;
	}
}
bool notNullRect(rect disc)
{
	if (disc.left_upper.x != 0) {
		return true;
	}
	return false;
}
int checkKey(int key)
{
	switch (key) {
	case SDLK_ESCAPE:
		exit(3);
		break;
	case SDLK_RETURN:
		return key;
		break;
	default:
		return isKeyUsed(key) ? (key - KEY_CONST) : -1;
		break;
	}
}
rect getTop(int index)
{
	return (top[index] > 0) ? stacks[index][top[index] - 1] : null_rect;
}
void renderGame()
{
	gfx_filledRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGTH - 1, BLACK);
	gfx_filledRect(0, (SCREEN_HEIGTH * PEG_SPAWN_H_CO)+1, SCREEN_WIDTH - 1,
				   SCREEN_HEIGTH - 1, GREEN);
	drawPegs();
	drawDiscs();
}
int signOfDifference(int num1, int num2)
{
	return (num1 - num2 <= 0) ? 1 : -1;
}
bool isWonOrLost()
{
	for (size_t i = 1; i < PEG_NO; i++) {
		if (top[i] == DISC_NO) {
			return true;
		}
	}
	return false;
}
void printMessage()
{
	while (gfx_pollkey() == -1) {
		const char* message = isWonOrLost() ? "You won!" : "You lost!";
		renderGame();
		gfx_textout(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2, message, WHITE);
		gfx_updateScreen();
	}
}
void animation(rect disc)
{
	renderGame();
	gfx_filledRect(disc.left_upper.x, disc.left_upper.y, disc.right_down.x,
				   disc.right_down.y, DISC_COLOR);
	gfx_updateScreen();
	SDL_Delay(ANIMATION_DELAY);
}
void gameLoop()
{
	while (1) {
		renderGame();
		gfx_updateScreen();
		int source = gfx_getkey();
		source = checkKey(source);
		if (source == SDLK_RETURN) {
			break;
		}
		int dest = gfx_getkey();
		dest = checkKey(dest);
		if (dest == SDLK_RETURN) {
			break;
		}
		handleMovement(source, dest);
	}
}
