/*
 * Player.cpp
 *
 *  Created on: May 8, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include "Player.h"

// Default constructor
Player::Player(Message& message, SDL_Renderer** renderer, ParticleManager &part, Sound& sound)
	: mMsg{ message },
	  mRenderer{ renderer },
	  mPart{ part },
	  mSound{ sound }
{
	setPosition(64, 360/2-112/2);
	w = 128;
	h = 64;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	holdPower = false;
	shoot = false;
	shootTimer = -1;
	speedX = 0.0;
	speedY = 0.0;
	xPower = 0.0;
	yPower = 0.0;
	health = 100.0;
	angle = 0.0;
	turretAngle = 0.0;
	//mPongScore = NULL;
	alpha = 255;
	flashTimer = 0;
	flash = false;
	alive = true;	
	
	std::string fileName{ "resource/gfx/player-copter.png" };

	Util::loadTextureFromFile(mMsg, mCopterTexture, mRenderer, fileName);

	mCopterRects[0] = { 0,0,128,64 };
	mCopterRects[1] = { 128,0,128,64 };
	mCopterRects[2] = { 256,0,128,64 };
	mCopterRects[3] = { 384,0,128,64 };
	mCopterRects[4] = { 512,0,128,64 };
}

Player::~Player()
{
	mCopterTexture.free();
}

void Player::reset() {
	setPosition(64, 360/2-112/2);
	w = 128;
	h = 64;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	holdPower = false;
	shoot = false;
	shootTimer = -1;
	speedX = 0.0;
	speedY = 0.0;
	xPower = 0.0;
	yPower = 0.0;
	health = 100.0;
	angle = 0.0;
	turretAngle = 0.0;
	alpha = 255;
	flashTimer = 0;
	flash = false;
	alive = true;
}

// Set Player position
void Player::setPosition(float spawnX, float spawnY) {
	x = spawnX;
	y = spawnY;
}

// Set Player x position
void Player::setX(float spawnX) {
	x = spawnX;
}

// Set Player y position
void Player::setY(float spawnY) {
	y = spawnY;
}

// Add XPower
void Player::addXPower(float amount) {
	if (xPower < 5) {
		xPower += amount;
	}
}

// Add YPower
void Player::addYPower(float amount) {
	yPower = amount;
}

// Hurt Player
void Player::Hurt(float damage) {
	health -= damage;
}

// Return Player x position
float Player::getX() {
	return x;
}

// Return Player y position
float Player::getY() {
	return y;
}

// Get Player left side
float Player::getLeftSide() {
	return x;
}

// Get Player right side
float Player::getRightSide() {
	return x+w;
}

// Get Player top side
float Player::getTopSide() {
	return y;
}

// Get Player bottom side
float Player::getBottomSide() {
	return y+h;
}

// Return Player width
float Player::getWidth() {
	return w;
}

// Return Player height
float Player::getHeight() {
	return h;
}

// Return Player width
float Player::getCenterX() {
	return x+w/2;
}

// Return Player height
float Player::getCenterY() {
	return y+h/2;
}

// Return Player y speed
float Player::getSpeedY() {
	return speedY;
}

LTexture& Player::getTexture()
{
	return mCopterTexture;
}

SDL_Rect* Player::getRects()
{
	return mCopterRects;
}

int& Player::getPlayerFrame()
{
	return mPlayerFrame;
}

void Player::update(int& mx, int& my) 
{
	const int screenWidth = 1280; // TODO: Get this from window CA 2022-11-16
	const int screenHeight = 720; // TODO: Get this from window CA 2022-11-16

	if (moveLeft) {
		if (angle > -20) {
			angle -= 1;
		}
	}
	else if (moveRight) {
		if (angle < 20) {
			angle += 1;
		}
	}

	// Move player left
	if (moveLeft && speedX > -6) {
		speedX -= 0.26f;
	}
	// Move player right
	else if (moveRight && speedX < 6) {
		speedX += 0.37f;
	}

	// No movement, start decay
	if (!moveLeft && !moveRight) {
		speedX = speedX - speedX * 0.05f;

		// Level coptor after tilting
		if (angle != 0) {
			angle = angle - angle * 0.05f;
		}
	}

	// Add movement
	x += speedX;

	// Move player up
	if (moveUp && speedY > -6) {
		speedY -= 0.25;
	}
	// Move player down
	else if (moveDown && speedY < 6) {
		speedY += 0.25;
	}

	// No movement, start decay
	if (!moveUp && !moveDown) {
	//	speedY = speedY - speedY * 0.09;
	}

	// Gravity
	speedY += grav;

	// Add movement
	y += speedY;

	// Player left of screen bound
	if (x < 0) {
		x = 0;
		speedX = 0.0;
	}
	// Player right of screen bound
	if (x + w > screenWidth) {
		x = screenWidth - w;
		speedX = 0.0;
	}

	// Player top of screen bound
	if (y < 0) {
		y = 0;
		speedY = 0.0;
	}
	// Player bottom of screen bound
	if (y + h > screenHeight - 28) {
		y = screenHeight - 28 - h;

		// If not flashing damaged
		if (!flash) {
			// Hurt player
			speedY = -5.0;
			flash = true;

			// Play SFX
			//Mix_PlayChannel(-1, mPongScore, 0);
			mSound.playSound(PONG_SCORE);

			// Spawn explosion
			mPart.SpawnExplosion(
					(float)(this->x + this->w) / 2,
					(float)(this->y + this->h) / 2, { 200,
							200, 200 });

			// Health
			health -= 25;
		}
	}

	// Shoot power
	if (holdPower) {
		addXPower(0.1f);
	}

	if (health <= 0) {
		health = 0;
		alive = false;
	}

	// Flash enemy
	if (flash) {

		flashTimer += 2;

		if (flashTimer < 15) {
			alpha = 90;
		}

		else if (flashTimer >= 15 && flashTimer < 30) {
			alpha = 170;
		}

		else if (flashTimer >= 30 && flashTimer < 45) {
			alpha = 90;
		}

		else if (flashTimer >= 45 && flashTimer < 60) {
			alpha = 170;
		} else {
			alpha = 255;
			flashTimer = 0;
			flash = false;
		}
	}

	// -------------------------------------------------------
	// from main() after p1.update() call
	// -------------------------------------------------------

	// Do Player frames
	mPlayerFrameTimer += mPlayerFrameRate;
	if (mPlayerFrameTimer > 60)
	{
		mPlayerFrameTimer = 0;
		mPlayerFrame++;
		if (mPlayerFrame > 4)
		{
			mPlayerFrame = 0;
		}
	}

	// Update Player's, manually
	if (shoot)
	{
		shoot = false;
		float centerX = getCenterX();
		float centerY = getCenterY();

		///////////////////////////////////////////////////////////////////////
		//-------------------------------------------------------------------//
		//---------------------- Handle Basic Shooting ----------------------//
		/***** Set Turret Position *****/
		float turret1w = 128;
		float turret1h = 64;
		float frigateAngle = angle;
		float radians = (3.1415926536f / 180) * (angle);
		//float Cos 		= floor(cos(radians)*100+0.05)/100;
		//float Sin 		= floor(sin(radians)*100+0.05)/100;
		float Cos = cos(radians);
		float Sin = sin(radians);
		// 1st turret
		float barrelW = (0 * Cos) - (0 * Sin);
		float barrelH = (0 * Sin) + (0 * Cos);
		float barrelX = centerX + barrelW;
		float barrelY = centerY + barrelH;
		float turret1x = barrelX - turret1w / 2;
		float turret1y = barrelY - turret1h / 2;
		float particleW = 19;
		float particleH = 4;
		/***** Set Turret Position *****/

		/***** Get turrets nose angle (get the exact position even when the player rotates) *****/
		frigateAngle = angle;
		radians = (3.1415926536f / 180) * (angle);
		barrelW = (60 * cos(radians)) - (32 * sin(radians));// add this to center of zombie (this will give us the guns barrel position)
		barrelH = (60 * sin(radians)) + (32 * cos(radians));
		barrelX = turret1x + turret1w / 2 - particleW / 2 + barrelW;
		barrelY = turret1y + turret1h / 2 - particleH / 2 + barrelH;
		/***** Get turrets nose angle *****/

		float newAngle = atan2(my - barrelY, mx - barrelX);
		newAngle = newAngle * 180 / (float)M_PI;

		mPart.spawnParticleAngle("slow", 4, barrelX, barrelY,
			particleW, particleH, newAngle, 11, 0.0f,
			{ 200, 200, 100 }, 1, 0, 0, 255, 0, 60, 0, false, 0.11f,
			false, 0.11f, false, 0.0f, Util::WHITE, 0.0f, 0.0f, 0.0f, false,
			0.0f, 0.0f, false, 0, 0.0f);
		// play sfx
		mSound.playSound(SHOOT);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////// Manual Updates ////////////////////////////////////////////

	// Particle collision with Player
	if (!flash)
	{
		for (int i = 0; i < mPart.getParticles().size(); i++)
		{
			if (mPart.getParticles()[i].mAlive)
			{
				if (mPart.getParticles()[i].mType == 3)
				{
					// Player check
					if (Util::checkCollision(mPart.getParticles()[i].mX, mPart.getParticles()[i].mY,
						mPart.getParticles()[i].mW, mPart.getParticles()[i].mH, getX(),
						getY(), getWidth(), getHeight()))
					{

						// Hurt player
						Hurt(10);

						// Flash player
						flash = true;

						// remove particle
						mPart.Remove(i);

						// spawn explosion
						mPart.SpawnExplosion(
							mPart.getParticles()[i].mX + mPart.getParticles()[i].mW / 2,
							mPart.getParticles()[i].mY + mPart.getParticles()[i].mH / 2, { 200,
									200, 200 });

						// play sound effect
						mSound.playSound(PONG_SCORE);
					}
				}
			}
		}
	}
}

// Draw Player
void Player::render(SDL_Renderer *gRenderer) {

	// Draw rectangle
	//SDL_Rect tempRect = {x, y, w, h};
	//SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	//SDL_RenderDrawRect(gRenderer, &tempRect);

	// Create Health bar
	SDL_Rect tempRect = { (int)x, (int)y-20, (int)w, 15 };
	SDL_SetRenderDrawColor(gRenderer, 0, 50, 0, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	tempRect = { (int)x, (int)y-20, (int)(w*health)/100, 15 };
	SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);

	tempRect = { (int)x, (int)y-20, (int)w, 15 };
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);
}
