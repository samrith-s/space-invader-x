#include "GameManager.h"


GameManager::GameManager(void) {}
GameManager::~GameManager(void) {}

/*---------------------------

	FUNCTION TO INIT SDL2

------------------------------*/
bool GameManager::initSDL() 
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL could not be initialised: ", SDL_GetError());
		return false;
	}
	else
		return true;
}


/*---------------------------

	FUNCTION TO INIT SDL_img

------------------------------*/
bool GameManager::initSDL_img() {
	if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		return true;
	else {
		printf("Error initialising IMG_Init(PNG/JPG). Reason: ", IMG_GetError());
		return false;
	}
}

/*---------------------------

	FUNCTION TO INIT SDL_ttf

------------------------------*/
bool GameManager::initSDL_ttf() {
		
	if(TTF_Init()!=0) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return false;
	}
	else
		return true;
}

/*---------------------------

	FUNCTION TO check initialization

------------------------------*/
bool GameManager::initialize(char* title, int SCREEN_WIDTH, int SCREEN_HEIGHT) {

	if(initSDL())
		w.createWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	else
		exit(2);

	if(!initSDL_img())
		exit(2);

	if(!initSDL_ttf())
		exit(2);

	bgImg = img_.optimizedLoad("Resources/background.jpg", w.getWindowSurface());

	e.SHOOTING_CHANCE = 101;
	e.SPEED = 3;

	SCREEN_W = SCREEN_WIDTH;
	SCREEN_H = SCREEN_HEIGHT;

	ENEMY_COUNT = 0;
	SCORE = 0;
	LIFE = 3;

	p.player.lives = LIFE;

	state = menu;

	renderer = SDL_CreateRenderer(w.getWindow(), -1, SDL_RENDERER_ACCELERATED);

	next_game_tick = SDL_GetTicks();

	return true;
}

/*---------------------------

	FUNCTION runs the game

------------------------------*/
void GameManager::run()
{
	running = true;
	loadObjects();
	clrscr();
	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			running = w.windowRunning(event);

			switch(state)
			{
				case menu:
					menuEvents(event);
					break;

				case game:
					playEvents(event);
					break;

				case gameOver:
					break;

				case newLvl:
					ENEMY_COUNT = 0;
					e.SPEED++;
					while(e.SPEED > 7 || e.SPEED < 3)
						e.SPEED = rand()%5;
					e.SHOOTING_CHANCE-=10;
					while(e.SHOOTING_CHANCE < 20 || e.SHOOTING_CHANCE > 101)
						e.SHOOTING_CHANCE = rand()%101;
					playEvents(event);
					break;

			}
		}

		if(state==menu)
			menuScreen();
		if(state==game)
			play();
		if(state==newLvl)
			play();
		if(state==gameOver) {
			std::cout << "Game over! You ran out of lives!" << std::endl;
			running = false;
		}

		next_game_tick += 1000 / 30;
		sleep = next_game_tick - SDL_GetTicks();
		
		if(sleep >= 0)
			SDL_Delay(sleep);
	}

	w.destroyWindow();
}

/*---------------------------

	Draw menu screen

------------------------------*/
void GameManager::menuScreen()
{
	clrscr();

	SDL_Surface* optImg_ = img_.optimizedLoad("Resources/logo.png", w.getWindowSurface());
	SDL_Surface* optImg_2 = img_.optimizedLoad("Resources/space_to_start.gif", w.getWindowSurface());

	SDL_Rect logoPos, stsPos;

	logoPos.h = 300;
	logoPos.w = 500;
	logoPos.x = (SCREEN_W - logoPos.w)/2;
	logoPos.y = 200;
	img_.renderToScreen(optImg_, &logoPos, NULL, w.getWindowSurface());

	stsPos.h = 100;
	stsPos.w = 500;
	stsPos.x = (SCREEN_W - logoPos.w)/2;
	stsPos.y = logoPos.y + logoPos.h + 50;
	img_.renderToScreen(optImg_2, &stsPos, NULL, w.getWindowSurface());

	SDL_UpdateWindowSurface(w.getWindow());
}

/*---------------------------

	Handle menu events

------------------------------*/
void GameManager::menuEvents(SDL_Event event)
{
	if(event.type == SDL_KEYUP)
		if(event.key.keysym.sym == SDLK_SPACE)
			state = game;
}

