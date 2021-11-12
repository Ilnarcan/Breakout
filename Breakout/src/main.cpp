#include "game.h"

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

int main(int argc, char* argv[])
{
	Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");

	Breakout.init();
	Breakout.start();

	return 0;
}
