/*
 * Particle.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include "ParticleManager.h"


ParticleManager::ParticleManager(Message& msg, SDL_Renderer** renderer, int numberOfParticles)
	: mMsg{ msg },
	  mRenderer{ renderer }
{
	for (int i = 0; i < numberOfParticles; i++)
	{
		Particle p;
		p.mX = 0;
		p.mY = 0;
		p.mW = 4;
		p.mH = 4;
		p.mAlpha = 255;
		p.mTime = 0;
		p.mTimeri = 0;
		p.mAlphaspeed = 0;
		p.mDeathTimer = 100;
		p.mDeathTimerSpeed = 1;
		p.mSpeed = 0.00;
		p.mVX = 0.00;
		p.mVY = 0.00;
		p.mSpeed = 0.00;
		p.mAngle = 0.00;
		p.mAngleSpe = 0;
		p.mAngleDir = 0;
		p.mOnScreen = false;
		p.mCollide = false;
		p.mDecay = false;
		p.mDecaySpeed = 0.0;
		p.mAlive = false;
		p.mSizeDeath = false;
		p.mDeathSpe = 0;
		p.mTrail = false;
		p.mTrailRate = 0;
		p.side = "";
		p.mType = 0;
		p.mDamage = 0;
		p.mColor = { 255, 255, 255, 255 };
		p.mGoTowardsTarget = false;
		p.mPlaySFXBeforeMoving = false;

		mParticles.push_back(p);
	}

	mNumberOfAliveParticles = 0;

	setClips(mCParticles[0], 0, 0, 8, 8);
	setClips(mCParticles[1], 8, 0, 8, 8);
	Util::loadTextureFromFile(mMsg, mGParticles, mRenderer, "resource/gfx/particles.png");
	//mGParticles.loadFromFile(mRenderer, "resource/gfx/particles.png");
	mGParticles.setBlendMode(SDL_BLENDMODE_ADD);
}

ParticleManager::~ParticleManager()
{
	mGParticles.free();
}

void ParticleManager::Remove(int i) 
{
	mParticles[i].mAlive = false;
	mNumberOfAliveParticles--;;
}

void ParticleManager::RemoveAll() 
{
	mNumberOfAliveParticles = 0;
	for (int i = 0; i < mParticles.size(); i++) 
	{
		mParticles[i].mAlive = false;
	}
}

void ParticleManager::spawnParticleAngle(std::string tag, int type,
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
		float grav)
{
	for (int i = 0; i < mParticles.size(); i++)
	{
		if (!mParticles[i].mAlive)
		{
			mParticles[i].mTag 			= tag;
			mParticles[i].mType 			= type;
			mParticles[i].mX 				= spawnX;
			mParticles[i].mY 				= spawnY;
			mParticles[i].mW 				= spawnW;
			mParticles[i].mH 				= spawnH;
			mParticles[i].mX2 				= spawnX + spawnW/2;
			mParticles[i].mY2 				= spawnY + spawnH/2;
			mParticles[i].mTime 			= 0;
			mParticles[i].mTimeri 			= 0;
			mParticles[i].mAngle 			= angle;
			mParticles[i].mSpeed 			= speed;
			mParticles[i].mGrav 			= grav;
			// If true, after timerBeforeMoving is over, the Particles will move towards the target
			//particle[i].goTowardsTarget = goTowardsTarget;
			if (goTowardsTarget) {
				float shootAngle = atan2(targetY - mParticles[i].mY-mParticles[i].mH/2,targetX - mParticles[i].mX-mParticles[i].mW/2);
				shootAngle = shootAngle * (180 / 3.1416f);
				if (shootAngle < 0) { shootAngle = 360 - (-shootAngle); }
				mParticles[i].mVX 				= (cos( (3.14159265f / 180) * (shootAngle) ));
				mParticles[i].mVY 				= (sin( (3.14159265f / 180) * (shootAngle) ));
				// Also change Particle direction? decide this
				mParticles[i].mAngle = shootAngle;
			}else{
				mParticles[i].mVX 				= (cos( (3.14159265f / 180) * (angle) ));
				mParticles[i].mVY 				= (sin( (3.14159265f / 180) * (angle) ));
			}
			mParticles[i].mDamage 			= damage;
			//particle[i].x 				= spawnX + (rand() % 4 + 2 * (cos( (3.14159265/180)*(angle) )));
			//particle[i].y 				= spawnY + (rand() % 4 + 2 * (sin( (3.14159265/180)*(angle) )));
			//particle[i].x 				= spawnX + cos( (3.14159265/180)*(angle) );
			//particle[i].y 				= spawnY + sin( (3.14159265/180)*(angle) );

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////// Set Corners /////////////////////////////
			float particleCX = spawnX+spawnW/2;
			float particleCY = spawnY+spawnH/2;
			float radians   = (3.1415926536f / 180) * (mParticles[i].mAngle);
			float Cos 		= floor(cos(radians) * 100 + 0.05f) / 100;
			float Sin 		= floor(sin(radians) * 100+ 0.05f) / 100;
			// Top Right corner
		    float barrelW  = ((spawnW/2) * Cos ) - (-(spawnH/2) * Sin );
		    float barrelH  = ((spawnW/2) * Sin ) + (-(spawnH/2) * Cos );
		    float barrelX = particleCX + barrelW;
		    float barrelY = particleCY + barrelH;
			mParticles[i].A.x = barrelX;
			mParticles[i].A.y = barrelY;
			// Bottom Right corner
			barrelW  = ((spawnW/2) * Cos ) - ((spawnH/2) * Sin );
			barrelH  = ((spawnW/2) * Sin ) + ((spawnH/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			mParticles[i].B.x = barrelX;
			mParticles[i].B.y = barrelY;
			// Top Left corner
			barrelW  = (-(spawnW/2) * Cos ) - (-(spawnH/2) * Sin );
			barrelH  = (-(spawnW/2) * Sin ) + (-(spawnH/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			mParticles[i].C.x = barrelX;
			mParticles[i].C.y = barrelY;
			// Bottom Left corner
			barrelW  = (-(spawnW/2) * Cos ) - ((spawnH/2) * Sin );
			barrelH  = (-(spawnW/2) * Sin ) + ((spawnH/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			mParticles[i].D.x = barrelX;
			mParticles[i].D.y = barrelY;
			//////////////////////////////// Set Corners /////////////////////////////
			//////////////////////////////////////////////////////////////////////////

			mParticles[i].side 			= "";
			mParticles[i].mOnScreen 		= false;
			mParticles[i].mCollide 		= false;
			mParticles[i].mDecay 			= decay;
			mParticles[i].mDecaySpeed 		= decaySpeed;
			mParticles[i].mColor 			= color;
			mParticles[i].mLayer 			= layer;
			mParticles[i].mAlphaspeed 		= alphaspeed;
			mParticles[i].mAlpha 			= alpha;
			mParticles[i].mDeathTimer 		= deathTimer;
			mParticles[i].mDeathTimerSpeed = deathTimerSpeed;
			mParticles[i].mAngleSpe		= angleSpe;
			mParticles[i].mAngleDir		= angleDir;
			mParticles[i].mSizeDeath 		= sizeDeath;
			mParticles[i].mDeathSpe 		= deathSpe;
			mParticles[i].mTrail 			= trail;
			mParticles[i].mTrailTimer 		= 0;
			mParticles[i].mTrailRate 		= trailRate;
			mParticles[i].mTrailColor 		= trailColor;
			mParticles[i].mTrailMinSize 	= trailMinSize;
			mParticles[i].mTrailMaxSize 	= trailMaxSize;
			mParticles[i].mTtimerBeforeMoving 	= timerBeforeMoving;
			mParticles[i].mAlive 			= true;
			mParticles[i].mPlaySFXBeforeMoving 	= playSFXBeforeMoving;
			mParticles[i].mBounces 	= bounces;
			mNumberOfAliveParticles++;
			break;
		}
	}
}

// Spawn a Particle: moves based on a fixed velocity given and fixed speed
/*void Particle::fireParticle(Particle particle[], int type, int damage,
									   int spawnx, int spawny, int w, int h,
									   double speed, float vX, float vY,
									   int alphaspeed, int alpha,
									   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
									   SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].type 			= type;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha  			= alpha;
			particle[i].x  				= spawnx;
			particle[i].y  				= spawny;
			particle[i].w  				= w;
			particle[i].h  				= h;
			particle[i].speed  			= speed;
			particle[i].angle 			= angle;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].vX  			= vX;
			particle[i].vY  			= vY;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			//particle[i].angle 		= atan2 ( radianSin, radianCos) * 180 / PI;
			PARTICLES++;
			break;
		}
	}
}*/

