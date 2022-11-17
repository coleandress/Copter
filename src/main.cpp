#define DEBUG 1

#include <sstream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LWindow.h"
#include "Particle.h"
#include "Player.h"
#include "Enemy.h"
#include "Sound.h"
#include "Font.h"

#include "Message.h"
#include "Background.h"
#include "EnemyManager.h"

void setup(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer, int& previousHighScore);
void initSDL(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer);
void loadMedia(Message& msg, SDL_Renderer** gRenderer);
void LoadHighScore(int& previousHighScore);
void handleInput(Player& p1, int& gameScene, int& previousHighScore, int& score, bool& paused, SDL_Event& events, Sound& sound);
void ContinueGame(Player& p1, int& gameScene, int& previousHighScore, int& score, Sound& sound);
void SaveHighScore(const int& score);
bool checkCollision(float x, float y, float w, float h, float x2, float y2, float w2, float h2);
float randFloat(float fMin, float fMax);
int count_digit(int number);
void freeSDL(LWindow& mWindow);

// Player
LTexture gCopter;
SDL_Rect rCopter[5];

int main(int, char**)
{
	// Replace this with enum
	/*
		 * 0: Before game scene
		 * 1: Game playing scene
		 * 2: Game lost scene
		 * 3: Game winner scene?
		*/
		// TODO: extract score into class

	LWindow window{};
	SDL_Renderer* renderer;
	Message msg{};
	int previousHighScore{ -1 };
	setup(msg, window, &renderer, previousHighScore);
	Background background(msg, window, &renderer);
	Particle part;
	Particle particles[1000];
	part.init(particles);
	part.load(&renderer); 
	Sound sound;
	EnemyManager enemyManager{ msg, window, &renderer, part, particles, sound };
	int gameScene{ 0 };
	//int highscore{ -1 };
	int score{ 0 };
	Font font;

	msg.log("In main(), calling: setup ...");

	bool paused = false;

	// Create pointer for events
	SDL_Event events;


	// Game loop
	bool quit = false;


	// Create Player 1
	Player p1;
	p1.init();

	// Timer
	Timer fps;
	bool cap = true;
	int frame = 0;
	const int FRAMES_PER_SECOND = 60;

	// Set Player 1 position
	p1.setPosition(64, 360 / 2 - 112 / 2);

	// game booleans
	float fireTimer = 0;
	float fireRate = 15;

	// Player Variables
	const float playerFrameRate = 15;
	float playerFrameTimer = 0;
	int playerFrame = 0;

	// Call before loop
	fireTimer = 0;
	fireRate = 15;
	playerFrame = 0;

	sound.playMusic();

	msg.log("Entering main game loop ...");

	// Game loop
	while (!quit)
	{
		// Start FPS cap
		fps.start();

		int mx, my;
		SDL_GetMouseState(&mx, &my);

		// Handle Events
		while (SDL_PollEvent(&events)) {

			// Handle window quit events
			if (events.type == SDL_QUIT) {
				quit = true;
			}

			// Handle window input events
			else
			{
				handleInput(p1, gameScene, previousHighScore, score, paused, events, sound);
			}
		}

		// If game paused
		if (!paused == 1)
		{
			if (gameScene == 1)
			{
				// particle test for particles
				for (int i = 0; i < part.mMax; i++)
				{
					if (particles[i].mAlive)
					{
						// Star particles
						if (particles[i].mType == 4)
						{
							// pong particle effects
							fireTimer += 60;
							if (fireTimer > 60)
							{
								fireTimer = 0;
								// spawn explosion
								// This is a neat effect, but it makes firing rapidly not function - CA 2022-11-14
								//part.SpawnExplosion(particles, particles[i].mX+particles[i].mW/2, particles[i].mY+particles[i].mH/2, {244,144,20});
							}
						}
					}
				}

				// Update Players
				p1.update(part, particles, sound);

				// Do Player frames
				playerFrameTimer += playerFrameRate;
				if (playerFrameTimer > 60)
				{
					playerFrameTimer = 0;
					playerFrame++;
					if (playerFrame > 4)
					{
						playerFrame = 0;
					}
				}

				// Update Player's, manually
				if (p1.shoot)
				{
					p1.shoot = false;
					float centerX = p1.getCenterX();
					float centerY = p1.getCenterY();

					///////////////////////////////////////////////////////////////////////
					//-------------------------------------------------------------------//
					//---------------------- Handle Basic Shooting ----------------------//
					/***** Set Turret Position *****/
					float turret1w = 128;
					float turret1h = 64;
					float frigateAngle = p1.angle;
					float radians = (3.1415926536f / 180) * (p1.angle);
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
					frigateAngle = p1.angle;
					radians = (3.1415926536f / 180) * (p1.angle);
					barrelW = (60 * cos(radians)) - (32 * sin(radians));// add this to center of zombie (this will give us the guns barrel position)
					barrelH = (60 * sin(radians)) + (32 * cos(radians));
					barrelX = turret1x + turret1w / 2 - particleW / 2 + barrelW;
					barrelY = turret1y + turret1h / 2 - particleH / 2 + barrelH;
					/***** Get turrets nose angle *****/

					float newAngle = atan2(my - barrelY, mx - barrelX);
					newAngle = newAngle * 180 / (float)M_PI;

					part.spawnParticleAngle(particles, "slow", 4, barrelX, barrelY,
						particleW, particleH, newAngle, 11, 0.0f,
						{ 200, 200, 100 }, 1, 0, 0, 255, 0, 60, 0, false, 0.11f,
						false, 0.11f, false, 0.0f, Util::WHITE, 0.0f, 0.0f, 0.0f, false,
						0.0f, 0.0f, false, 0, 0.0f);
					// play sfx
					sound.playSound(SHOOT);
				}

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////// Manual Updates ////////////////////////////////////////////

				// Particle collision with Player
				if (!p1.flash)
				{
					for (int i = 0; i < 1000; i++)
					{
						if (particles[i].mAlive)
						{
							if (particles[i].mType == 3)
							{
								// Player check
								if (checkCollision(particles[i].mX, particles[i].mY,
									particles[i].mW, particles[i].mH, p1.getX(),
									p1.getY(), p1.getWidth(), p1.getHeight()))
								{

									// Hurt player
									p1.Hurt(10);

									// Flash player
									p1.flash = true;

									// remove particle
									part.Remove(particles, i);

									// spawn explosion
									part.SpawnExplosion(particles,
										particles[i].mX + particles[i].mW / 2,
										particles[i].mY + particles[i].mH / 2, { 200,
												200, 200 });

									// play sound effect
									sound.playSound(PONG_SCORE);
								}
							}
						}
					}
				}

				// Particle collision with Enemies
				for (int i = 0; i < 1000; i++)
				{
					if (particles[i].mAlive)
					{
						if (particles[i].mType == 4)
						{
							// Enemy check
							for (int j = 0; j < enemyManager.ENEMY_MAX; j++)
							{
								if (enemyManager.getEnemies()[j].alive)
								{
									// ON-HIT Collision Check
									if (checkCollision(particles[i].mX,
										particles[i].mY, particles[i].mW,
										particles[i].mH, enemyManager.getEnemies()[j].x, enemyManager.getEnemies()[j].y,
										enemyManager.getEnemies()[j].w, enemyManager.getEnemies()[j].h))
									{
										// Flash enemy
										enemyManager.getEnemies()[j].flash = true;

										// Remove enemy
										enemyManager.getEnemies()[j].health -= 25;

										// remove particle
										part.Remove(particles, i);

										// spawn explosion
										part.SpawnExplosion(particles,
											particles[i].mX + particles[i].mW / 2,
											particles[i].mY + particles[i].mH / 2,
											{ 200, 200, 200 });

										// Play SFX
										sound.playSound(PONG_SCORE);
									}
								}
							}
						}
					}
				}

				// If in Game playing scene
				if (gameScene == 1)
				{
					// If player died
					if (!p1.alive)
					{
						// Show losing screen
						gameScene = 2;
					}
				}
			}	// end gameScene 1

			enemyManager.updateEnemies(score);

			//TODO: Extract into EnemyManager once I figure out how I want to handle particles.
			// enemy shoot
			//enemyManager.enemiesShoot();

			/*for (int i = 0; i < enemyManager.ENEMY_MAX; i++)
			{
				if (enemyManager.getEnemies()[i].alive)
				{
					// Handle shoot rate
					enemyManager.getEnemies()[i].shootTimer += enemyManager.getEnemies()[i].shootRate;
					if (enemyManager.getEnemies()[i].shootTimer > 60)
					{
						enemyManager.getEnemies()[i].shootTimer = 0;
						// shoot particle
						float newX = enemyManager.getEnemies()[i].x + enemyManager.getEnemies()[i].w / 2 - 12;
						float newY = enemyManager.getEnemies()[i].y + enemyManager.getEnemies()[i].h / 2 - 9;
						part.spawnParticleAngle(particles, "slow", 3, newX,
							newY, 11, 11, randFloat(200, 225), 9, 0.0f, {
									200, 200, 200 }, 1, 1, 1, 255, 0, 60, 0,
									false, 0.11f, false, 0.11f, false, 0.0f, white,
									0.0f, 0.0f, 0.0f, false, 0.0f, 0.0f, false, 0,
									0.004f);
						// play sfx
						sound.playSound(SHOOT);
					}
				}
			}*/
		}		// end !paused

		background.moveBackgrounds();

		// Update Particles
		//ParticleUpdate(part, particles, 0, 0, mWidth, mHeight, 0, 0);
		Particle::ParticleUpdate(part, particles, 0, 0, window.getWidth(), window.getHeight(), 0, 0, window, sound);
		//part.Update(particles, 0, 0, mWidth, mHeight, 0, 0);
		part.updateStarParticles(particles, 0, 0, window.getWidth(), window.getHeight());
		part.updateBulletParticles(particles, 0, 0, window.getWidth(), window.getHeight());

		// Clear render screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		background.renderBackgrounds();

		// Render particles
		part.Render(renderer, particles, 0, 0);

		// Render floor
		SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
		SDL_RenderFillRect(renderer, &background.getFloorRect());

		// Render floor
		SDL_Rect playerPower =
		{
			(int)p1.getX() + (int)p1.getWidth(),
			(int)p1.getY() + (int)p1.getHeight() / 2 - 1,
			(int)p1.xPower * 6, 2
		};

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &playerPower);

		////// Render game objects //////

		// Render Player 1
		if (p1.alive)
		{
			gCopter.setAlpha(p1.alpha);
			gCopter.render(renderer, (int)p1.getX(), (int)p1.getY(), (int)p1.getWidth(), (int)p1.getHeight(), &rCopter[playerFrame], p1.angle);
			p1.render(renderer);
		}

		enemyManager.renderEnemies();

		////// Render Text //////

		////// Game Scenes /////

		std::stringstream tempss;
		// Before game start scene
		if (gameScene == 0)
		{
			// Render text: To start game
			tempss.str(std::string());
			tempss << "Press Space to Start.";
			font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::WHITE, font.getFont(VIGA));
			font.getTexture().render(renderer, window.getWidth() / 2 - font.getTexture().getWidth() / 2, window.getHeight() - font.getTexture().getHeight(),
				font.getTexture().getWidth(), font.getTexture().getHeight());

		}

		// Playing game scene
		else if (gameScene == 1)
		{
		}

		// Lost scene
		else if (gameScene == 2)
		{
			tempss.str(std::string());
			tempss << "You lose. Boo hoo.";
			font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::BLACK, font.getFont(VIGA));
			font.getTexture().render(renderer, window.getWidth() / 2 - font.getTexture().getWidth() / 2,
				window.getHeight() - font.getTexture().getHeight() - 22,
				font.getTexture().getWidth(), font.getTexture().getHeight());

			tempss.str(std::string());
			tempss << "Press Space to Start.";
			font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::BLACK, font.getFont(VIGA));
			font.getTexture().render(renderer, window.getWidth() / 2 - font.getTexture().getWidth() / 2,
				window.getHeight() - font.getTexture().getHeight(),
				font.getTexture().getWidth(), font.getTexture().getHeight());
		}

		// Winning scene
		else if (gameScene == 2)
		{
			tempss.str(std::string());
			tempss << "You win something!";
			font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::BLACK, font.getFont(VIGA));
			font.getTexture().render(renderer, 640 / 2 - font.getTexture().getWidth() / 2,
				360 - font.getTexture().getHeight() - 22,
				font.getTexture().getWidth(), font.getTexture().getHeight());

			tempss.str(std::string());
			tempss << "Press Space to Start again.";
			font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::BLACK, font.getFont(VIGA));
			font.getTexture().render(renderer, 640 / 2 - font.getTexture().getWidth() / 2,
				360 - font.getTexture().getHeight(),
				font.getTexture().getWidth(), font.getTexture().getHeight());
		}

		// Game paused
		if (paused)
		{
			tempss.str(std::string());
			font.getTexture().setAlpha(255);
			font.getTexture().loadFromRenderedText(renderer, "Paused", { 255,255,255 }, font.getFont(VIGA));
			font.getTexture().render(renderer, 640 / 2 - font.getTexture().getWidth() / 2, 360 - font.getTexture().getHeight() - 20,
				font.getTexture().getWidth(), font.getTexture().getHeight());
		}

		/////// Render Score Text /////////

		// Render score text top-right of screen
		tempss.str(std::string());
		if (count_digit(previousHighScore) == 1)
		{
			tempss << "Highscore: 00000" << previousHighScore;
		}
		else if (count_digit(previousHighScore) == 2)
		{
			tempss << "Highscore: 00000" << previousHighScore;
		}
		else if (count_digit(previousHighScore) == 3)
		{
			tempss << "Highscore: 0000" << previousHighScore;
		}
		else if (count_digit(previousHighScore) == 4)
		{
			tempss << "Highscore: 000" << previousHighScore;
		}
		else if (count_digit(previousHighScore) == 5)
		{
			tempss << "Highscore: 00" << previousHighScore;
		}
		else if (count_digit(previousHighScore) == 6)
		{
			tempss << "Highscore: 0" << previousHighScore;
		}
		else
		{
			tempss << "Highscore: 000000" << previousHighScore;
		}

		font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::WHITE, font.getFont(VIGA));
		font.getTexture().render(renderer, window.getWidth() - font.getTexture().getWidth() - 10, 4, font.getTexture().getWidth(), font.getTexture().getHeight());

		// Render score text top-right of screen
		tempss.str(std::string());
		if (count_digit(score) == 1)
		{
			tempss << "Score: 000000" << score;
		}
		else if (count_digit(score) == 2)
		{
			tempss << "Score: 00000" << score;
		}
		else if (count_digit(score) == 3)
		{
			tempss << "Score: 0000" << score;
		}
		else if (count_digit(score) == 4)
		{
			tempss << "Score: 000" << score;
		}
		else if (count_digit(score) == 5)
		{
			tempss << "Score: 00" << score;
		}
		else if (count_digit(score) == 6)
		{
			tempss << "Score: 0" << score;
		}
		else
		{
			tempss << "Score: 000000" << score;
		}
		font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::WHITE, font.getFont(VIGA));
		font.getTexture().render(renderer, window.getWidth() - font.getTexture().getWidth() - 10, 28, font.getTexture().getWidth(), font.getTexture().getHeight());

		//////// Debug ///////

