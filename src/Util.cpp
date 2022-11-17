#include "Util.h"

float Util::randFloat(float fMin, float fMax)
{
	float f = (float)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

const SDL_Color Util::BLACK{ 0, 0, 0, 255 };
const SDL_Color Util::BLUE{ 0, 0, 255, 255 };
const SDL_Color Util::GREEN{ 0, 255, 0, 255 };
const SDL_Color Util::RED{ 255, 0, 0, 255 };
const SDL_Color Util::WHITE{ 255, 255, 255, 255 };
const SDL_Color Util::ORANGE{ 180, 90, 20, 255 };