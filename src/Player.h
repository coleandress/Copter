/*
 * Player.h
 *
 *  Created on: May 8, 2017
 *      Author: Carl
 */

#include <SDL2/SDL_mixer.h>
#include "Particle.h"

// Player class
class Player {
public:

	// Create pong scoring
	Mix_Chunk *mPongScore;

	const float grav{ 0.09f };

	// Default constructor
	Player();

	void init(Mix_Chunk *sPongScore);

	void reset();

	// Set Player position
	void setPosition(int spawnX, int spawnY);

	// Set Player position
	void setX(int spawnX);

	// Set Player position
	void setY(int spawnY);

	// Add xPower
	void addXPower(float amount);

	// Add yPower
	void addYPower(float amount);

	// Hurt Player
	void Hurt(float damage);

	// Get Player x position
	float getX();

	// Get Player y position
	float getY();

	// Get Player left side
	float getLeftSide();

	// Get Player right side
	float getRightSide();

	// Get Player top side
	float getTopSide();

	// Get Player bottom side
	float getBottomSide();

	// Get Player width
	float getWidth();

	// Get Player width
	float getHeight();

	// Get Player center x position
	float getCenterX();

	// Get Player center y position
	float getCenterY();

	// Get Player y speed
	float getSpeedY();

	// Update Player
	void update(Particle &part, Particle particles[]);

	// Draw Player
	void render(SDL_Renderer *gRenderer);

	// Move Player left
	bool moveLeft;

	// Move Player right
	bool moveRight;

	// Move Player up
	bool moveUp;

	// Move Player down
	bool moveDown;

	// Player x speed
	float speedX;

	// Player y speed
	float speedY;

public: // abilities

	bool shoot;
	float shootTimer;
	bool holdPower;
	float xPower;
	float yPower;
	float angle;
	float turretAngle;
	float alpha;
	float flashTimer;
	bool flash;
	bool alive;

private:
	int x;
	int y;
	int w;
	int h;
	int health;
};