// Generate Stars
void ParticleManager::genStars(int startX, int startY, int /*endW*/, int /*endH*/) {
	//this->ptimer += 1;
	//if (this->ptimer > 10){
	//	this->ptimer = 0;
	for (int i=0; i<65; i++){
		int randl = rand() % 5 + 3;
		//int rands = rand() % 3 + 4;
		int randW = rand() % 2304;
		int randH = rand() % 1296;
		//iint randW = rand() % 8000;
		//iint randH = rand() % 3300;
		Uint8 randc = rand() %  100 + 150;
		SDL_Color tempColor = { randc, randc, randc };
		spawnParticleAngle("none", 4,
				(float)(startX/randl + randW),
				(float)(startY/randl + randH),
						   (float)(10 - randl), (float)(10 - randl),
						   0.0f, 0.0f,
						   0,
						   tempColor, randl,
						   1, 1,
						   (float)(rand() % 100 + 50), (float)randDouble(0.5, 1.1),
						   100, 0,
						   true, 0.08f);
	}



		/*for (int i=0; i<65; i++){
			int randAlpha = rand() % 100+150;
			int cW = screnWidth + 200;
			int cH = screenHeight + 200;
			int randW = rand() % cW;
			int randH = rand() % cH;
			SDL_Color tempColor = { rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1};
			int randl = rand() % 5 + 5;
			int randw = rand() % 2 + 2;

			fireParticle(particle, 2, 0,
									0-100 + randW, 0-100 + randH,
									10/randl, 10/randl,
									0,
									0.0,0.0, 1, randAlpha,
									100, 1, 0.0, 0.0, 0.0,
									tempColor, randl);
		}*/
	//}
}

