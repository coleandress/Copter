/*
 * Player.h
 *
 *  Created on: May 8, 2017
 *      Author: Carl
 */

#include <SDL2/SDL_mixer.h>
#include "ParticleManager.h"
#include "Sound.h"
#include "Message.h"
#include "Util.h"

// Player class
class Player {
public:

	// Create pong scoring
	//Mix_Chunk *mPongScore;

	const float grav{ 0.09f };

	// Default constructor
	Player(Message& message, SDL_Renderer** renderer, ParticleManager &part, Sound& sound);
	~Player();

	void reset();

	// Set Player position
	void setPosition(float spawnX, float spawnY);

	// Set Player position
	void setX(float spawnX);

	// Set Player position
	void setY(float spawnY);

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

	LTexture& getTexture();

	SDL_Rect* getRects();

	int& getPlayerFrame();

	// Update Player
	void update(int& mx, int& my);

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
	float speedX{};

	// Player y speed
	float speedY{};

public: // abilities

	bool shoot;
	float shootTimer;
	bool holdPower;
	float xPower;
	float yPower;
	float angle;
	float turretAngle;
	Uint8 alpha;
	float flashTimer;
	bool flash;
	bool alive;

private:
	float x;
	float y;
	float w;
	float h;
	float health;
	Message& mMsg;
	SDL_Renderer** mRenderer;
	ParticleManager& mPart;
	Sound& mSound;
	LTexture mCopterTexture;
	SDL_Rect mCopterRects[5];
	const float mPlayerFrameRate{ 15 };
	float mPlayerFrameTimer{ 0 };
	int mPlayerFrame{ 0 };
};
