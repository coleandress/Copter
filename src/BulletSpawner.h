#pragma once
#include <string>
#include <SDL2/SDL_pixels.h>
#include "ParticleManager.h"

class BulletSpawner
{
public:
	BulletSpawner(ParticleManager& particleManager);

	void shoot(std::string tag, int type,
		float spawnX, float spawnY,
		float spawnW, float spawnH,
		float angle, float speed,
		float damage,
		SDL_Color color, int layer,
		float angleSpe, float angleDir,
		float alpha, float alphaspeed,
		float deathTimer, float deathTimerSpeed,
		bool sizeDeath, float deathSpe,
		bool decay, float decaySpeed,
		bool trail, float trailRate, SDL_Color trailColor,
		float trailMinSize, float trailMaxSize,
		float timerBeforeMoving,
		bool goTowardsTarget, float targetX, float targetY,
		bool playSFXBeforeMoving, int bounces,
		float grav);

private:
	ParticleManager& mParticleManager;
};
