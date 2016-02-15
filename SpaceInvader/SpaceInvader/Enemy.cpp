#include "Enemy.h"
#include <cstdlib>
#include <ctime>


Enemy::Enemy(void) { alien1_points = 10; alien2_points = 30; alien3_points = 50; }

Enemy::~Enemy(void) {}

void Enemy::init(SDL_Surface* screen, int Enemy_Width, int Enemy_Height) 
{
	std::srand(std::time(NULL));

	int i, j;

	imgLoader img_;

	int direction = 0;

	SDL_Surface* alien1 = img_.optimizedLoad("Resources/alien_level_1.gif", screen);
	SDL_Surface* alien2 = img_.optimizedLoad("Resources/alien_level_2.gif", screen);
	SDL_Surface* alien3 = img_.optimizedLoad("Resources/alien_level_3.gif", screen);
	invaders.boundBox.w = 0;
	invaders.boundBox.h = 0;

	for(i=0;i<5;i++)
		for(j=0;j<10;j++) {
			enemy[i][j].alive = true;
			enemy[i][j].boundBox.h = Enemy_Width;
			enemy[i][j].boundBox.w = Enemy_Height;
			enemy[i][j].boundBox.x = (j*Enemy_Width) + (j*5);
			enemy[i][j].boundBox.y = ((i+1)*Enemy_Height) + 35 + (i*5);

			if(i<1) {
				enemy[i][j].img_ = alien3;
				enemy[i][j].points = alien3_points;
			}
			if(i>=1 && i <3) {
				enemy[i][j].img_ = alien2;
				enemy[i][j].points = alien2_points;
			}
			if(i>=3) {
				enemy[i][j].img_ = alien1;
				enemy[i][j].points = alien1_points;
			}

			enemy[i][j].screen_ = screen;
			enemy[i][j].bullet.fire = false;

			invaders.boundBox.w = enemy[i][j].boundBox.x;
			invaders.boundBox.h = enemy[i][j].boundBox.y;

			img_.renderToScreen(enemy[i][j].img_, &enemy[i][j].boundBox, NULL, screen);
		}

	invaders.boundBox.w += Enemy_Width;
	invaders.boundBox.h += Enemy_Height;
	invaders.direction = right;
	invaders.boundBox.x = 0;
	invaders.boundBox.y = enemy[0][0].boundBox.y;

	bulImg = img_.optimizedLoad("Resources/bullet_enemy.gif", screen);
}

void Enemy::move(int MAX_LEFT, int MAX_RIGHT, int speed) 
{

	if(invaders.boundBox.x  < MAX_LEFT) {
		invaders.direction = right;
		move_down();
	}
	if(invaders.boundBox.x + invaders.boundBox.w > MAX_RIGHT) {
		invaders.direction = left;
		move_down();
	}

	int i,j;

	switch(invaders.direction) {

		case left:
			for(i=0; i<5; i++)
				for(j=0; j<10; j++)
					if(enemy[i][j].alive) {
						enemy[i][j].boundBox.x-=speed;
						img_.renderToScreen(enemy[i][j].img_, &enemy[i][j].boundBox, NULL, enemy[i][j].screen_);
					}
			invaders.boundBox.x-=speed;
			break;

		case right:
			for(i=0; i<5; i++)
				for(j=0; j<10; j++)
					if(enemy[i][j].alive) {
						enemy[i][j].boundBox.x+=speed;
						img_.renderToScreen(enemy[i][j].img_, &enemy[i][j].boundBox, NULL, enemy[i][j].screen_);
					}
			invaders.boundBox.x+=speed;
			break;

	}

	fire();
	updateBullets();
}

void Enemy::move_down() 
{
	int i, j;

	for(i=0; i<5; i++)
		for(j=0; j<10; j++)
			if(enemy[i][j].alive)
				enemy[i][j].boundBox.y += 3;
}

void Enemy::fire() 
{
	if(rand()%SHOOTING_CHANCE == 10) {
		rand_row = rand() % 5;
		rand_col = rand() % 10;

		struct enemy_t thisEnem = enemy[rand_row][rand_col];

		if(thisEnem.alive && !thisEnem.bullet.fire) {
			bulPos.x = thisEnem.boundBox.x + (thisEnem.boundBox.w/2);
			bulPos.y = thisEnem.boundBox.y + 40;
			bulPos.w = 5;
			bulPos.h = 20;

			thisEnem.bullet.box = bulPos;
			img_.renderToScreen(bulImg, &thisEnem.bullet.box, NULL, thisEnem.screen_);
			thisEnem.bullet.fire = !thisEnem.bullet.fire;
			
			enemy[rand_row][rand_col] = thisEnem;
		}
	}
}

void Enemy::updateBullets()
{
	int i, j;

	for(i=0; i<5; i++)
		for(j=0; j<10; j++) {
			if(enemy[i][j].alive && enemy[i][j].bullet.fire) {
				if(enemy[i][j].bullet.box.y < 860) {
					enemy[i][j].bullet.box.y+=10;
					img_.renderToScreen(bulImg, &enemy[i][j].bullet.box, NULL, enemy[i][j].screen_);
				}
				else
					enemy[i][j].bullet.fire = !enemy[i][j].bullet.fire;
			}
		}
}