#if DEBUG
		// Render text: To start game
		tempss.str(std::string());
		tempss << "gameScene: " << gameScene;
		font.getTexture().loadFromRenderedText(renderer, tempss.str().c_str(), Util::WHITE, font.getFont(VIGA));
		font.getTexture().render(renderer, 0, 0,
			font.getTexture().getWidth(), font.getTexture().getHeight());
#endif


		// Update screen
		SDL_RenderPresent(renderer);

		// fps
		frame++;
		if ((cap == true) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND))
			SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
	}

	// Free resources
	part.free();
	freeSDL(window);

	return 0;
}

void setup(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer, int& previousHighScore)
{
	// Make random actually random
	srand((unsigned int)time(NULL));

	// Initialize SDL
	initSDL(msg, mWindow, gRenderer);

	// Load media
	loadMedia(msg, gRenderer);

	// Load high score
	LoadHighScore(previousHighScore);
}

void initSDL(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer)
{
	// Initialize Video, Initializes the subsystems specified, in this video

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		msg.fatalError("Call to SDL_Init failed (" + string(SDL_GetError()) + ")");

	if (!mWindow.create("Ping", 2, 1, 1, 0))
		msg.fatalError("Call to 'create' (LWindow) failed");

	*gRenderer = mWindow.createRenderer();
}

