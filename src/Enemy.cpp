#include "Enemy.h"

/*void Enemy::initEnemy(Enemy enemy[], int& enemyCount, const int& enemyMax)
{
	enemyCount = 0;
	for (int i = 0; i < enemyMax; i++) {
		enemy[i].x = -100;
		enemy[i].y = -100;
		enemy[i].w = 32;
		enemy[i].h = 32;
		enemy[i].shootRate = 0;
		enemy[i].shootTimer = 0;
		enemy[i].type = 0;
		enemy[i].alpha = 255;
		enemy[i].flashTimer = 0;
		enemy[i].flash = false;
		enemy[i].alive = false;
	}
}*/

/*void Enemy::spawnEnemy(Enemy enemy[], float x, float y, float w, float h, int type, int& enemyCount, const int& enemyMax) {
	for (int i = 0; i < enemyMax; i++) {
		if (!enemy[i].alive) {
			enemy[i].x = x;
			enemy[i].y = y;
			enemy[i].w = w;
			enemy[i].h = h;
			enemy[i].type = type;
			if (type == 0) {
				enemy[i].health = 100;
				enemy[i].shootRate = 1.09f;
				enemy[i].shootTimer = 0;
			}
			else if (type == 1) {
				enemy[i].health = 175;
				enemy[i].shootRate = 0.89f;
				enemy[i].shootTimer = 0;
			}
			else if (type == 2) {
				enemy[i].health = 300;
				enemy[i].shootRate = 0.66f;
				enemy[i].shootTimer = 0;
			}
			enemy[i].alpha = 255;
			enemy[i].flashTimer = 0;
			enemy[i].flash = false;
			enemy[i].alive = true;
			enemyCount++;
			break;
		}
	}
}*/

/*void Enemy::updateEnemy(Enemy enemy[], int& enemyCount, const int& enemyMax, int& score) {
	for (int i = 0; i < enemyMax; i++) {
		if (enemy[i].alive) {

			// enemy constantly moving left
			if (enemy[i].type == 0) {
				enemy[i].x -= 2.77f;
			}
			else if (enemy[i].type == 1) {
				enemy[i].x -= 2.89f;
			}
			else if (enemy[i].type == 2) {
				enemy[i].x -= 3.34f;
			}

			// Flash enemy
			if (enemy[i].flash) {

				enemy[i].flashTimer += 3;

				if (enemy[i].flashTimer < 15) {
					enemy[i].alpha = 90;
				}

				else if (enemy[i].flashTimer >= 15 && enemy[i].flashTimer < 30) {
					enemy[i].alpha = 170;
				}

				else if (enemy[i].flashTimer >= 30 && enemy[i].flashTimer < 45) {
					enemy[i].alpha = 90;
				}

				else if (enemy[i].flashTimer >= 45 && enemy[i].flashTimer < 60) {
					enemy[i].alpha = 170;
				}
				else {
					enemy[i].alpha = 255;
					enemy[i].flashTimer = 0;
					enemy[i].flash = false;
				}
			}

			// enemy death by health
			if (enemy[i].health <= 0) {

				// Add player score
				if (enemy[i].type == 0) {
					score += 10;
				}
				else if (enemy[i].type == 1) {
					score += 20;
				}
				else if (enemy[i].type == 2) {
					score += 30;
				}

				// Remove enemy
				enemy[i].alive = false;
				enemyCount--;
			}

			// enemy death by border
			if (enemy[i].x < 0 - enemy[i].w) {
				enemy[i].alive = false;
				enemyCount--;
			}
		}
	}
}*/

void Enemy::renderEnemy(Enemy enemy[], float camx, float camy, SDL_Renderer& gRenderer, LTexture& gTanks, SDL_Rect rTanks[], const int& enemyMax) {
	for (int i = 0; i < enemyMax; i++) {
		if (enemy[i].alive) {
			if (enemy[i].flash) {
				gTanks.setAlpha(enemy[i].alpha);
				gTanks.render(&gRenderer, (int)enemy[i].x - (int)camx, (int)enemy[i].y - (int)camy, (int)enemy[i].w, (int)enemy[i].h, &rTanks[enemy[i].type]);
			}
			else {
				gTanks.setAlpha(enemy[i].alpha);
				gTanks.render(&gRenderer, (int)enemy[i].x - (int)camx, (int)enemy[i].y - (int)camy, (int)enemy[i].w, (int)enemy[i].h, &rTanks[enemy[i].type]);
			}
			/*SDL_Rect playerPower = {enemy[i].x-camx, enemy[i].y-camy,  enemy[i].w, enemy[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &playerPower);*/
		}
	}
}