/*---------------------------

	Load game objects

------------------------------*/
void GameManager::loadObjects()
{
	p.init(w.getWindowSurface(), SCREEN_W, SCREEN_H, "center", 64, 64, 7);
	e.init(w.getWindowSurface(), 45, 45);

	SDL_UpdateWindowSurface(w.getWindow());
}

/*---------------------------

	Handles playing events

------------------------------*/
void GameManager::playEvents(SDL_Event event)
{
	p.handleEvents(event, 0, SCREEN_W);
}

/*---------------------------

	Handles animations while state=game

------------------------------*/
void GameManager::play()
{
	//draw_string("Press Spacebar to Start!", 200, 300);

	if(p.player.alive && state!=paused) {
		EnemyPlayerBulletCollide();
		PlayerEnemyBulletCollide();
		EnemyPlayerCollide();
		clrscr();

		e.move(0, SCREEN_W, e.SPEED);
		p.update();

		SDL_Rect livesPos;
		for(int k=1; k<=p.player.lives; k++) {
			livesPos.w = 35;
			livesPos.h = 35;
			livesPos.x = (SCREEN_W - (k*livesPos.w)) - (k*5);
			livesPos.y = 10;

			img_.renderToScreen(p.player.img_, &livesPos, NULL, w.getWindowSurface());
		}

		if(ENEMY_COUNT == 50) {
			if(p.player.lives < LIFE)
				p.player.lives++;
			state = newLvl;
			loadObjects();
		}
		SDL_UpdateWindowSurface(w.getWindow());
	}
	else {
		if(p.player.lives > 0) {
			SDL_Delay(500);
			p.player.alive = !p.player.alive;
			p.player.lives--;
		}
		else
			state = gameOver;
	}
}

/*---------------------------

	Enemy collision with player bullet

------------------------------*/
void GameManager::EnemyPlayerBulletCollide() {
	int i, j;

	for(i=0; i<5; i++) {
			for(j=0; j<10; j++) {
				if (p.player.bullet.fire && e.enemy[i][j].alive 
					&& p.player.bullet.box.x >= e.enemy[i][j].boundBox.x 
					&& p.player.bullet.box.x <= e.enemy[i][j].boundBox.x + e.enemy[i][j].boundBox.w 
					&& p.player.bullet.box.y <= e.enemy[i][j].boundBox.y +  e.enemy[i][j].boundBox.h 
					&& p.player.bullet.box.y >= e.enemy[i][j].boundBox.y) {
						e.enemy[i][j].alive = false;
						p.player.bullet.fire = false;
						SCORE += e.enemy[i][j].points;
						std::cout<< "Current Score: " << SCORE << std::endl;
						ENEMY_COUNT++;
				}
			}
		}
}

/*---------------------------

	player collision with enemy bullet

------------------------------*/

void GameManager::PlayerEnemyBulletCollide() {
	int i, j;
	
	for(i=0; i<5; i++) {
			for(j=0; j<10; j++) {
				if (e.enemy[i][j].bullet.fire
					&& e.enemy[i][j].bullet.box.x >= p.player.box.x 
					&& e.enemy[i][j].bullet.box.x <= p.player.box.x + p.player.box.w 
					&& e.enemy[i][j].bullet.box.y <= p.player.box.y +  p.player.box.h 
					&& e.enemy[i][j].bullet.box.y >= p.player.box.y) {
						p.player.alive = false;
						e.enemy[i][j].bullet.fire = false;
				}
			}
		}

}

/*---------------------------

	Enemy and player collision

------------------------------*/
void GameManager::EnemyPlayerCollide() {

	int i, j;

	for(i=0; i<5; i++)
		for(j=0; j<10; j++) {
			if(e.enemy[i][j].alive && e.enemy[i][j].boundBox.y + e.enemy[i][j].boundBox.h >= p.player.box.y)
				p.player.alive = false;
		}
}

void GameManager::clrscr()
{
	SDL_Rect bgRect = {0, 0, SCREEN_W, SCREEN_H};
	img_.renderToScreen(bgImg, &bgRect, NULL, w.getWindowSurface());
}