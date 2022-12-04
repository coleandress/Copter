#include "BulletSpawner.h"

BulletSpawner::BulletSpawner(ParticleManager& particleManager)
	: mParticleManager { particleManager }
{
}

void BulletSpawner::shoot(std::string tag, int type, float spawnX, float spawnY, float spawnW, float spawnH,
	float angle, float speed, float damage, SDL_Color color, int layer, float angleSpe, float angleDir, 
	float alpha, float alphaspeed, float deathTimer, float deathTimerSpeed, bool sizeDeath, float deathSpe, 
	bool decay, float decaySpeed, bool trail, float trailRate, SDL_Color trailColor, float trailMinSize, 
	float trailMaxSize, float timerBeforeMoving, bool goTowardsTarget, float targetX, float targetY,
	bool playSFXBeforeMoving, int bounces, float grav)
{
	mParticleManager.spawnParticleAngle(tag, type, spawnX, spawnY, spawnW, spawnH,
		angle, speed, damage, color, layer, angleSpe, angleDir,
		alpha, alphaspeed, deathTimer, deathTimerSpeed, sizeDeath, deathSpe,
		decay, decaySpeed, trail, trailRate, trailColor, trailMinSize,
		trailMaxSize, timerBeforeMoving, goTowardsTarget, targetX, targetY,
		playSFXBeforeMoving, bounces, grav);
}

void BulletSpawner::spawnExplosion(float x, float y, SDL_Color explosionColor)
{
	mParticleManager.SpawnExplosion(x, y, explosionColor);
}