// Spawn a Particle: moves based on location of a target given
/*void Particle::fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
										int alphaspeed, int alpha,
										int deathTimer, int deathTimerSpeed,
										double speed,
										SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].alpha  			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].type 			= type;
			particle[i].x 				= spawnx;
			particle[i].y 				= spawny;
			particle[i].w 				= w;
			particle[i].h 				= h;
			particle[i].speed 			= speed;
			float bmx 					= targetx;
			float bmy 					= targety;
			float distance 				= sqrt((bmx - spawnx) * (bmx - spawnx - particle[i].w / 2) +
											   (bmy - spawny) * (bmy - spawny - particle[i].h / 2));
			particle[i].vX 				= particle[i].speed  *  (bmx - spawnx - particle[i].w / 2) / distance;
			particle[i].vY 				= particle[i].speed  *  (bmy - spawny - particle[i].h / 2) / distance;
			particle[i].angle 			= atan2(bmy - particle[i].y, bmx - particle[i].x);
			particle[i].angle 			= particle[i].angle * (180 / 3.1416);
			particle[i].angleSpe		= 0;
			particle[i].angleDir		= 0;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].collide 		= false;
			particle[i].onScreen 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			PARTICLES++;
			break;
		}
	}
}*/


void ParticleManager::updateBulletParticles(int /*mapX*/, int /*mapY*/, int /*mapW*/, int /*mapH*/) {
	for (int i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].mAlive)
		{

			// Enemy particle I
			if (mParticles[i].mType == 0) {
				// Update particles angle based on its X and Y velocities
				mParticles[i].mAngle = atan2 ( mParticles[i].mVY, mParticles[i].mVX) * 180 / 3.14159265f;
			}
			// Enemy particle II
			if (mParticles[i].mType == 1) {

			}
			// Enemy particle IV
			if (mParticles[i].mType == 3) {

			}
			// Enemy particle V
			if (mParticles[i].mType == 4) {

			}

			// Grenade particle
			/*if (particle[i].type == 3)
			{
				// get particle radius
				particle[i].radius = particle[i].w;

				// Particle movement
				particle[i].x += particle[i].vX * particle[i].speed;
				particle[i].y += particle[i].vY * particle[i].speed;

				if (particle[i].decay) {
					particle[i].speed = particle[i].speed - particle[i].speed * 0.09;
				}

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle map collision
				if (particle[i].x+particle[i].w < mapX) {
					particle[i].x = mapX+mapW-particle[i].w;
				}
				if (particle[i].x > mapX+mapW) {
					particle[i].x = mapX-particle[i].w;
				}
				if (particle[i].y+particle[i].h < mapY) {
					particle[i].y = mapY+mapH-particle[i].h;
				}
				if (particle[i].y > mapY+mapH) {
					particle[i].y = mapY-particle[i].h;
				}

				// Particle death
				if (particle[i].time > particle[i].deathTimer) {
					// play sound effect
					Mix_PlayChannel(3, sGrenadeExplode, 0);
					// spawn explosion particle effect
					for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
						int rands = rand() % 9 + 2;
						float newX = particle[i].x+particle[i].w/2;
						float newY = particle[i].y+particle[i].h/2;
						spawnParticleAngle(particle, 2,
											newX-rands/2,
											newY-rands/2,
										   rands, rands,
										   h, randDouble(2.1, 9.1),
										   0.0,
										   {255, 144, 10, 255}, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 2 + 5,
										   rand() % 50 + 90, 0,
										   true, 0.11,
										   rand() % 25 + 2, 0.15);
					}
					for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
						int rands = rand() % 9 + 2;
						float newX = particle[i].x+particle[i].w/2;
						float newY = particle[i].y+particle[i].h/2;
						spawnParticleAngle(particle, 2,
											newX-rands/2,
											newY-rands/2,
										   rands, rands,
										   h, randDouble(2.1, 9.1),
										   0.0,
										   {255, 255, 0, 255}, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 2 + 5,
										   rand() % 50 + 90, 0,
										   true, 0.11,
										   rand() % 25 + 2, 0.15);
					}
					// remove particle
					Remove(particle, i);
				}
			}*/
		}
	}
}

