#include "Util.h"

void Util::loadTextureFromFile(Message& msg, LTexture& texture, SDL_Renderer** renderer, const std::string& fileName)
{
	if (!texture.loadFromFile(renderer, fileName))
		msg.fatalError("Call to 'Util::loadTextureFromFile' failed (" + fileName + ")");
}

void Util::loadSoundFromFile(Message& msg, Mix_Music** music, const std::string& fileName)
{
	*music = Mix_LoadMUS(fileName.c_str());
	if (!*music)
		msg.fatalError("Call to 'Util::loadSoundFromFile' failed (" + fileName + ")");
}

void Util::loadSoundFromFile(Message& msg, Mix_Chunk** sound, const std::string& fileName)
{
	*sound = Mix_LoadWAV(fileName.c_str());
	if (!*sound)
		msg.fatalError("Call to 'Util::loadSoundFromFile' failed (" + fileName + ")");
}

void Util::loadFontFromFile(Message& msg, TTF_Font** font, const std::string& fileName, const int& size)
{
	*font = TTF_OpenFont(fileName.c_str(), size);
	if (!*font)
		msg.fatalError("Call to 'Util::loadFontFromFile' failed (" + fileName + ")");
}

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

const int Util::MAX_PARTICLES{ 1000 };
