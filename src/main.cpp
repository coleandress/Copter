
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>       /* time */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* srand, rand */
#include <math.h>       /* sin */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Particle.h"
#include "Player.h"


//const int screenWidth = 270;
//const int screenHeight = 152;
//const int screenWidth = 640;
//const int screenHeight = 360;

/// Constants ///

// Create window dimensions
const int screenWidth = 1280;
const int screenHeight = 720;

// ground
const float ground = 720;

int mWidth;
int mHeight;

// Scores

/* 0: Before game scene
 * 1: Game playing scene
 * 2: Game lost scene
 * 3: Game winner scene?
 *
 */
int gameScene = 0;
int highscore = -1;
int previousHighScore = -1;
int score = 0;

// Bools
bool paused = false;

// Create pointer for events
SDL_Event events;

// Create window
SDL_Window *mWindow = NULL;

// Create renderer
SDL_Renderer *gRenderer;

// Game Music
Mix_Music *sMusic 	= NULL;

// Create player hitting pong sound effect
Mix_Chunk *sPongPlayer = NULL;

// Create pong hitting screen sound effect
Mix_Chunk *sPongScreen = NULL;

// Create pong scoring
Mix_Chunk *sPongScore = NULL;

// Create player hurt
Mix_Chunk *sPlayerHurt = NULL;

// Create shoot audio
Mix_Chunk *sShoot = NULL;

// Create font variable
TTF_Font* viga = NULL;

// Colors
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color blue = { 0, 0, 255, 255 };
SDL_Color green = { 0, 255, 0, 255 };
SDL_Color red = { 255, 0, 0, 255 };
SDL_Color white = { 255, 255, 255, 255 };
SDL_Color orange = { 180, 90, 20, 255 };

// Create Surface to put font text on
SDL_Surface *surface1 = NULL;

// Create Texture from a Surface
SDL_Texture *texture1 = NULL;

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

// Generate a random double number
double randDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

bool checkCollision(float x, float y, int w, int h, float x2, float y2, int w2, int h2) {
	if (x+w > x2 && x < x2+w2 && y+h > y2 && y < y2+h2) {
		return true;
	}else {
		return false;
	}
}

// Initialize SDL
void initSDL() {
	// Initialize Video
	SDL_Init( SDL_INIT_VIDEO ); // initializes the subsystems specified, in this video

	// Initialize font loading
	TTF_Init();

	// Initialize audio loading
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	// Declare window dimensions
	mWidth = screenWidth;
	mHeight = screenHeight;

	// Declare window
	mWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN);

	// Declare renderer
	gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

