#pragma once
#include "Enemy.h"
#include "LWindow.h"
#include "Message.h"

class EnemyManager
{
public:
	EnemyManager(Message& msg, LWindow& window, SDL_Renderer** renderer);
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
};

