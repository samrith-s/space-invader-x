#include "Player.h"


Player::Player(void) {}

Player::~Player(void) {}

void Player::init(SDL_Surface* screen, int SCREEN_WIDTH, int SCREEN_HEIGHT, char* pos, int w, int h, int speed)
{
	playerPos.w = w;
	playerPos.h = h;

	if(pos=="center")
		playerPos.x = (SCREEN_WIDTH + playerPos.w)/2;
	if(pos=="left")
		playerPos.x = 5;
	if(pos=="right")
		playerPos.x = (SCREEN_WIDTH - playerPos.w) - 5;

	playerPos.y = (SCREEN_HEIGHT - playerPos.h) - 10;
	
	player.box = playerPos;

	player.alive = true;
	player.speed = speed;

	player.bullet.fire = false;

	player.img_ = img_.optimizedLoad("Resources/player.gif", screen);

	bulImg = img_.optimizedLoad("Resources/bullet_player.gif", screen);

	img_.renderToScreen(player.img_, &playerPos, NULL, screen);

	player.screen = screen;

}

void Player::handleEvents(SDL_Event event, int MAX_LEFT, int MAX_RIGHT)
{
	if(player.alive) {

		if(event.type==SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_LEFT && playerPos.x > MAX_LEFT)
				playerPos.x-=player.speed;
			else if(event.key.keysym.sym == SDLK_RIGHT && playerPos.x + playerPos.w < MAX_RIGHT)
				playerPos.x+=player.speed;

			player.box = playerPos;
		}

		if(event.type==SDL_KEYUP) {
			if(event.key.keysym.sym == SDLK_SPACE && !player.bullet.fire) {
				bulPos.x = playerPos.x + (playerPos.w/2);
				bulPos.y = playerPos.y - 40;
				bulPos.w = 5;
				bulPos.h = 20;

				player.bullet.box = bulPos;
				img_.renderToScreen(bulImg, &player.bullet.box, NULL, player.screen);
				player.bullet.fire = !player.bullet.fire;
			}
		}
	}
}

void Player::update()
{
	if(player.alive) {
		img_.renderToScreen(player.img_, &playerPos, NULL, player.screen);
		if(player.bullet.fire) {
			if(player.bullet.box.y > 0) {
				player.bullet.box.y-=10;
				img_.renderToScreen(bulImg, &player.bullet.box, NULL, player.screen);
			}
			else
				player.bullet.fire = !player.bullet.fire;
		}
	}
}
