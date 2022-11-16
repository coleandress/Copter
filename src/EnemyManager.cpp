#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	for (int i = 0; i < ENEMY_MAX; i++) {
		mEnemies[i].x = -100;
		mEnemies[i].y = -100;
		mEnemies[i].w = 32;
		mEnemies[i].h = 32;
		mEnemies[i].shootRate = 0;
		mEnemies[i].shootTimer = 0;
		mEnemies[i].type = 0;
		mEnemies[i].alpha = 255;
		mEnemies[i].flashTimer = 0;
		mEnemies[i].flash = false;
		mEnemies[i].alive = false;
	}
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::updateEnemies(int& score)
{
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (mEnemies[i].alive) {

			// enemy constantly moving left
			if (mEnemies[i].type == 0) {
				mEnemies[i].x -= 2.77f;
			}
			else if (mEnemies[i].type == 1) {
				mEnemies[i].x -= 2.89f;
			}
			else if (mEnemies[i].type == 2) {
				mEnemies[i].x -= 3.34f;
			}

			// Flash enemy
			if (mEnemies[i].flash) {

				mEnemies[i].flashTimer += 3;

				if (mEnemies[i].flashTimer < 15) {
					mEnemies[i].alpha = 90;
				}

				else if (mEnemies[i].flashTimer >= 15 && mEnemies[i].flashTimer < 30) {
					mEnemies[i].alpha = 170;
				}

				else if (mEnemies[i].flashTimer >= 30 && mEnemies[i].flashTimer < 45) {
					mEnemies[i].alpha = 90;
				}

				else if (mEnemies[i].flashTimer >= 45 && mEnemies[i].flashTimer < 60) {
					mEnemies[i].alpha = 170;
				}
				else {
					mEnemies[i].alpha = 255;
					mEnemies[i].flashTimer = 0;
					mEnemies[i].flash = false;
				}
			}

			// enemy death by health
			if (mEnemies[i].health <= 0) {

				// Add player score
				if (mEnemies[i].type == 0) {
					score += 10;
				}
				else if (mEnemies[i].type == 1) {
					score += 20;
				}
				else if (mEnemies[i].type == 2) {
					score += 30;
				}

				// Remove enemy
				mEnemies[i].alive = false;
				mEnemyCount--;
			}

			// enemy death by border
			if (mEnemies[i].x < 0 - mEnemies[i].w) {
				mEnemies[i].alive = false;
				mEnemyCount--;
			}
		}
	}

	mEnemySpawnTimer += 0.22f;
	if (mEnemySpawnTimer > 60)
	{
		mEnemySpawnTimer = 0;

		// Spawn a random enemy a few pixels to the right of the scree
		//spawnEnemy(enemy, 1280 + rand() % 100, ground - 64 - 32 + 5, 64, // I changed this need to indicate the height is the ground CA 2022-11-10
		spawnEnemies();

		/*Enemy::spawnEnemy(
			enemy,
			1280.0f + rand() % 100,
			window.getHeight() - 64.0f - 32.0f + 5.0f,
			64,
			64,
			rand() % 3,
			enemyCount,
			enemyMax
		);*/
	}
}

//void Enemy::spawnEnemy(Enemy enemy[], float x, float y, float w, float h, int type, int& enemyCount, const int& enemyMax) {
void EnemyManager::spawnEnemies()
{
	float x = 1280.0f + rand() % 100; // TODO: Study and adjust this, need to replace 1280 with window width
	float y = 720.0f - 64.0f - 32.0f + 5.0f;  // TODO: pass in window and replace 720 with getHeight.  This is the ground I believe.
	int w = 64;
	int h = 64;
	int type = rand() % 3;

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (!mEnemies[i].alive) {
			mEnemies[i].x = x;
			mEnemies[i].y = y;
			mEnemies[i].w = w;
			mEnemies[i].h = h;
			mEnemies[i].type = type;
			if (type == 0) {
				mEnemies[i].health = 100;
				mEnemies[i].shootRate = 1.09f;
				mEnemies[i].shootTimer = 0;
			}
			else if (type == 1) {
				mEnemies[i].health = 175;
				mEnemies[i].shootRate = 0.89f;
				mEnemies[i].shootTimer = 0;
			}
			else if (type == 2) {
				mEnemies[i].health = 300;
				mEnemies[i].shootRate = 0.66f;
				mEnemies[i].shootTimer = 0;
			}
			mEnemies[i].alpha = 255;
			mEnemies[i].flashTimer = 0;
			mEnemies[i].flash = false;
			mEnemies[i].alive = true;
			mEnemyCount++;
			break;
		}
	}
}

Enemy* EnemyManager::getEnemies() //TODO need to discuss
{
	return mEnemies;
}
