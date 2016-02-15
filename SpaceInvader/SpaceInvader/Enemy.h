#pragma once

using namespace std;

#include "standardLoad.h"
#include "imgLoader.h"
#include <random>

class Enemy
{
	private:
		int alien1_points;
		int alien2_points;
		int alien3_points;

		SDL_Surface* bulImg;
		SDL_Rect bulPos;

		int direction;

		enum direction_t {left, right};

		struct bullets_t {
			bool fire;
			SDL_Rect box;
		};

		struct enemy_t {
			SDL_Rect boundBox;
			bool alive;
			SDL_Surface* img_;
			SDL_Surface* screen_;
			int points;
			struct bullets_t bullet;
		};

		struct invaders_t {
			SDL_Rect boundBox;
			enum direction_t direction;
		};

		int rand_row;
		int rand_col;

		imgLoader img_;

	public:

		int SHOOTING_CHANCE;
		int SPEED;

		enemy_t enemy[5][10];

		invaders_t invaders;

		Enemy(void);
		~Enemy(void);

		void init(SDL_Surface* screen, int Enemy_Width, int Enemy_Height);

		void move(int MAX_LEFT, int MAX_RIGHT, int speed);

		void move_down();

		void fire();

		void updateBullets();

		void destroy();
};

