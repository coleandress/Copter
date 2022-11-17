#include "Util.h"

float Util::randFloat(float fMin, float fMax)
{
	float f = (float)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

bool Util::checkCollision(float x, float y, float w, float h, float x2, float y2, float w2, float h2)
{
	if (x + w > x2 && x < x2 + w2 && y + h > y2 && y < y2 + h2) {
		return true;
	}
	else {
		return false;
	}
}

const SDL_Color Util::BLACK{ 0, 0, 0, 255 };
const SDL_Color Util::BLUE{ 0, 0, 255, 255 };
const SDL_Color Util::GREEN{ 0, 255, 0, 255 };
const SDL_Color Util::RED{ 255, 0, 0, 255 };
const SDL_Color Util::WHITE{ 255, 255, 255, 255 };
const SDL_Color Util::ORANGE{ 180, 90, 20, 255 };