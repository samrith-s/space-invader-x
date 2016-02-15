
using namespace std;

#include "GameManager.h"

char* title = "Space Invaders- CLASSIC";
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 860;

enum state_t {menu, play, pause} state;

int main( int argc, char*argv[]) {

	GameManager game;

	game.initialize(title, SCREEN_WIDTH, SCREEN_HEIGHT); 

	game.run();

	SDL_Quit();
	return 0;
}