#pragma once

#include "standardLoad.h"
#include "imgLoader.h"

class Player
{
	private:
		SDL_Rect playerPos;
		SDL_Surface* playerSurface;
		imgLoader img_;

		struct bullet_t {
			SDL_Rect box;
			bool fire;
		};

		struct player_t {
			bool alive;
			int speed;
			SDL_Surface* img_;
			SDL_Surface* screen;
			struct bullet_t bullet;
			SDL_Rect box;
			int lives;
		};

		SDL_Surface* bulImg;
		SDL_Rect bulPos;

	public:

		struct player_t player;

		Player(void);
		~Player(void);

		void init(SDL_Surface* screen, int SCREEN_WIDTH, int SCREEN_HEIGHT, char* pos, int w, int h, int speed);

		void handleEvents(SDL_Event event, int MAX_LEFT, int MAX_RIGHT);

		void update();

		void destroy();
};