// Update Particles
void ParticleManager::updateStarParticles(int /*mapX*/, int /*mapY*/, int /*mapW*/, int /*mapH*/) {
	for (int i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].mAlive)
		{
			// Star particles
			if (mParticles[i].mType == 2)
			{

			}
		}
	}
}
void ParticleManager::Render(int camX, int camY) {
	for (int i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].mAlive) {
			mGParticles.setAlpha((Uint8)mParticles[i].mAlpha);
			mGParticles.setColor(mParticles[i].mColor.r, mParticles[i].mColor.g, mParticles[i].mColor.b);
			mGParticles.render(*mRenderer,
				(int)(mParticles[i].mX - camX/mParticles[i].mLayer),
				(int)(mParticles[i].mY - camY/mParticles[i].mLayer),
				(int)mParticles[i].mW,
				(int)mParticles[i].mH,
				&mCParticles[0],
				(double)mParticles[i].mAngle);
		}
	}
}

// Render stars
void ParticleManager::renderStarParticle(int camx, int camy, float /*playerZ*/) {
	for (int i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].mAlive) {

			// Render stars particle
			if (mParticles[i].mType == 2) {

				mGParticles.setAlpha((Uint8)mParticles[i].mAlpha);
				mGParticles.setColor(mParticles[i].mColor.r, mParticles[i].mColor.g, mParticles[i].mColor.b);
				mGParticles.render(*mRenderer,
					(int)mParticles[i].mX - camx,
					(int)mParticles[i].mY - camy,
					(int)mParticles[i].mW,
					(int)mParticles[i].mH,
					&mCParticles[0],
					mParticles[i].mAngle);

				// Top Right corner
			    /*SDL_Rect tempRect = { particle[i].A.x-1  - camx, particle[i].A.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Bottom Right corner
				tempRect = { particle[i].B.x-1  - camx, particle[i].B.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Top Left corner
				tempRect = { particle[i].C.x-1  - camx, particle[i].C.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Bottom Left corner
				tempRect = { particle[i].D.x-1  - camx, particle[i].D.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);*/

				/*SDL_Rect tempRect = {particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
						   particle[i].w,  particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, particle[i].color.r, particle[i].color.g, particle[i].color.b,1);
				SDL_RenderDrawRect(gRenderer, &tempRect);*/
			}
		}
	}
}

std::vector<Particle>& ParticleManager::getParticles()
{
	return mParticles;
}

//-------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------ Function Extensions-----------------------------------------------------//

void ParticleManager::SpawnExplosion(float x, float y, SDL_Color explosionColor) {
	for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
		int rands = rand() % 4 + 2;
		spawnParticleAngle(
			"none", 
			2,
			(float)x-rands / 2,
			(float)y-rands / 2,
			(float)rands,
			(float)rands,
			(float)j,
			(float)randDouble(0.9, 3.1),
			0.0,
			explosionColor,
			1,
			1,
			1,
			(float)(rand() % 100 + 150), 
			(float)(rand() % 2 + 2),
			(float)(rand() % 50 + 90),
			0,
			true, 
			0.11f,
			true,
			0.11f);
	}
}