static void doLoadFromFile(Message& msg, LTexture& texture, SDL_Renderer** gRenderer, const string& fileName)
{
	if (!texture.loadFromFile(gRenderer, fileName))
		msg.fatalError("Call to 'loadFromFile' failed (" + fileName + ")");
}

void loadMedia(Message& msg, SDL_Renderer** gRenderer)
{
	doLoadFromFile(msg, gCopter, gRenderer, "resource/gfx/player-copter.png");

	// Texture clips

	// Copter
	rCopter[0] = { 0,0,128,64 };
	rCopter[1] = { 128,0,128,64 };
	rCopter[2] = { 256,0,128,64 };
	rCopter[3] = { 384,0,128,64 };
	rCopter[4] = { 512,0,128,64 };
}

void LoadHighScore(int& previousHighScore)
{
	std::fstream fileTileDataL("data/highscore.txt");

	if (fileTileDataL)
		fileTileDataL >> previousHighScore;
}

void handleInput(Player& p1, int& gameScene, int& previousHighScore, int& score, bool& paused, SDL_Event& events, Sound& sound)
{
	// Key Pressed
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {

		// Switch case of all possible keyboard presses
		switch (events.key.keysym.sym) {
		case SDLK_a:
			p1.moveLeft = true;
			break;
		case SDLK_d:
			p1.moveRight = true;
			break;
		case SDLK_w:				// If W Pressed, set Player moveUp to true
			p1.moveUp = true;
			break;
		case SDLK_s:				// If S Pressed, set Player moveUp to true
			p1.moveDown = true;
			break;
		case SDLK_ESCAPE:			// pause game
			paused = (!paused);
			break;
		case SDLK_SPACE:			// start game
			ContinueGame(p1, gameScene, previousHighScore, score, sound);
			break;
		}
	}

	// Key Released
	else if (events.type == SDL_KEYUP && events.key.repeat == 0) {

		// Switch case of all possible keyboard presses
		switch (events.key.keysym.sym) {
		case SDLK_a:
			p1.moveLeft = false;
			break;
		case SDLK_d:
			p1.moveRight = false;
			break;
		case SDLK_w:	// If W Released, set Player moveUp to false
			p1.moveUp = false;
			break;
		case SDLK_s:	// If S Released, set Player moveDown to false
			p1.moveDown = false;
			break;
		case SDLK_SPACE:	// Player 1, shoot
			//p1.shoot = false;
			break;
		}
	}

	// Mouse pressed
	if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_LEFT) {
			if (!paused) {
				if (gameScene == 1) {
					//p1.holdPower = false;
					p1.shoot = true;
					//p1.xPower = 0.0;
					//p1.holdPower = true;
					//p1.shootTimer = 60;
				}
			}
		}
	}

	// Mouse pressed
	if (events.type == SDL_MOUSEBUTTONUP) {
		if (events.button.button == SDL_BUTTON_LEFT) {

		}
	}
}