// Load media
void loadMedia() {

	// load audio
	sPongPlayer = Mix_LoadWAV("pong-player.wav");
	sPongScreen = Mix_LoadWAV("pong-screen.wav");
	sPongScore = Mix_LoadWAV("pong-score.wav");
	sPlayerHurt = Mix_LoadWAV("player-hurt.wav");
	sShoot = Mix_LoadWAV("player-shoot.wav");
	sMusic = Mix_LoadMUS("resource/sounds/music.mp3");

	gOrangeBG.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_08_1920 x 1080.png");
	gSunClouds.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_07_1920 x 1080.png");
	gCity3.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_06_1920 x 1080.png");
	gCity2.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_05_1920 x 1080.png");
	gCity1.loadFromFile(gRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_04_1920 x 1080.png");

	gTanks.loadFromFile(gRenderer, "resource/gfx/tanks.png");
	gCopter.loadFromFile(gRenderer, "resource/gfx/player-copter.png");

	// load fonts
	viga = TTF_OpenFont("viga-regular.ttf", 24); //this opens a font style and sets a size

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
void freeSDL() {
	// Free texture
	SDL_DestroyTexture(texture1);

	// Free surface
	SDL_FreeSurface(surface1);
	gText.free();
	gOrangeBG.free();
	gSunClouds.free();
	gCity3.free();
	gCity2.free();
	gCity1.free();
	gTanks.free();
	gCopter.free();

	// Free audio
	Mix_FreeChunk(sPongPlayer);
	Mix_FreeChunk(sPongScreen);
	Mix_FreeChunk(sPongScore);
	Mix_FreeChunk(sPlayerHurt);
	Mix_FreeChunk(sShoot);
	Mix_FreeMusic(sMusic);

	// Close upon exit
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(mWindow);

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

int count_digit(int number) {
   return int(log10(number) + 1);
}

void SaveHighScore() {
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

void LoadHighScore() {
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////// ENEMY ////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------//
int enemyCount = 0;
const int enemyMax = 64;
struct Enemy {
	float x, y;
	float w, h;
	/*
	 * 0: Trooper
	 * 1: Medium Trooper
	 * 2: Heavy Trooper
	 */
	int type;
	float health;
	float shootRate;
	float shootTimer;
	float alpha;
	int flashTimer;
	bool flash;
	bool alive;
};

void initEnemy(Enemy enemy[]) {
	enemyCount = 0;
	for (int i=0; i<enemyMax; i++) {
		enemy[i].x = -100;
		enemy[i].y = -100;
		enemy[i].w = 32;
		enemy[i].h = 32;
		enemy[i].shootRate = 0;
		enemy[i].shootTimer = 0;
		enemy[i].type = 0;
		enemy[i].alpha = 255;
		enemy[i].flashTimer = 0;
		enemy[i].flash = false;
		enemy[i].alive = false;
	}
}

void spawnEnemy(Enemy enemy[], float x, float y, float w, float h, int type) {
	for (int i=0; i<enemyMax; i++) {
		if (!enemy[i].alive) {
			enemy[i].x = x;
			enemy[i].y = y;
			enemy[i].w = w;
			enemy[i].h = h;
			enemy[i].type = type;
			if (type == 0) {
				enemy[i].health = 100;
				enemy[i].shootRate = 1.09;
				enemy[i].shootTimer = 0;
			}
			else if (type == 1) {
				enemy[i].health = 175;
				enemy[i].shootRate = 0.89;
				enemy[i].shootTimer = 0;
			}
			else if (type == 2) {
				enemy[i].health = 300;
				enemy[i].shootRate = 0.66;
				enemy[i].shootTimer = 0;
			}
			enemy[i].alpha = 255;
			enemy[i].flashTimer = 0;
			enemy[i].flash = false;
			enemy[i].alive = true;
			enemyCount++;
			break;
		}
	}
}

void updateEnemy(Enemy enemy[]) {
	for (int i=0; i<enemyMax; i++) {
		if (enemy[i].alive) {

			// enemy constantly moving left
			if (enemy[i].type == 0) {
				enemy[i].x -= 2.77;
			}
			else if (enemy[i].type == 1) {
				enemy[i].x -= 2.89;
			}
			else if (enemy[i].type == 2) {
				enemy[i].x -= 3.34;
			}

			// Flash enemy
			if (enemy[i].flash) {

				enemy[i].flashTimer += 3;

				if (enemy[i].flashTimer < 15) {
					enemy[i].alpha = 90;
				}

				else if (enemy[i].flashTimer >= 15 && enemy[i].flashTimer < 30) {
					enemy[i].alpha = 170;
				}

				else if (enemy[i].flashTimer >= 30 && enemy[i].flashTimer < 45) {
					enemy[i].alpha = 90;
				}

				else if (enemy[i].flashTimer >= 45 && enemy[i].flashTimer < 60) {
					enemy[i].alpha = 170;
				} else {
					enemy[i].alpha = 255;
					enemy[i].flashTimer = 0;
					enemy[i].flash = false;
				}
			}

			// enemy death by health
			if (enemy[i].health <= 0) {

				// Add player score
				if (enemy[i].type == 0) {
					score += 10;
				}
				else if (enemy[i].type == 1) {
					score += 20;
				}
				else if (enemy[i].type == 2) {
					score += 30;
				}

				// Remove enemy
				enemy[i].alive = false;
				enemyCount--;
			}

			// enemy death by border
			if (enemy[i].x < 0 - enemy[i].w) {
				enemy[i].alive = false;
				enemyCount--;
			}
		}
	}
}

void renderEnemy(Enemy enemy[], float camx, float camy) {
	for (int i=0; i<enemyMax; i++) {
		if (enemy[i].alive) {
			if (enemy[i].flash) {
				gTanks.setAlpha(enemy[i].alpha);
				gTanks.render(gRenderer, enemy[i].x-camx, enemy[i].y-camy,  enemy[i].w, enemy[i].h, &rTanks[enemy[i].type]);
			} else {
				gTanks.setAlpha(enemy[i].alpha);
				gTanks.render(gRenderer, enemy[i].x-camx, enemy[i].y-camy,  enemy[i].w, enemy[i].h, &rTanks[enemy[i].type]);
			}
			/*SDL_Rect playerPower = {enemy[i].x-camx, enemy[i].y-camy,  enemy[i].w, enemy[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &playerPower);*/
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// PARTICLES //////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------//
void ParticleUpdate(Particle &part, Particle particle[], int mapX, int mapY, int mapW, int mapH,
					  float camx, float camy) {
	for (int i = 0; i < 1000; i++) {
		if (particle[i].alive)
		{
			// If there is a timer before moving, do timer first before handling Particle
			if (particle[i].timerBeforeMoving != 0) {
				particle[i].timerBeforeMoving -= 1;
			}else{
				// Play one time sound effect
				if (particle[i].playSFXBeforeMoving) {
					particle[i].playSFXBeforeMoving = false;
					// play SFX
					//Mix_PlayChannel(-1, sFireBall, 0);
				}
			}

			///////////////////////////////////////////////////////////

			// Particle Y gravity
			particle[i].vY += particle[i].grav;

			// Particle movement
			particle[i].x += particle[i].vX * particle[i].speed;
			particle[i].y += particle[i].vY * particle[i].speed;

			// Speed decay of grenade
			if (particle[i].decay) {
				particle[i].speed = particle[i].speed - particle[i].speed * particle[i].decaySpeed;
			}
			// Particle death, upon size
			if (particle[i].sizeDeath) {
				particle[i].w -= particle[i].deathSpe;
				particle[i].h -= particle[i].deathSpe;

			}
			// Particle spin
			particle[i].angle += particle[i].angleSpe * particle[i].angleDir;
			// Particle death, Time
			particle[i].time += particle[i].deathTimerSpeed;
			// Particle death, transparency
			particle[i].alpha -= particle[i].alphaspeed;
			//////////////////////////////////////////////////////////

			// particle center
			particle[i].x2 = particle[i].x + particle[i].w/2;
			particle[i].y2 = particle[i].y + particle[i].h/2;

			// get particle radius
			particle[i].radius = particle[i].w;

			//If the tile is in the screen
			if (particle[i].x + particle[i].w > camx && particle[i].x < camx + screenWidth
			 && particle[i].y + particle[i].h > camy && particle[i].y < camy + screenHeight) {
				particle[i].onScreen = true;
			} else {
				particle[i].onScreen = false;
			}

			// Particle trail
			/*if (particle[i].trail) {
				particle[i].trailTimer += particle[i].trailRate;
				if (particle[i].trailTimer > 60) {
					particle[i].trailTimer = 0;
					float trajAngle = atan2(particle[i].vY,particle[i].vX);
					trajAngle = trajAngle * (180 / 3.1416);
					//Set player angle max limits
					if (trajAngle < 0) {
						trajAngle = 360 - (-trajAngle);
					}
					float radians = (3.1415926536/180)*(trajAngle);
					////////////////////////////////////////////////
					float barrelW  = (5 * cos(radians) ) - (0 * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
					float barrelH  = (5 * sin(radians) ) + (0 * cos(radians) );
					////////////////////////////////////////////////
					// spawn explosion particle effect
					for (double h=0.0; h< 90.0; h+=rand() % 5 + 2){
						int rands = randDouble(particle[i].trailMinSize, particle[i].trailMaxSize);
						float newX = particle[i].x+particle[i].w/2;
						float newY = particle[i].y+particle[i].h/2;
						spawnParticleAngle(particle, "none", 4,
											newX-rands/2 + barrelW,
											newY-rands/2 + barrelH,
										   rands, rands,
										   (trajAngle+180)+h-45, randDouble(1.1, 2.1),
										   0.0,
										   particle[i].trailColor, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 10 + 10,
										   60, 0,
										   true, 0.11,
										   true, randDouble(0.005, 0.6));
					}
				}
			}*/

			///////////////////////////////////////////////////////////////////////////////
			/////////////////////////// Set Corners of a Particle /////////////////////////
			//---------------------------------------------------------------------------//
			float particleCX = particle[i].x+particle[i].w/2;
			float particleCY = particle[i].y+particle[i].h/2;
			float particleAngle = particle[i].angle;
			float radians   = (3.1415926536/180)*(particle[i].angle);
			float Cos 		= floor(cos(radians)*100+0.05)/100;
			float Sin 		= floor(sin(radians)*100+0.05)/100;

			// Top Right corner
		    float barrelW  = ((particle[i].w/2) * Cos ) - (-(particle[i].h/2) * Sin );
		    float barrelH  = ((particle[i].w/2) * Sin ) + (-(particle[i].h/2) * Cos );
		    float barrelX = particleCX + barrelW;
		    float barrelY = particleCY + barrelH;
			particle[i].A.x = barrelX;
			particle[i].A.y = barrelY;

			// Bottom Right corner
			barrelW  = ((particle[i].w/2) * Cos ) - ((particle[i].h/2) * Sin );
			barrelH  = ((particle[i].w/2) * Sin ) + ((particle[i].h/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].B.x = barrelX;
			particle[i].B.y = barrelY;

			// Top Left corner
			barrelW  = (-(particle[i].w/2) * Cos ) - (-(particle[i].h/2) * Sin );
			barrelH  = (-(particle[i].w/2) * Sin ) + (-(particle[i].h/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].C.x = barrelX;
			particle[i].C.y = barrelY;

			// Bottom Left corner
			barrelW  = (-(particle[i].w/2) * Cos ) - ((particle[i].h/2) * Sin );
			barrelH  = (-(particle[i].w/2) * Sin ) + ((particle[i].h/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].D.x = barrelX;
			particle[i].D.y = barrelY;

			// Handle different types of deaths
			if (particle[i].time > particle[i].deathTimer) {
				// remove particle
				part.Remove(particle, i);
				// spawn explosion
				//SpawnFireExplosion(particle, particle[i].x+particle[i].w/2, particle[i].y+particle[i].h/2, {200,200,100});
				part.SpawnExplosion(particle, particle[i].x+particle[i].w/2, particle[i].y+particle[i].h/2, {200,200,200});
			}
			else if (particle[i].alpha < 0) {
				part.Remove(particle, i);
			}
			else if (particle[i].w <= 0 || particle[i].h <= 0){
				part.Remove(particle, i);
			}
			// Ground bounce
			else if (particle[i].y + particle[i].h > ground - 32 && particle[i].alphaspeed == 0) {
				particle[i].onScreen = true;
				// remove particle
				part.Remove(particle, i);
				// spawn explosion
				//SpawnFireExplosion(particle, particle[i].x+particle[i].w/2, particle[i].y+particle[i].h/2, {200,200,100});
				part.SpawnExplosion(particle, particle[i].x+particle[i].w/2, particle[i].y+particle[i].h/2, {200,200,100});
				// play sound effect
				Mix_PlayChannel(-1, sPongScore, 0);
			}

			// Particle map collision
			/*else if (particle[i].x+particle[i].w < mapX) {
				if (particle[i].damage > 0) {
					//Mix_PlayChannel(-1, sSpellExplode, 0);
					//SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				Remove(particle, i);
				//particle[i].x = mapX+mapW-particle[i].w;
			}
			else if (particle[i].x > mapX+mapW) {
				if (particle[i].damage > 0) {
					//Mix_PlayChannel(-1, sSpellExplode, 0);
					//SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				Remove(particle, i);
				//particle[i].x = mapX-particle[i].w;
			}
			else if (particle[i].y+particle[i].h < mapY) {
				if (particle[i].damage > 0) {
					//Mix_PlayChannel(-1, sSpellExplode, 0);
					//SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				Remove(particle, i);
				//particle[i].y = mapY+mapH-particle[i].h;
			}
			else if (particle[i].y > mapY+mapH) {
				if (particle[i].damage > 0) {
					//Mix_PlayChannel(-1, sSpellExplode, 0);
					//SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				Remove(particle, i);
				//particle[i].y = mapY-particle[i].h;
			}*/

			// particle circle collision check with other particles
			/*for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (particle[j].alive) {
						if (particle[j].tag != particle[i].tag) {
							if (particle[j].damage > 0 &&  particle[i].damage > 0) {
								float bmx = particle[j].x+particle[j].w/2;
								float bmy = particle[j].y+particle[j].h/2;
								float bmx2 = particle[i].x+particle[i].w/2;
								float bmy2 = particle[i].y+particle[i].h/2;
								float angle = atan2(bmy - bmy2,bmx - bmx2);
								angle = angle * (180 / 3.1416);
								if (angle < 0) {
									angle = 360 - (-angle);
								}
								float radians = (3.1415926536/180)*(angle);
								float Cos = floor(cos(radians)*10+0.5)/10;
								float Sin = floor(sin(radians)*10+0.5)/10;
								float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
													  (bmy - bmy2) * (bmy - bmy2));
								if (distance <= 1) {
									distance = 1;
								}
								if (distance < 2 + 2) {
									//particle[i].vX -= 1 * Cos;
									//particle[i].vY -= 1 * Sin;
									Mix_PlayChannel(-1, sSpellExplode, 0);
									float bmx, bmy, bmx2, bmy2;
									bmx  = particle[j].x+particle[j].w/2;
									bmy  = particle[j].y+particle[j].h/2;
									bmx2 = particle[i].x+particle[i].w/2;
									bmy2 = particle[i].y+particle[i].h/2;

									// Get midpoint between player 1 and player 2
									int midX = (bmx + bmx2) /2;
									int midY = (bmy + bmy2) /2;
									SpawnExplosion(particle, midX, midY, {255, 255,255} );
									Remove(particle, i);
								}
							}
						}
					}
				}
			}*/
		}
	}
}


typedef struct SDL_RectM
{
    float x, y;
    float w, h;
} SDL_RectM;

void ContinueGame(Player &p1) {

	// Before game play scene, start game
	if (gameScene == 0)
	{
		// Set gamemode to play game
		gameScene = 1;

		LoadHighScore();

		// Play SFX
		Mix_PlayChannel(-1, sPongScore, false);

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
		SaveHighScore();

		LoadHighScore();

		// Play SFX
		Mix_PlayChannel(-1, sPlayerHurt, false);

		// Set gamemode to play game
		gameScene = 1;

		// Reset score
		score = 0;

		// Reset protection
		//protection = true;
		//protectionTimer = 0;
	}
}

int main(int argc, char *argv[]) {

	// Make random actually random
	srand(time(0));

	// Initialize SDL
	initSDL();

	// Load media
	loadMedia();

	// Load high score
	LoadHighScore();

	// Game loop
	bool quit = false;

	// Particles
	Particle part;
	Particle particles[1000];
	part.init(particles);
	part.load(gRenderer);

	// Enemy
	Enemy enemy[enemyMax];
	initEnemy(enemy);

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
	float enemySpawnTimer = 0.0;
	float enemySpawnRate = 0.45;

	// Player Variables
	const float playerFrameRate = 15;
	float playerFrameTimer = 0;
	int playerFrame = 0;

	// World
	SDL_Rect floor = {0, mHeight-28, mWidth, 28};
	SDL_RectM rSunClouds = {0, 0, 640, 360};
	SDL_RectM rCity3 = {0, 0, 640, 360};
	SDL_RectM rCity2 = {0, 0, 640, 360};
	SDL_RectM rCity1 = {0, 0, 640, 360};

	// Call before loop
	enemyCount = 0;
	gameScene = 0;
	paused = false;
	quit = false;
	fireTimer = 0;
	fireRate = 15;
	playerFrame = 0;

	Mix_FadeInMusic(sMusic, -1, 2000);

	// Game loop
	while (!quit) {

		// reset Player's positions
		//p1.setX(64);

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
			else{

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
							ContinueGame(p1);
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
		}

		// If game paused
		if (!paused == 1) {
			if (gameScene == 1) {

				// particle test for particles
				for (int i = 0; i < part.max; i++) {
					if (particles[i].alive) {
						// Star particles
						if (particles[i].type == 4) {
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
					int randPosition = rand() % int(p1.getHeight());
					int randSetBack = rand() % 3 + 1;
					float newX = p1.getX() + p1.getWidth() - 16;
					float newY = p1.getY() + p1.getHeight() / 2 + 32 - 6;

					float centerX = p1.getCenterX();
					float centerY = p1.getCenterY();

					///////////////////////////////////////////////////////////////////////
					//-------------------------------------------------------------------//
					//---------------------- Handle Basic Shooting ----------------------//
					/***** Set Turret Position *****/
					float turret1w = 128;
					float turret1h = 64;
					float frigateAngle = p1.angle;
					float radians = (3.1415926536 / 180) * (p1.angle);
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
					radians = (3.1415926536 / 180) * (p1.angle);
					barrelW = (60 * cos(radians)) - (32 * sin(radians));// add this to center of zombie (this will give us the guns barrel position)
					barrelH = (60 * sin(radians)) + (32 * cos(radians));
					barrelX = turret1x + turret1w / 2 - particleW / 2 + barrelW;
					barrelY = turret1y + turret1h / 2 - particleH / 2 + barrelH;
					/***** Get turrets nose angle *****/

					float newAngle = atan2(my - barrelY, mx - barrelX);
					newAngle = newAngle * 180 / M_PI;

					part.spawnParticleAngle(particles, "slow", 4, barrelX, barrelY,
							particleW, particleH, newAngle, 11, 0.0,
							{ 200, 200, 100 }, 1, 0, 0, 255, 0, 60, 0, false, 0.11,
							false, 0.11, false, 0.0, white, 0.0, 0.0, 0.0, false,
							0.0, 0.0, false, 0, 0.0);
					// play sfx
					Mix_PlayChannel(-1, sShoot, false);

					/*int randPosition = rand() % int(p1.getHeight());
					 int randSetBack = rand() % 3 + 1;
					 //p1.setX( p1.getX() - randSetBack );
					 //float newX = p1.getX() + p1.getWidth() + 4;
					 //float newY = p1.getY() + randPosition - 2;
					 float newX = p1.getX() + p1.getWidth();
					 float newY = p1.getY() + p1.getHeight()/2 - 4/2;
					 part.spawnParticleAngle(particles, "slow", 4,
					 newX, newY,
					 12, 12,
					 // 0.0, randDouble(9.9, 11.9),
					 0.0, 4,
					 0.0,
					 {200,200,200}, 1,
					 1, 1,
					 255, 0,
					 60, 0,
					 false, 0.11,
					 false, 0.11,
					 false, 0.0, white,
					 0.0, 0.0,
					 0.0,
					 false, 0.0, 0.0,
					 false, 0,
					 0.05);*/
					// spawn explosion
					//.SpawnExplosion(particles, newX, newY, {200,200,200});
					/*if (p1.shootTimer > 0) {
					 p1.shootTimer -= 1;
					 // shoot
					 for (int i=0; i <= 60; i+= 20) {
					 if (i == int(p1.shootTimer)) {
					 int randPosition = rand() % p1.getHeight();
					 int randSetBack = rand() % 3 + 1;
					 p1.setX( p1.getX() - randSetBack );
					 float newX = p1.getX() + p1.getWidth() + 4;
					 float newY = p1.getY() + randPosition - 2;
					 part.spawnParticleAngle(particles, "slow", 4,
					 newX, newY,
					 4, 4,
					 0.0, randDouble(9.9, 11.9),
					 0.0,
					 {200,200,100}, 1,
					 1, 1,
					 255, 0,
					 rand() % 10 + 50, 3,
					 false, 0.11,
					 false, 0.11);
					 // play sfx
					 Mix_PlayChannel(-1, sShoot, false);
					 // spawn explosion
					 part.SpawnExplosion(particles, newX, newY, {200,200,100});
					 }
					 }
					 }else{
					 p1.shoot = false;
					 }*/
				}

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////// Manual Updates ////////////////////////////////////////////

				// Particle collision with Player
				if (!p1.flash) {
					for (int i = 0; i < 1000; i++) {
						if (particles[i].alive) {
							if (particles[i].type == 3) {
								// Player check
								if (checkCollision(particles[i].x, particles[i].y,
										particles[i].w, particles[i].h, p1.getX(),
										p1.getY(), p1.getWidth(), p1.getHeight())) {

									// Hurt player
									p1.Hurt(10);

									// Flash player
									p1.flash = true;

									// remove particle
									part.Remove(particles, i);

									// spawn explosion
									part.SpawnExplosion(particles,
											particles[i].x + particles[i].w / 2,
											particles[i].y + particles[i].h / 2, { 200,
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
					if (particles[i].alive) {
						if (particles[i].type == 4) {
							// Enemy check
							for (int j = 0; j < enemyMax; j++) {
								if (enemy[j].alive) {

									// Splash Damage Collision Check

									// Camera target distance
									/*float bmx, bmy, bmx2, bmy2;
									 bmx  = enemy[j].x+enemy[j].w/2;
									 bmy  = enemy[j].y+enemy[j].h/2;
									 bmx2  = particles[i].x+particles[i].w/2;
									 bmy2  = particles[i].y+particles[i].h/2;
									 float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
									 (bmy - bmy2) * (bmy - bmy2));
									 if (distance < 64) {
									 // Remove enemy
									 enemy[j].alive = false;
									 enemyCount--;
									 // remove particle
									 // spawn explosion
									 // play sound effect
									 part.Remove(particles, i);
									 part.SpawnExplosion(particles, particles[i].x+particles[i].w/2, particles[i].y+particles[i].h/2, {200,200,200});
									 Mix_PlayChannel(-1, sPongScore, 0);
									 }*/

									// Camera target
									/*float vX, vY;
									 if (distance > 0.5){
									 vX 	= 2 * (10*distance/600) * (bmx - bmx2) / distance;
									 vY 	= 2 * (10*distance/600) * (bmy - bmy2) / distance;
									 }*/

									// ON-HIT Collision Check
									if (checkCollision(particles[i].x,
											particles[i].y, particles[i].w,
											particles[i].h, enemy[j].x, enemy[j].y,
											enemy[j].w, enemy[j].h)) {

										// Flash enemy
										enemy[j].flash = true;

										// Remove enemy
										enemy[j].health -= 25;

										// remove particle
										part.Remove(particles, i);

										// spawn explosion
										part.SpawnExplosion(particles,
												particles[i].x + particles[i].w / 2,
												particles[i].y + particles[i].h / 2,
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
			updateEnemy(enemy);

			// Spawn Enemies
			enemySpawnTimer += 0.22;
			if (enemySpawnTimer > 60) {
				enemySpawnTimer = 0;

				// Spawn a random enemy a few pixels to the right of the screen
				spawnEnemy(enemy, 1280 + rand() % 100, ground - 64 - 32 + 5, 64,
						64, rand() % 3);
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
								newY, 11, 11, randDouble(200, 225), 9, 0.0, {
										200, 200, 200 }, 1, 1, 1, 255, 0, 60, 0,
								false, 0.11, false, 0.11, false, 0.0, white,
								0.0, 0.0, 0.0, false, 0.0, 0.0, false, 0,
								0.004);
						// play sfx
						Mix_PlayChannel(-1, sShoot, false);
					}
				}
			}
		}		// end !paused

		// Move backgrounds
		rSunClouds.x -= 0.05;
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
		ParticleUpdate(part, particles, 0, 0, mWidth, mHeight, 0, 0);
		//part.Update(particles, 0, 0, mWidth, mHeight, 0, 0);
		part.updateStarParticles(particles, 0, 0, mWidth, mHeight);
		part.updateBulletParticles(particles, 0, 0, mWidth, mHeight);

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Layer 0, Orange Background
			gOrangeBG.render(gRenderer, 0, 0, screenWidth, screenHeight);

			// Layer 1, Sun and Clouds
			gSunClouds.render(gRenderer, rSunClouds.x, rSunClouds.y, screenWidth, screenHeight);

			// Layer 2, City
			gCity3.render(gRenderer, rCity3.x+1280, rCity3.y, screenWidth, screenHeight);
			gCity3.render(gRenderer, rCity3.x, rCity3.y, screenWidth, screenHeight);

			// Layer 3, City
			gCity2.render(gRenderer, rCity2.x, rCity2.y, screenWidth, screenHeight);
			gCity2.render(gRenderer, rCity2.x+1280, rCity2.y, screenWidth, screenHeight);

			// Layer 4, City
			gCity1.render(gRenderer, rCity1.x, rCity1.y, screenWidth, screenHeight);
			gCity1.render(gRenderer, rCity1.x+1280, rCity1.y, screenWidth, screenHeight);

			// Render particles
			part.Render(gRenderer, particles, 0, 0);

			// Render floor
			SDL_SetRenderDrawColor(gRenderer, 70, 70, 70, 255);
			SDL_RenderFillRect(gRenderer, &floor);

			// Render floor
			SDL_Rect playerPower = {p1.getX()+p1.getWidth(), p1.getY()+p1.getHeight()/2-1, p1.xPower*6, 2};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &playerPower);

			////// Render game objects //////

			// Render Player 1
			if (p1.alive) {
				gCopter.setAlpha(p1.alpha);
				gCopter.render(gRenderer, p1.getX(), p1.getY(), p1.getWidth(), p1.getHeight(), &rCopter[playerFrame], p1.angle);
				p1.render(gRenderer);
			}

			//p1.render(gRenderer);

			// Render enemy
			renderEnemy(enemy, 0, 0);

			////// Render Text //////

			////// Game Scenes /////

			std::stringstream tempss;
			// Before game start scene
			if (gameScene == 0) {

				// Render text: To start game
				tempss.str(std::string());
				tempss << "Press Space to Start.";
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), white, viga);
				gText.render(gRenderer, screenWidth/2 - gText.getWidth()/2, screenHeight * 0.90 - gText.getHeight(),
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
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), black, viga);
				gText.render(gRenderer, screenWidth/2 - gText.getWidth()/2,
						screenHeight * 0.90 - gText.getHeight()-22,
							 gText.getWidth(), gText.getHeight());

				tempss.str(std::string());
				tempss << "Press Space to Start.";
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), black, viga);
				gText.render(gRenderer, screenWidth/2 - gText.getWidth()/2,
						screenHeight * 0.90 - gText.getHeight(),
							 gText.getWidth(), gText.getHeight());
			}

			// Winning scene
			else if (gameScene == 2) {
				tempss.str(std::string());
				tempss << "You win something!";
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), black, viga);
				gText.render(gRenderer, 640/2 - gText.getWidth()/2,
						 360 * 0.90 - gText.getHeight()-22,
							 gText.getWidth(), gText.getHeight());

				tempss.str(std::string());
				tempss << "Press Space to Start again.";
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), black, viga);
				gText.render(gRenderer, 640/2 - gText.getWidth()/2,
						 360 * 0.90 - gText.getHeight(),
							 gText.getWidth(), gText.getHeight());
			}

			// Game paused
			if (paused) {

				tempss.str(std::string());
				gText.setAlpha(255);
				gText.loadFromRenderedText(gRenderer, "Paused", {255,255,255}, viga);
				gText.render(gRenderer, 640/2-gText.getWidth()/2, 360 - gText.getHeight() - 20,
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

			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), white, viga);
			gText.render(gRenderer, screenWidth - gText.getWidth() - 10, 4, gText.getWidth(), gText.getHeight());

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
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), white, viga);
			gText.render(gRenderer, screenWidth - gText.getWidth() - 10, 28, gText.getWidth(), gText.getHeight());

			//////// Debug ///////

			// Render text: To start game
			/*tempss.str(std::string());
			tempss << "gameScene: " << gameScene;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), white, viga);
			gText.render(gRenderer, 0, 0,
						 gText.getWidth(), gText.getHeight());*/


		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}

	// Free resources
	part.free();
	freeSDL();

	return 0;
}
