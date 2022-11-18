#pragma once
#include "LWindow.h"
#include "Message.h"
#include "Particle.h"
#include "Util.h"

struct Enemy
{
	float x, y;
	float w, h;
	/*
	 * 0: Trooper
	 * 1: Medium Trooper
	 * 2: Heavy Trooper
	 */
	int type;
	int health;
	float shootRate;
	float shootTimer;
	Uint8 alpha;
	int flashTimer;
	bool flash;
	bool alive;
};

class EnemyManager
{
public:
	EnemyManager(Message& msg, LWindow& window, SDL_Renderer** renderer, Particle& part, Particle particles[], Sound& sound);
	~EnemyManager();
	void updateEnemies(int& score);
	void spawnEnemies();
	void enemiesShoot();
	void renderEnemies();
	Enemy* getEnemies();  //TODO need to discuss

	const static int ENEMY_MAX{ 64 };

private:
	Enemy mEnemies[ENEMY_MAX]{};
	int mEnemyCount{ 0 };
	float mEnemySpawnTimer{ 0.0 };
	LTexture mTankTexture;
	SDL_Rect mTankRects[6];

	Message& mMsg;
	LWindow& mWindow;
	SDL_Renderer** mRenderer;
	Particle mPart;
	Particle* mParticles;
	Sound& mSound;
};

