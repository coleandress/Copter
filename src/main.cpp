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


LTexture gText;

/// Backgrounds ///
LTexture gOrangeBG;
LTexture gSunClouds;
LTexture gCity3;
LTexture gCity2;
LTexture gCity1;

// Enemy
LTexture gTanks;
SDL_Rect rTanks[6];

// Player
LTexture gCopter;
SDL_Rect rCopter[5];


// Initialize SDL
void initSDL(LWindow& mWindow, SDL_Renderer** gRenderer) {
	// Initialize Video
	SDL_Init( SDL_INIT_VIDEO ); // initializes the subsystems specified, in this video

	// Initialize font loading
	TTF_Init();

	// Initialize audio loading
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	// Declare window dimensions
	//mWidth = screenWidth;
	//mHeight = screenHeight;

	// Declare window
	//mWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN);
	//LWindow mWindow;
	mWindow.create("Ping", 2, 1, 1, 0);

	// Declare renderer
	//gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	*gRenderer = mWindow.createRenderer();
}

// Load media
void loadMedia(
	SDL_Renderer** gRenderer,
	Mix_Music** sMusic,
	Mix_Chunk** sPongPlayer,
	Mix_Chunk** sPongScreen,
	Mix_Chunk** sPongScore,
	Mix_Chunk** sPlayerHurt,
	Mix_Chunk** sShoot,
	TTF_Font** viga
) 
{

	// load audio
	*sMusic = Mix_LoadMUS("resource/sounds/music.mp3");
	*sPongPlayer = Mix_LoadWAV("resource/sounds/pong-player.wav");
	*sPongScreen = Mix_LoadWAV("resource/sounds/pong-screen.wav");
	*sPongScore = Mix_LoadWAV("resource/sounds/pong-score.wav");
	*sPlayerHurt = Mix_LoadWAV("resource/sounds/player-hurt.wav");
	*sShoot = Mix_LoadWAV("resource/sounds/player-shoot.wav");

	gOrangeBG.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_08_1920 x 1080.png");
	gSunClouds.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_07_1920 x 1080.png");
	gCity3.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_06_1920 x 1080.png");
	gCity2.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_05_1920 x 1080.png");
	gCity1.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_04_1920 x 1080.png");

	gTanks.loadFromFile(gRenderer, "resource/gfx/tanks.png");
	gCopter.loadFromFile(gRenderer, "resource/gfx/player-copter.png");

	// load fonts
	*viga = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 24); //this opens a font style and sets a size

	// Texture clips

	// Tanks
	rTanks[0] = {0,0,32,32};
	rTanks[1] = {32,0,32,32};
	rTanks[2] = {64,0,32,32};
	rTanks[3] = {0,32,32,32};
	rTanks[4] = {32,32,32,32};
	rTanks[5] = {64,32,32,32};

	// Copter
	rCopter[0] = {0,0,128,64};
	rCopter[1] = {128,0,128,64};
	rCopter[2] = {256,0,128,64};
	rCopter[3] = {384,0,128,64};
	rCopter[4] = {512,0,128,64};

}

