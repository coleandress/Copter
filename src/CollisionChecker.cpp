#include "CollisionChecker.h"

CollisionChecker::CollisionChecker(ParticleManager& part, Player& p1, EnemyManager& enemyManager, Sound& sound)
	: mParticleManager{ part },
	  mP1{ p1 },
	  mEnemyManager{ enemyManager },
	  mSound{ sound }
{
}

void CollisionChecker::update()
{	
	// Particle collision with Player
	if (!mP1.flash)
	{
		for (int i = 0; i < mParticleManager.getParticles().size(); i++)
		{
			if (mParticleManager.getParticles()[i].mAlive)
			{
				if (mParticleManager.getParticles()[i].mType == 3)
				{
					// Player check
					if (Util::checkCollision(mParticleManager.getParticles()[i].mX, mParticleManager.getParticles()[i].mY,
						mParticleManager.getParticles()[i].mW, mParticleManager.getParticles()[i].mH, mP1.getX(),
						mP1.getY(), mP1.getWidth(), mP1.getHeight()))
					{

						// Hurt player
						mP1.Hurt(10);

						// Flash player
						mP1.flash = true;

						// remove particle
						mParticleManager.Remove(i);

						// spawn explosion
						mParticleManager.SpawnExplosion(
							mParticleManager.getParticles()[i].mX + mParticleManager.getParticles()[i].mW / 2,
							mParticleManager.getParticles()[i].mY + mParticleManager.getParticles()[i].mH / 2, { 200,
									200, 200 });

						// play sound effect
						mSound.playSound(PONG_SCORE);
					}
				}
			}
		}
	}


	// Particle collision with Enemies
	for (int i = 0; i < mParticleManager.getParticles().size(); i++)
	{
		if (mParticleManager.getParticles()[i].mAlive)
		{
			if (mParticleManager.getParticles()[i].mType == 4)
			{
				// Enemy check
				for (int j = 0; j < mEnemyManager.ENEMY_MAX; j++)
				{
					if (mEnemyManager.getEnemies()[j].alive)
					{
						// ON-HIT Collision Check
						if (Util::checkCollision(mParticleManager.getParticles()[i].mX,
							mParticleManager.getParticles()[i].mY, mParticleManager.getParticles()[i].mW,
							mParticleManager.getParticles()[i].mH, mEnemyManager.getEnemies()[j].x, mEnemyManager.getEnemies()[j].y,
							mEnemyManager.getEnemies()[j].w, mEnemyManager.getEnemies()[j].h))
						{
							// Flash enemy
							mEnemyManager.getEnemies()[j].flash = true;

							// Remove enemy
							mEnemyManager.getEnemies()[j].health -= 25;

							// remove particle
							mParticleManager.Remove(i);

							// spawn explosion
							mParticleManager.SpawnExplosion(
								mParticleManager.getParticles()[i].mX + mParticleManager.getParticles()[i].mW / 2,
								mParticleManager.getParticles()[i].mY + mParticleManager.getParticles()[i].mH / 2,
								{ 200, 200, 200 });

							// Play SFX
							mSound.playSound(PONG_SCORE);
						}
					}
				}
			}
		}
	}
}
