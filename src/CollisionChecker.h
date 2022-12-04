#pragma once

#include "ParticleManager.h"
#include "Player.h"
#include "EnemyManager.h"

class CollisionChecker
{
public:
	CollisionChecker(ParticleManager& part, Player& p1, EnemyManager& enemyManager, Sound& sound);

	void update();

private:
	ParticleManager& mParticleManager;
	Player& mP1;
	EnemyManager& mEnemyManager;
	Sound& mSound;
};

