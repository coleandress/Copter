#pragma once

#include <cstdlib>
#include <SDL2/SDL.h>

class Util
{
public:
	static float randFloat(float fMin, float fMax);	// Colors
	static const SDL_Color BLACK;
	static const SDL_Color BLUE;
	static const SDL_Color GREEN;
	static const SDL_Color RED;
	static const SDL_Color WHITE;
	static const SDL_Color ORANGE;
};

