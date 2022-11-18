#pragma once

#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Message.h"
#include "LTexture.h"

class Util
{
public:
	static void loadTextureFromFile(Message& msg, LTexture& texture, SDL_Renderer** renderer, const std::string& fileName);
	static void loadSoundFromFile(Message& msg, Mix_Music** music, const std::string& fileName);
	static void loadSoundFromFile(Message& msg, Mix_Chunk** sound, const std::string& fileName);
	static float randFloat(float fMin, float fMax);	// Colors
	static bool checkCollision(float x, float y, float w, float h, float x2, float y2, float w2, float h2);
	static const SDL_Color BLACK;
	static const SDL_Color BLUE;
	static const SDL_Color GREEN;
	static const SDL_Color RED;
	static const SDL_Color WHITE;
	static const SDL_Color ORANGE;
};