void ParticleManager::SpawnFireExplosion(float x, float y, SDL_Color explosionColor) {
	for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
		int rands = rand() % 4 + 2;
		spawnParticleAngle("none", 4,
							x-rands/2-2,
							y-rands/2-2,
						   (float)rands+4, (float)rands+4,
						   (float)j, (float)randDouble(0.9, 3.1),
						   0.0,
						   {244,144,100}, 1,
						   1, 1,
						   (float)(rand() % 100 + 150),
						   (float)(rand() % 2 + 2),
						   (float)(rand() % 50 + 90), 0,
						   true, 0.11f,
						   true, 0.11f);
		spawnParticleAngle("none", 4,
							x-rands/2,
							y-rands/2,
						   (float)rands, (float)rands,
						   (float)j, (float)randDouble(0.9, 3.1),
						   0.0,
						   explosionColor, 1,
						   1, 1,
						   (float)(rand() % 100 + 150), (float)(rand() % 2 + 2),
						   (float)(rand() % 50 + 90), 0,
						   true, 0.11f,
						   true, 0.11f);
	}
}

void ParticleManager::SpawnTrail(float x, float y, SDL_Color explosionColor) {
	int rands = rand() % 4 + 2;
	spawnParticleAngle("none", 4,
						x-rands/2,
						y-rands/2,
					   (float)rands, (float)rands,
					   360, (float)randDouble(0.9, 3.1),
					   0.0,
					   explosionColor, 1,
					   1, 1,
					   (float)(rand() % 100 + 150), (float)(rand() % 2 + 2),
					   (float)(rand() % 50 + 90), 0,
					   true, 0.11f,
					   true, 0.11f);
}

void ParticleManager::SpawnFireTrail(float x, float y, SDL_Color explosionColor) {
	int rands = rand() % 4 + 2;
	spawnParticleAngle("none", 4,
						x-rands/2-2,
						y-rands/2-2,
					   (float)rands+4, (float)rands+4,
					   360, (float)randDouble(0.9, 3.1),
					   0.0,
					   {244,144,100}, 1,
					   1, 1,
					   (float)(rand() % 100 + 150), (float)(rand() % 2 + 2),
					   (float)(rand() % 50 + 90), 0,
					   true, 0.11f,
					   true, 0.11f);
	spawnParticleAngle("none", 4,
						x-rands/2,
						y-rands/2,
					   (float)rands, (float)rands,
					   360, (float)randDouble(0.9, 3.1),
					   0.0,
					   explosionColor, 1,
					   1, 1,
					   (float)(rand() % 100 + 150), (float)(rand() % 2 + 2),
					   (float)(rand() % 50 + 90), 0,
					   true, 0.11f,
					   true, 0.11f);
}

