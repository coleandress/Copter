#include "EnemyManager.h"

EnemyManager::EnemyManager(Message& msg, LWindow& window, SDL_Renderer** renderer, ParticleManager& part, Particle particles[], Sound& sound)
	: mMsg{ msg },
	  mWindow{ window },
	  mRenderer{ renderer },
	  mPart{ part },
	  mParticles { particles },
	  mSound { sound }
{
	Util::loadTextureFromFile(mMsg, mTankTexture, mRenderer, "resource/gfx/tanks.png");
	
	// Texture clips
	
	mTankRects[0] = { 0,0,32,32 };
	mTankRects[1] = { 32,0,32,32 };
	mTankRects[2] = { 64,0,32,32 };
	mTankRects[3] = { 0,32,32,32 };
	mTankRects[4] = { 32,32,32,32 };
	mTankRects[5] = { 64,32,32,32 };


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
	mTankTexture.free();
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
		spawnEnemies();
	}

	enemiesShoot();
}

void EnemyManager::spawnEnemies()
{
	float x = 1280.0f + rand() % 100; // TODO: Study and adjust this, need to replace 1280 with window width
	float y = 720.0f - 64.0f - 32.0f + 5.0f;  // TODO: pass in window and replace 720 with getHeight.  This is the ground I believe.
	float w = 64.0f;
	float h = 64.0f;
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

void EnemyManager::enemiesShoot()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (mEnemies[i].alive)
		{
			// Handle shoot rate
			mEnemies[i].shootTimer += mEnemies[i].shootRate;
			if (mEnemies[i].shootTimer > 60)
			{
				mEnemies[i].shootTimer = 0;
				// shoot particle
				float newX = mEnemies[i].x + mEnemies[i].w / 2 - 12;
				float newY = mEnemies[i].y + mEnemies[i].h / 2 - 9;
				mPart.spawnParticleAngle(mParticles, "slow", 3, newX,
					newY, 11, 11, Util::randFloat(200, 225), 9, 0.0f, {
							200, 200, 200 }, 1, 1, 1, 255, 0, 60, 0,
							false, 0.11f, false, 0.11f, false, 0.0f, Util::WHITE,
							0.0f, 0.0f, 0.0f, false, 0.0f, 0.0f, false, 0,
							0.004f);
				// play sfx
				mSound.playSound(SHOOT);
			}
		}
	}

}

void EnemyManager::renderEnemies()
{
	float camx = 0;
	float camy = 0;

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (mEnemies[i].alive) {
			if (mEnemies[i].flash) {
				mTankTexture.setAlpha(mEnemies[i].alpha);
				mTankTexture.render(*mRenderer, (int)mEnemies[i].x - (int)camx, (int)mEnemies[i].y - (int)camy, (int)mEnemies[i].w, (int)mEnemies[i].h, &mTankRects[mEnemies[i].type]);
			}
			else {
				mTankTexture.setAlpha(mEnemies[i].alpha);
				mTankTexture.render(*mRenderer, (int)mEnemies[i].x - (int)camx, (int)mEnemies[i].y - (int)camy, (int)mEnemies[i].w, (int)mEnemies[i].h, &mTankRects[mEnemies[i].type]);
			}
			/*SDL_Rect playerPower = {enemy[i].x-camx, enemy[i].y-camy,  enemy[i].w, enemy[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &playerPower);*/
		}
	}
}

Enemy* EnemyManager::getEnemies() //TODO need to discuss
{
	return mEnemies;
}
