/*
 * Particless.h
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */

#ifndef LOCAL_PARTICLE_H_
#define LOCAL_PARTICLE_H_

#include <SDL2/SDL_Mixer.h>

#include "Helper.h"
#include "LTexture.h"
#include "LWindow.h"
#include "Sound.h"
#include <vector>


struct Particle: Helper
{
public:	// variables
	float mX2{}, mY2{};							// particle center
	float mRadius{};								// particle radius
	float mX{}, mY{};
	float mW{}, mH{};
	double mAlpha{};
	double mAlphaspeed{};
	double mTime{};							// Current tick
	double mDeathTimer{};						// Max ticks before particle death
	double mDeathTimerSpeed{};					// Death tick rate
	float mAngle;
	float mVX{}, mVY{};
	float mSpeed{};
	float mAngleSpe{}, mAngleDir{};
	bool mOnScreen{};
	bool mAlive{};
	bool mCollide{};
	float mGrav{};
	std::string side;
	/* 0: player particle, does damage
	 * 1: enemies particle, does damage
	 * 2: stars, no damage
	 * 3: grenade, does damage
	 * 3: enemy particle II, does damage
	 */
	int mType;
	std::string mTag;		// who the owner of this particle is
	float mDamage;
	SDL_Color mColor;
	int mLayer;

	// Death by size
	bool mSizeDeath{};
	float mDeathSpe{};

	// Trail of particles
	bool mTrail{};
	float mTrailTimer{};
	float mTrailRate{};
	SDL_Color mTrailColor{};
	float mTrailMinSize{};
	float mTrailMaxSize{};
	float mTtimerBeforeMoving{};		// when it hits 0, it will move
	bool mGoTowardsTarget{};			// If true, after timerBeforeMoving is over, the Particles will move towards the target
	bool mPlaySFXBeforeMoving{};		// After waiting to move, should particle play a sound (i.e.: make a swoosh sound)
	int mBounces{};					// amount of times a particle may bounce before being destroyed
public:	// SAT theorem for collision during rotation's

	/* Frigate Corners
	 * A: Top Right
	 * B: Bottom Right
	 * C: Top Left
	 * D: Bottom Left
	 * */
	Point A{}, B{}, C{}, D{}; // These aren't doing anything

public:
	bool mDecay{};				// decay particle speed?
	float mDecaySpeed{};		// how much decay are we doing?
	int mTimeri{};				// timer that may be used for anything SPECIFICALLY for something (i.e.: this particle spawns out more particles such as a smoke)
};


class ParticleManager: Helper
{
public:
	ParticleManager(Message& msg, SDL_Renderer** renderer, int numberOfParticles);
	~ParticleManager();

public:	// other variables
	int mPTimer{};
	int mNumberOfAliveParticles{};
	LTexture mGParticles{};					// Particle Textures
	SDL_Rect mCParticles[2];				// [0: Blue], [1: Green], [2: Orange], [3: Red], [4: White], [5: Yellow] Particle

public:	// basic functions
	//void init(Particle particle[]);
	void Remove(int i);
	void RemoveAll();

public:	// functions
	void spawnParticleAngle(std::string tag, int type,
			float spawnX, float spawnY,
			float spawnW, float spawnH,
			float angle, float speed,
			float damage,
			SDL_Color color, int layer,
			float angleSpe, float angleDir,
			float alpha, float alphaspeed,
			float deathTimer, float deathTimerSpeed,
			bool sizeDeath = false, float deathSpe = 0.0,
			bool decay = false, float decaySpeed = 0.0,
			bool trail = false, float trailRate = 0.0, SDL_Color trailColor = {0, 255, 0},
			float trailMinSize = 0.0, float trailMaxSize = 0.0,
			float timerBeforeMoving = 0.0,
			bool goTowardsTarget = false, float targetXe = 0.0, float targetYe = 0.0,
			bool playSFXBeforeMoving = false, int bounces = 0,
			float grav = 0.0);

	// Update Bullet Particles
	void updateBulletParticles(int mapX, int mapY, int mapW, int mapH);

	// Render Particles
	void Render(int camX, int camY);

public:	// Function extensions

	// Spawn an explosion
	void SpawnExplosion(float x, float y, SDL_Color explosionColor);
	void SpawnFireExplosion(float x, float y, SDL_Color explosionColor);
	void SpawnTrail(float x, float y, SDL_Color explosionColor);
	void SpawnFireTrail(float x, float y, SDL_Color explosionColor);

public:	// Star
	void genStars(int startX, int startYy, int endW, int endH);
	void updateStarParticles(int mapX, int mapY, int mapW, int mapH);
	void renderStarParticle(int camX, int camY, float playerZ);

	// TODO: This is temporary until I can reverse the dependencies on Player and Enemy
	std::vector<Particle>& getParticles();


	void updateParticles(int /*mapX*/, int /*mapY*/, int /*mapW*/, int /*mapH*/,
		float camx, float camy, LWindow& mWindow, Sound& sound);

private:
	Message& mMsg;
	SDL_Renderer** mRenderer;

	std::vector<Particle> mParticles;
};

#endif /* LOCAL_PARTICLE_H_ */