// Free SDL
void freeSDL(
	LWindow& mWindow,
	Mix_Music** sMusic,
	Mix_Chunk** sPongPlayer,
	Mix_Chunk** sPongScreen,
	Mix_Chunk** sPongScore,
	Mix_Chunk** sPlayerHurt,
	Mix_Chunk** sShoot
	) 
{
	gText.free();
	gOrangeBG.free();
	gSunClouds.free();
	gCity3.free();
	gCity2.free();
	gCity1.free();
	gTanks.free();
	gCopter.free();

	// Free audio
	Mix_FreeMusic(*sMusic);
	Mix_FreeChunk(*sPongPlayer);
	Mix_FreeChunk(*sPongScreen);
	Mix_FreeChunk(*sPongScore);
	Mix_FreeChunk(*sPlayerHurt);
	Mix_FreeChunk(*sShoot);

	mWindow.free();

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

int count_digit(int number) {
   return int(log10(number) + 1);
}

void SaveHighScore(const int& score) {
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

void LoadHighScore(int& previousHighScore) {
	std::fstream fileTileDataL("data/highscore.txt");
	fileTileDataL >> previousHighScore;
	fileTileDataL.close();
}

bool checkCollision(float x, float y, float w, float h, float x2, float y2, float w2, float h2) {
	if (x+w > x2 && x < x2+w2 && y+h > y2 && y < y2+h2) {
		return true;
	}else {
		return false;
	}
}


struct Gun {
	float atkSpeed;
	Gun(float newAtkSpe) {
		atkSpeed = newAtkSpe;
	}
};

struct SDL_RectM
{
    float x, y;
    float w, h;
};

void ContinueGame(Player &p1, int& gameScene, int& previousHighScore, int& score, Mix_Chunk** sPongScore, Mix_Chunk** sPlayerHurt) {

	// Before game play scene, start game
	if (gameScene == 0)
	{
		// Set gamemode to play game
		gameScene = 1;

		LoadHighScore(previousHighScore);

		// Play SFX
		Mix_PlayChannel(-1, *sPongScore, false);

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
		Mix_PlayChannel(-1, *sPlayerHurt, false);

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

void setup(
	LWindow& mWindow,
	SDL_Renderer** gRenderer,
	int& previousHighScore,
	Mix_Music** sMusic,
	Mix_Chunk** sPongPlayer,
	Mix_Chunk** sPongScreen,
	Mix_Chunk** sPongScore,
	Mix_Chunk** sPlayerHurt,
	Mix_Chunk** sShoot,
	TTF_Font** viga
	)
{

	// Make random actually random
	srand((unsigned int)time(NULL));

	// Initialize SDL
	initSDL(mWindow, gRenderer);

	// Load media
	loadMedia(gRenderer, sMusic, sPongPlayer, sPongScreen, sPongScore, sPlayerHurt, sShoot, viga);

	// Load high score
	LoadHighScore(previousHighScore);
}

void handleInput(Player& p1, int& gameScene, int& previousHighScore, int& score, bool& paused, SDL_Event& events, Mix_Chunk** sPongScore, Mix_Chunk** sPlayerHurt)
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
			case SDLK_s	:				// If S Pressed, set Player moveUp to true
				p1.moveDown = true;
				break;
			case SDLK_ESCAPE:			// pause game
				paused = (!paused);
				break;
			case SDLK_SPACE:			// start game
				ContinueGame(p1, gameScene, previousHighScore, score, sPongScore, sPlayerHurt);
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

// Generate a random double number
double randDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

float randFloat(float fMin, float fMax)
{
	float f = (float)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

bool checkCollision(float x, float y, int w, int h, float x2, float y2, int w2, int h2) {
	if (x + w > x2 && x < x2 + w2 && y + h > y2 && y < y2 + h2) {
		return true;
	}
	else {
		return false;
	}
}

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
	SDL_Renderer* renderer{};
	int gameScene{ 0 };
	//int highscore{ -1 };
	int previousHighScore{ -1 };
	int score{ 0 };
	Mix_Music* sMusic{ nullptr };
	Mix_Chunk* sPongPlayer{ nullptr };
	Mix_Chunk* sPongScreen{ nullptr };
	Mix_Chunk* sPongScore{ nullptr };
	Mix_Chunk* sPlayerHurt{ nullptr };
	Mix_Chunk* sShoot{ nullptr };
	TTF_Font* viga{ nullptr };

	setup(
		window,
		&renderer,
		previousHighScore,
		&sMusic,
		&sPongPlayer,
		&sPongScreen,
		&sPongScore,
		&sPlayerHurt,
		&sShoot,
		&viga
	);

	bool paused = false;

	// Create pointer for events
	SDL_Event events; 
	
	// Colors
	SDL_Color black = { 0, 0, 0, 255 };
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color orange = { 180, 90, 20, 255 };

	// Game loop
	bool quit = false;

	// Particles
	Particle part;
	Particle particles[1000];
	part.init(particles);
	part.load(&renderer);

	// Create Guns
	Gun M16(12.5);

	// Create Player 1
	Player p1;
	p1.init(sPongScore);

	// Timer
    Timer fps;
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;

	// Set Player 1 position
	p1.setPosition(64, 360/2-112/2);

	// game booleans
	float fireTimer = 0;
	float fireRate = 15;

	// Enemy Variables
	int enemyCount = 0;
	const int enemyMax = 64;
	float enemySpawnTimer = 0.0; 
	Enemy enemy[enemyMax];
	Enemy::initEnemy(enemy, enemyCount, enemyMax);

	// Player Variables
	const float playerFrameRate = 15;
	float playerFrameTimer = 0;
	int playerFrame = 0;

	// World
	//SDL_Rect floor = {0, mHeight-28, mWidth, 28};
	SDL_Rect floor = {0, window.getHeight() - 28, window.getWidth(), 28};
	SDL_RectM rSunClouds = {0, 0, 640, 360};
	SDL_RectM rCity3 = {0, 0, 640, 360};
	SDL_RectM rCity2 = {0, 0, 640, 360};
	SDL_RectM rCity1 = {0, 0, 640, 360};

	// Call before loop
	gameScene = 0;
	paused = false;
	quit = false;
	fireTimer = 0;
	fireRate = 15;
	playerFrame = 0;

	Mix_FadeInMusic(sMusic, -1, 2000);

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
				handleInput(p1, gameScene, previousHighScore, score, paused, events, &sPongScore, &sPlayerHurt);
			}
		}

		// If game paused
		if (!paused == 1) {
			if (gameScene == 1) {

				// particle test for particles
				for (int i = 0; i < part.mMax; i++) {
					if (particles[i].mAlive) {
						// Star particles
						if (particles[i].mType == 4) {
							// pong particle effects
							fireTimer += 60;
							if (fireTimer > 60) {
								fireTimer = 0;
								// spawn explosion
								//part.SpawnExplosion(particles, particles[i].x+particles[i].w/2, particles[i].y+particles[i].h/2, {244,144,20});
							}
						}
					}
				}

				// Update Players
				p1.update(part, particles);

				// Do Player frames
				playerFrameTimer += playerFrameRate;
				if (playerFrameTimer > 60) {
					playerFrameTimer = 0;
					playerFrame++;
					if (playerFrame > 4) {
						playerFrame = 0;
					}
				}

				// Update Player's, manually
				if (p1.shoot) {
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
							false, 0.11f, false, 0.0f, white, 0.0f, 0.0f, 0.0f, false,
							0.0f, 0.0f, false, 0, 0.0f);
					// play sfx
					Mix_PlayChannel(-1, sShoot, false);
				}

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////// Manual Updates ////////////////////////////////////////////

				// Particle collision with Player
				if (!p1.flash) {
					for (int i = 0; i < 1000; i++) {
						if (particles[i].mAlive) {
							if (particles[i].mType == 3) {
								// Player check
								if (checkCollision(particles[i].mX, particles[i].mY,
										particles[i].mW, particles[i].mH, p1.getX(),
										p1.getY(), p1.getWidth(), p1.getHeight())) {

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
									Mix_PlayChannel(-1, sPongScore, 0);
								}
							}
						}
					}
				}

				// Particle collision with Enemies
				for (int i = 0; i < 1000; i++) {
					if (particles[i].mAlive) {
						if (particles[i].mType == 4) {
							// Enemy check
							for (int j = 0; j < enemyMax; j++) {
								if (enemy[j].alive) {

									// ON-HIT Collision Check
									if (checkCollision(particles[i].mX,
											particles[i].mY, particles[i].mW,
											particles[i].mH, enemy[j].x, enemy[j].y,
											enemy[j].w, enemy[j].h)) {

										// Flash enemy
										enemy[j].flash = true;

										// Remove enemy
										enemy[j].health -= 25;

										// remove particle
										part.Remove(particles, i);

										// spawn explosion
										part.SpawnExplosion(particles,
												particles[i].mX + particles[i].mW / 2,
												particles[i].mY + particles[i].mH / 2,
												{ 200, 200, 200 });

										// Play SFX
										Mix_PlayChannel(-1, sPongScore, 0);
									}
								}
							}
						}
					}
				}

				// If in Game playing scene
				if (gameScene == 1) {

					// If player died
					if (!p1.alive) {

						// Show losing screen
						gameScene = 2;
					}
				}
			}	// end gameScene 1

			// Update Enemies
			Enemy::updateEnemy(enemy, enemyCount, enemyMax, score);

			// Spawn Enemies
			enemySpawnTimer += 0.22f;
			if (enemySpawnTimer > 60) {
				enemySpawnTimer = 0;

				// Spawn a random enemy a few pixels to the right of the scree
				//spawnEnemy(enemy, 1280 + rand() % 100, ground - 64 - 32 + 5, 64, // I changed this need to indicate the height is the ground CA 2022-11-10
				Enemy::spawnEnemy(
					enemy, 
					1280.0f + rand() % 100, 
					window.getHeight() - 64.0f - 32.0f + 5.0f, 
					64,
					64, 
					rand() % 3, 
					enemyCount, 
					enemyMax
				);
			}

			// enemy shoot
			for (int i = 0; i < enemyMax; i++) {
				if (enemy[i].alive) {
					// Handle shoot rate
					enemy[i].shootTimer += enemy[i].shootRate;
					if (enemy[i].shootTimer > 60) {
						enemy[i].shootTimer = 0;
						// shoot particle
						float newX = enemy[i].x + enemy[i].w / 2 - 12;
						float newY = enemy[i].y + enemy[i].h / 2 - 9;
						part.spawnParticleAngle(particles, "slow", 3, newX,
								newY, 11, 11, randFloat(200, 225), 9, 0.0f, {
										200, 200, 200 }, 1, 1, 1, 255, 0, 60, 0,
								false, 0.11f, false, 0.11f, false, 0.0f, white,
								0.0f, 0.0f, 0.0f, false, 0.0f, 0.0f, false, 0,
								0.004f);
						// play sfx
						Mix_PlayChannel(-1, sShoot, false);
					}
				}
			}
		}		// end !paused

		// Move backgrounds
		rSunClouds.x -= 0.05f;
		rCity3.x -= 2;
		rCity2.x -= 3;
		rCity1.x -= 4;

		if (rSunClouds.x < -1280) {
			rSunClouds.x = 0;
		}
		if (rCity3.x < -1280) {
			rCity3.x = 0;
		}

		if (rCity2.x < -1280) {
			rCity2.x = 0;
		}

		if (rCity1.x < -1280) {
			rCity1.x = 0;
		}

		// Update Particles
		//ParticleUpdate(part, particles, 0, 0, mWidth, mHeight, 0, 0);
		Particle::ParticleUpdate(part, particles, 0, 0, window.getWidth(), window.getHeight(), 0, 0, window, sPongScore);
		//part.Update(particles, 0, 0, mWidth, mHeight, 0, 0);
		part.updateStarParticles(particles, 0, 0, window.getWidth(), window.getHeight());
		part.updateBulletParticles(particles, 0, 0, window.getWidth(), window.getHeight());

		// Clear render screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

			// Layer 0, Orange Background
			gOrangeBG.render(renderer, 0, 0, window.getWidth(), window.getHeight());

			// Layer 1, Sun and Clouds
			gSunClouds.render(renderer, (int)rSunClouds.x, (int)rSunClouds.y, window.getWidth(), window.getHeight());

			// Layer 2, City
			gCity3.render(renderer, (int)rCity3.x+1280, (int)rCity3.y, window.getWidth(), window.getHeight());
			gCity3.render(renderer, (int)rCity3.x, (int)rCity3.y, window.getWidth(), window.getHeight());

			// Layer 3, City
			gCity2.render(renderer, (int)rCity2.x, (int)rCity2.y, window.getWidth(), window.getHeight());
			gCity2.render(renderer, (int)rCity2.x+1280, (int)rCity2.y, window.getWidth(), window.getHeight());

			// Layer 4, City
			gCity1.render(renderer, (int)rCity1.x, (int)rCity1.y, window.getWidth(), window.getHeight());
			gCity1.render(renderer, (int)rCity1.x+1280, (int)rCity1.y, window.getWidth(), window.getHeight());

			// Render particles
			part.Render(renderer, particles, 0, 0);

			// Render floor
			SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
			SDL_RenderFillRect(renderer, &floor);

			// Render floor
			SDL_Rect playerPower = { 
				(int)p1.getX() + (int)p1.getWidth(), 
				(int)p1.getY() + (int)p1.getHeight()/2-1, 
				(int)p1.xPower*6, 2 
			};

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &playerPower);

			////// Render game objects //////

			// Render Player 1
			if (p1.alive) {
				gCopter.setAlpha(p1.alpha);
				gCopter.render(renderer, (int)p1.getX(), (int)p1.getY(), (int)p1.getWidth(), (int)p1.getHeight(), &rCopter[playerFrame], p1.angle);
				p1.render(renderer);
			}

			// Render enemy
			Enemy::renderEnemy(enemy, 0, 0, *renderer, gTanks, rTanks, enemyMax);

			////// Render Text //////

			////// Game Scenes /////

			std::stringstream tempss;
			// Before game start scene
			if (gameScene == 0) {

				// Render text: To start game
				tempss.str(std::string());
				tempss << "Press Space to Start.";
				gText.loadFromRenderedText(renderer, tempss.str().c_str(), white, viga);
				gText.render(renderer, window.getWidth()/2 - gText.getWidth()/2, window.getHeight() - gText.getHeight(),
							 gText.getWidth(), gText.getHeight());

			}

			// Playing game scene
			else if (gameScene == 1)
			{

			}

			// Lost scene
			else if (gameScene == 2) {
				tempss.str(std::string());
				tempss << "You lose. Boo hoo.";
				gText.loadFromRenderedText(renderer, tempss.str().c_str(), black, viga);
				gText.render(renderer, window.getWidth()/2 - gText.getWidth()/2,
						window.getHeight() - gText.getHeight()-22,
							 gText.getWidth(), gText.getHeight());

				tempss.str(std::string());
				tempss << "Press Space to Start.";
				gText.loadFromRenderedText(renderer, tempss.str().c_str(), black, viga);
				gText.render(renderer, window.getWidth()/2 - gText.getWidth()/2,
						window.getHeight() - gText.getHeight(),
							 gText.getWidth(), gText.getHeight());
			}

			// Winning scene
			else if (gameScene == 2) {
				tempss.str(std::string());
				tempss << "You win something!";
				gText.loadFromRenderedText(renderer, tempss.str().c_str(), black, viga);
				gText.render(renderer, 640/2 - gText.getWidth()/2,
						 360 - gText.getHeight()-22,
							 gText.getWidth(), gText.getHeight());

				tempss.str(std::string());
				tempss << "Press Space to Start again.";
				gText.loadFromRenderedText(renderer, tempss.str().c_str(), black, viga);
				gText.render(renderer, 640/2 - gText.getWidth()/2,
						 360 - gText.getHeight(),
							 gText.getWidth(), gText.getHeight());
			}

			// Game paused
			if (paused) {

				tempss.str(std::string());
				gText.setAlpha(255);
				gText.loadFromRenderedText(renderer, "Paused", {255,255,255}, viga);
				gText.render(renderer, 640/2-gText.getWidth()/2, 360 - gText.getHeight() - 20,
										gText.getWidth(), gText.getHeight());
			}

			/////// Render Score Text /////////

			// Render score text top-right of screen
			tempss.str(std::string());
			if (count_digit(previousHighScore) == 1) {
				tempss << "Highscore: 00000" << previousHighScore;
			}
			else if (count_digit(previousHighScore) == 2) {
				tempss << "Highscore: 00000" << previousHighScore;
			}
			else if (count_digit(previousHighScore) == 3) {
				tempss << "Highscore: 0000" << previousHighScore;
			}
			else if (count_digit(previousHighScore) == 4) {
				tempss << "Highscore: 000" << previousHighScore;
			}
			else if (count_digit(previousHighScore) == 5) {
				tempss << "Highscore: 00" << previousHighScore;
			}
			else if (count_digit(previousHighScore) == 6) {
				tempss << "Highscore: 0" << previousHighScore;
			} else {
				tempss << "Highscore: 000000" << previousHighScore;
			}

			gText.loadFromRenderedText(renderer, tempss.str().c_str(), white, viga);
			gText.render(renderer, window.getWidth() - gText.getWidth() - 10, 4, gText.getWidth(), gText.getHeight());

			// Render score text top-right of screen
			tempss.str(std::string());
			if (count_digit(score) == 1) {
				tempss << "Score: 000000" << score;
			}
			else if (count_digit(score) == 2) {
				tempss << "Score: 00000" << score;
			}
			else if (count_digit(score) == 3) {
				tempss << "Score: 0000" << score;
			}
			else if (count_digit(score) == 4) {
				tempss << "Score: 000" << score;
			}
			else if (count_digit(score) == 5) {
				tempss << "Score: 00" << score;
			}
			else if (count_digit(score) == 6) {
				tempss << "Score: 0" << score;
			} else {
				tempss << "Score: 000000" << score;
			}
			gText.loadFromRenderedText(renderer, tempss.str().c_str(), white, viga);
			gText.render(renderer, window.getWidth() - gText.getWidth() - 10, 28, gText.getWidth(), gText.getHeight());

			//////// Debug ///////

#if DEBUG
			// Render text: To start game
			tempss.str(std::string());
			tempss << "gameScene: " << gameScene;
			gText.loadFromRenderedText(renderer, tempss.str().c_str(), white, viga);
			gText.render(renderer, 0, 0,
						 gText.getWidth(), gText.getHeight());
#endif


		// Update screen
		SDL_RenderPresent(renderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}

	// Free resources
	part.free();
	freeSDL(window, &sMusic, &sPongPlayer, &sPongScreen, &sPongScore, &sPlayerHurt, &sShoot);

	return 0;
}
