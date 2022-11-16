#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL.h>

#include "LTexture.h"

class Enemy
{
public:
	//static void initEnemy(Enemy enemy[], int& enemyCount, const int& enemyMax);
	//static void spawnEnemy(Enemy enemy[], float x, float y, float w, float h, int type, int& enemyCount, const int& enemyMax);
	//static void updateEnemy(Enemy enemy[], int& enemyCount, const int& enemyMax, int& score);
	static void renderEnemy(Enemy enemy[], float camx, float camy, SDL_Renderer& gRenderer, LTexture& gTanks, SDL_Rect rTanks[], const int& enemyMax);

//private:
public:
	float x, y;
	float w, h;
	/*
	 * 0: Trooper
	 * 1: Medium Trooper
	 * 2: Heavy Trooper
	 */
	int type;
	int health;
	float shootRate;
	float shootTimer;
	Uint8 alpha;
	int flashTimer;
	bool flash;
	bool alive;
};

