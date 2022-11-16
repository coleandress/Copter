#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL.h>

#include "LTexture.h"

class Enemy
{
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