void ContinueGame(Player& p1, int& gameScene, int& previousHighScore, int& score, Sound& sound)
{
	// Before game play scene, start game
	if (gameScene == 0)
	{
		// Set gamemode to play game
		gameScene = 1;

		LoadHighScore(previousHighScore);

		// Play SFX
		sound.playSound(PONG_SCORE);

		// Reset score
		score = 0;

		// Reset protection
		p1.reset();
		//protection = true;
		//protectionTimer = 0;
	}

	// Lose, reset game
	else if (gameScene == 2)
	{
		// Save highscore
		SaveHighScore(previousHighScore);

		LoadHighScore(previousHighScore);

		// Play SFX
		//Mix_PlayChannel(-1, *sPlayerHurt, false);
		sound.playSound(PLAYER_HURT);

		// Set gamemode to play game
		gameScene = 1;

		// Reset score
		score = 0;

		p1.reset();
		// Reset protection
		//protection = true;
		//protectionTimer = 0;
	}
}

void SaveHighScore(const int& score)
{
	bool saveHighscore = false;

	// Open highscore first to check value
	int tempScore = -1;
	std::fstream fileOpen("data/highscore.txt");
	fileOpen >> tempScore;
	fileOpen.close();

	// If current score is higher than previously saved score, save it
	if (score > tempScore) {
		saveHighscore = true;
	}

	// Now save high score
	if (saveHighscore) {
		std::ofstream fileSave;
		fileSave.open("data/highscore.txt");
		fileSave << score;
		fileSave.close();
	}
}

bool checkCollision(float x, float y, float w, float h, float x2, float y2, float w2, float h2)
{
	if (x + w > x2 && x < x2 + w2 && y + h > y2 && y < y2 + h2) {
		return true;
	}
	else {
		return false;
	}
}

/*float randFloat(float fMin, float fMax)
{
	float f = (float)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}*/

int count_digit(int number)
{
	return int(log10(number) + 1);
}

void freeSDL(LWindow& mWindow)
{
	gCopter.free();

	mWindow.free();

	//Quit SDL subsystems
	SDL_Quit();
}
