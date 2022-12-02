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
#include "ParticleManager.h"
#include "Player.h"
#include "Sound.h"
#include "Font.h"

#include "Message.h"
#include "Background.h"
#include "EnemyManager.h"

void setup(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer, int& previousHighScore);
void initSDL(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer);
void LoadHighScore(int& previousHighScore);
void handleInput(Player& p1, int& gameScene, int& previousHighScore, int& score, bool& paused, SDL_Event& events, Sound& sound);
void ContinueGame(Player& p1, int& gameScene, int& previousHighScore, int& score, Sound& sound);
void SaveHighScore(const int& score);
int count_digit(int number);
void freeSDL(LWindow& mWindow);


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
	ParticleManager part(msg, &renderer, Util::MAX_PARTICLES);

	Sound sound{ msg };
	EnemyManager enemyManager{ msg, window, &renderer, part, sound };
	int gameScene{ 0 };
	//int highscore{ -1 };
	int score{ 0 };
	Font font{ msg };

	msg.log("In main(), calling: setup ...");

	bool paused = false;

	// Create pointer for events
	SDL_Event events;

	// Game loop
	bool quit = false;

	// Create Player 1
	Player p1{ msg, &renderer, part, sound };
	//p1.init();

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
	//const float playerFrameRate = 15;
	//float playerFrameTimer = 0;
	//int playerFrame = 0;

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
		if (!paused)
		{
			if (gameScene)
			{
				// TODO: Mess with this later
				
				// particle test for particles
				for (int i = 0; i < part.getParticles().size(); i++)
				{
					if (part.getParticles()[i].mAlive)
					{
						// Star particles
						if (part.getParticles()[i].mType == 4)
						{
							// pong particle effects
							fireTimer += 60;
							if (fireTimer > 60)
							{
								fireTimer = 0;
								// spawn explosion
								// This is a neat effect, but it makes firing rapidly not function - CA 2022-11-14
								//part.SpawnExplosion(part.getParticles()[i].mX+part.getParticles()[i].mW/2, part.getParticles()[i].mY+part.getParticles()[i].mH/2, {244,144,20});
							}
						}
					}
				}

				// Update Players
				p1.update(mx, my);

				// Particle collision with Enemies
				for (int i = 0; i < part.getParticles().size(); i++)
				{
					if (part.getParticles()[i].mAlive)
					{
						if (part.getParticles()[i].mType == 4)
						{
							// Enemy check
							for (int j = 0; j < enemyManager.ENEMY_MAX; j++)
							{
								if (enemyManager.getEnemies()[j].alive)
								{
									// ON-HIT Collision Check
									if (Util::checkCollision(part.getParticles()[i].mX,
										part.getParticles()[i].mY, part.getParticles()[i].mW,
										part.getParticles()[i].mH, enemyManager.getEnemies()[j].x, enemyManager.getEnemies()[j].y,
										enemyManager.getEnemies()[j].w, enemyManager.getEnemies()[j].h))
									{
										// Flash enemy
										enemyManager.getEnemies()[j].flash = true;

										// Remove enemy
										enemyManager.getEnemies()[j].health -= 25;

										// remove particle
										part.Remove(i);

										// spawn explosion
										part.SpawnExplosion(
											part.getParticles()[i].mX + part.getParticles()[i].mW / 2,
											part.getParticles()[i].mY + part.getParticles()[i].mH / 2,
											{ 200, 200, 200 });

										// Play SFX
										sound.playSound(PONG_SCORE);
									}
								}
							}
						}
					}
				}
				enemyManager.updateEnemies(score);

				background.moveBackgrounds();

				// Update Particles
				//ParticleUpdate(part, particles, 0, 0, mWidth, mHeight, 0, 0);
				ParticleManager::ParticleUpdate(part, 0, 0, window.getWidth(), window.getHeight(), 0, 0, window, sound);
				//part.Update(particles, 0, 0, mWidth, mHeight, 0, 0);
				part.updateStarParticles(0, 0, window.getWidth(), window.getHeight());
				part.updateBulletParticles(0, 0, window.getWidth(), window.getHeight());

				// Clear render screen
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);

				background.renderBackgrounds();

				// Render particles
				part.Render(0, 0);

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
					p1.getTexture().setAlpha(p1.alpha);
					p1.getTexture().render(renderer, (int)p1.getX(), (int)p1.getY(), (int)p1.getWidth(), (int)p1.getHeight(), &p1.getRects()[p1.getPlayerFrame()], p1.angle);
					p1.render(renderer);
				}

				enemyManager.renderEnemies();

				if (!p1.alive)
					gameScene = 2;
			}	// end gameScene 1


			////// Render Text //////
			////// Game Scenes /////
			std::stringstream tempss;
			// Before game start scene
			if (gameScene == 0)
			{
				background.moveBackgrounds();
				background.renderBackgrounds();

				// TODO: duplicate code that needs to be extracted
				p1.getTexture().setAlpha(p1.alpha);
				p1.getTexture().render(renderer, (int)p1.getX(), (int)p1.getY(), (int)p1.getWidth(), (int)p1.getHeight(), &p1.getRects()[p1.getPlayerFrame()], p1.angle);
				p1.render(renderer);


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
		} // end !paused

		// fps
		frame++;
		if ((cap == true) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND))
			SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
	}

	// Free resources
	freeSDL(window);

	return 0;
}

void setup(Message& msg, LWindow& mWindow, SDL_Renderer** gRenderer, int& previousHighScore)
{
	// Make random actually random
	srand((unsigned int)time(NULL));

	// Initialize SDL
	initSDL(msg, mWindow, gRenderer);

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

int count_digit(int number)
{
	return int(log10(number) + 1);
}

void freeSDL(LWindow& mWindow)
{
	mWindow.free();

	//Quit SDL subsystems
	SDL_Quit();
}