void ParticleManager::updateParticles(int /*mapX*/, int /*mapY*/, int /*mapW*/, int /*mapH*/,
	float camx, float camy, LWindow& mWindow, Sound& sound) {
	for (int i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].mAlive)
		{
			// If there is a timer before moving, do timer first before handling Particle
			if (mParticles[i].mTtimerBeforeMoving != 0) {
				mParticles[i].mTtimerBeforeMoving -= 1;
			}
			else {
				// Play one time sound effect
				if (mParticles[i].mPlaySFXBeforeMoving) {
					mParticles[i].mPlaySFXBeforeMoving = false;
					// play SFX
					//Mix_PlayChannel(-1, sFireBall, 0);
				}
			}

			///////////////////////////////////////////////////////////

			// Particle Y gravity
			mParticles[i].mVY += mParticles[i].mGrav;

			// Particle movement
			mParticles[i].mX += mParticles[i].mVX * mParticles[i].mSpeed;
			mParticles[i].mY += mParticles[i].mVY * mParticles[i].mSpeed;

			// Speed decay of grenade
			if (mParticles[i].mDecay) {
				mParticles[i].mSpeed = mParticles[i].mSpeed - mParticles[i].mSpeed * mParticles[i].mDecaySpeed;
			}
			// Particle death, upon size
			if (mParticles[i].mSizeDeath) {
				mParticles[i].mW -= mParticles[i].mDeathSpe;
				mParticles[i].mH -= mParticles[i].mDeathSpe;

			}
			// Particle spin
			mParticles[i].mAngle += mParticles[i].mAngleSpe * mParticles[i].mAngleDir;
			// Particle death, Time
			mParticles[i].mTime += mParticles[i].mDeathTimerSpeed;
			// Particle death, transparency
			mParticles[i].mAlpha -= mParticles[i].mAlphaspeed;
			//////////////////////////////////////////////////////////

			// particle center
			mParticles[i].mX2 = mParticles[i].mX + mParticles[i].mW / 2;
			mParticles[i].mY2 = mParticles[i].mY + mParticles[i].mH / 2;

			// get particle radius
			mParticles[i].mRadius = mParticles[i].mW;

			//If the tile is in the screen
			if (mParticles[i].mX + mParticles[i].mW > camx && mParticles[i].mX < camx + mWindow.getWidth()
				&& mParticles[i].mY + mParticles[i].mH > camy && mParticles[i].mY < camy + mWindow.getHeight()) {
				mParticles[i].mOnScreen = true;
			}
			else {
				mParticles[i].mOnScreen = false;
			}

			///////////////////////////////////////////////////////////////////////////////
			/////////////////////////// Set Corners of a Particle /////////////////////////
			//---------------------------------------------------------------------------//
			float particleCX = mParticles[i].mX + mParticles[i].mW / 2;
			float particleCY = mParticles[i].mY + mParticles[i].mH / 2;
			float radians = (3.1415926536f / 180) * (mParticles[i].mAngle);
			float Cos = floor(cos(radians) * 100 + 0.05f) / 100;
			float Sin = floor(sin(radians) * 100 + 0.05f) / 100;

			// Top Right corner
			float barrelW = ((mParticles[i].mW / 2) * Cos) - (-(mParticles[i].mH / 2) * Sin);
			float barrelH = ((mParticles[i].mW / 2) * Sin) + (-(mParticles[i].mH / 2) * Cos);
			float barrelX = particleCX + barrelW;
			float barrelY = particleCY + barrelH;
			mParticles[i].A.x = barrelX;
			mParticles[i].A.y = barrelY;

			// Bottom Right corner
			barrelW = ((mParticles[i].mW / 2) * Cos) - ((mParticles[i].mH / 2) * Sin);
			barrelH = ((mParticles[i].mW / 2) * Sin) + ((mParticles[i].mH / 2) * Cos);
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			mParticles[i].B.x = barrelX;
			mParticles[i].B.y = barrelY;

			// Top Left corner
			barrelW = (-(mParticles[i].mW / 2) * Cos) - (-(mParticles[i].mH / 2) * Sin);
			barrelH = (-(mParticles[i].mW / 2) * Sin) + (-(mParticles[i].mH / 2) * Cos);
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			mParticles[i].C.x = barrelX;
			mParticles[i].C.y = barrelY;

			// Bottom Left corner
			barrelW = (-(mParticles[i].mW / 2) * Cos) - ((mParticles[i].mH / 2) * Sin);
			barrelH = (-(mParticles[i].mW / 2) * Sin) + ((mParticles[i].mH / 2) * Cos);
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			mParticles[i].D.x = barrelX;
			mParticles[i].D.y = barrelY;

			// Handle different types of deaths
			if (mParticles[i].mTime > mParticles[i].mDeathTimer) {
				// remove particle
				Remove(i);
				// spawn explosion
				SpawnExplosion(mParticles[i].mX + mParticles[i].mW / 2, mParticles[i].mY + mParticles[i].mH / 2, { 200,200,200 });
			}
			else if (mParticles[i].mAlpha < 0) {
				Remove(i);
			}
			else if (mParticles[i].mW <= 0 || mParticles[i].mH <= 0) {
				Remove(i);
			}
			// Ground bounce
			//else if (mParticles[i].y + mParticles[i].h > ground - 32 && mParticles[i].alphaspeed == 0) { // I changed this but need to document it's the ground CA 2022-11-10
			else if (mParticles[i].mY + mParticles[i].mH > mWindow.getHeight() - 32 && mParticles[i].mAlphaspeed == 0) {
				mParticles[i].mOnScreen = true;
				// remove particle
				Remove(i);
				// spawn explosion
				SpawnExplosion(mParticles[i].mX + mParticles[i].mW / 2, mParticles[i].mY + mParticles[i].mH / 2, { 200,200,100 });
				// play sound effect
				//Mix_PlayChannel(-1, sPongScore, 0);
				sound.playSound(PONG_SCORE);
			}
		}
	}
}

//------------------------------------------------------ Function Extensions-----------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
