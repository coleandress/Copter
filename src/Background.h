#pragma once

#include "Message.h"
#include "LWindow.h"
#include "LTexture.h"

class Background
{
public:
	Background(Message& msg, LWindow& window, SDL_Renderer** renderer);
	~Background();
	void loadBackgrounds();
	void renderBackgrounds();
	void moveBackgrounds();
	SDL_Rect& getFloorRect();

private:
	Message& mMsg;
	LWindow& mWindow;
	SDL_Renderer** mRenderer;
	LTexture mOrangeBG;
	LTexture mSunClouds;
	LTexture mCity3;
	LTexture mCity2;
	LTexture mCity1; 
	SDL_Rect mFloorRect;
	SDL_FRect mSunCloudsRect;
	SDL_FRect mCity3Rect;
	SDL_FRect mCity2Rect;
	SDL_FRect mCity1Rect;
};


