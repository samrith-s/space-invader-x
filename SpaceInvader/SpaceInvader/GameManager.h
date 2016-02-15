#pragma once

#include "standardLoad.h"
#include "imgLoader.h"
#include "Window.h"
#include "Player.h"
#include "Enemy.h"

class GameManager
{
	private:
		bool running;
		SDL_Event event;

		Uint32 next_game_tick;
		int sleep;

		Window w;
		Player p;
		Enemy e;

		imgLoader img_;

		int SCORE;
		int LIFE;
		int ENEMY_COUNT;

		SDL_Surface* bgImg;

		enum state_t {menu, game, paused, newLvl, gameOver};
		
		SDL_Renderer* renderer;

		SDL_Surface* cmap;

	public:

		state_t state;
		int SCREEN_W;
		int SCREEN_H;

		GameManager(void);
		~GameManager(void);

		bool initialize(char* title, int SCREEN_WIDTH, int SCREEN_HEIGHT);
		bool initSDL();
		bool initSDL_img();
		bool initSDL_ttf();

		void menuEvents(SDL_Event event);
		void menuScreen();

		void loadObjects();
		void playEvents(SDL_Event event);
		void play();

		void EnemyPlayerBulletCollide();
		void PlayerEnemyBulletCollide();
		void EnemyPlayerCollide();

		void run();

		void clrscr();
};

