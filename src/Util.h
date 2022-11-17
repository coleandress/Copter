#pragma once

#include <cstdlib>
#include <SDL2/SDL.h>

class Util
{
public:
	static float randFloat(float fMin, float fMax);	// Colors
	static const SDL_Color BLACK;// = { 0, 0, 0, 255 };
	static const SDL_Color BLUE; // { 0, 0, 255, 255 };
	static const SDL_Color GREEN;// = { 0, 255, 0, 255 };
	static const SDL_Color RED;// = { 255, 0, 0, 255 };
	static const SDL_Color WHITE;// = { 255, 255, 255, 255 };
	static const SDL_Color ORANGE;// = { 180, 90, 20, 255 };
};

