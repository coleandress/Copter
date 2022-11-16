#pragma once
#include "Enemy.h"

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void updateEnemies(int& score);
	void spawnEnemies();
	Enemy* getEnemies();  //TODO need to discuss

	const static int ENEMY_MAX{ 64 };

private:
	Enemy mEnemies[ENEMY_MAX]{};
	int mEnemyCount{ 0 };
	float mEnemySpawnTimer{ 0.0 };
};

// Enemy Variables
//int enemyCount = 0;
//const int enemyMax = 64;
//float enemySpawnTimer = 0.0;
//Enemy enemy[enemyMax];
//Enemy::initEnemy(enemy, enemyCount, enemyMax);
