#include "Background.h"

Background::Background(Message& msg, LWindow& window, SDL_Renderer** renderer)
	: mMsg{ msg },
	  mWindow{ window },
	  mRenderer{ renderer },
	  mFloorRect{ 0, mWindow.getHeight() - 28, mWindow.getWidth(), 28 },
	  mSunCloudsRect{ 0, 0, 640, 360 },
	  mCity3Rect{ 0, 0, 640, 360 },
	  mCity2Rect{ 0, 0, 640, 360 },
	  mCity1Rect{ 0, 0, 640, 360 }
{
	loadBackgrounds();
}

Background::~Background()
{
	mOrangeBG.free();
	mSunClouds.free();
	mCity3.free();
	mCity2.free();
	mCity1.free();
}

void Background::loadBackgrounds()
{
	Util::loadTextureFromFile(mMsg, mOrangeBG, mRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_08_1920 x 1080.png");
	Util::loadTextureFromFile(mMsg, mSunClouds, mRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_07_1920 x 1080.png");
	Util::loadTextureFromFile(mMsg, mCity3, mRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_06_1920 x 1080.png");
	Util::loadTextureFromFile(mMsg, mCity2, mRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_05_1920 x 1080.png");
	Util::loadTextureFromFile(mMsg, mCity1, mRenderer, "resource/gfx/Backgrounds - FREE/Background 07/PARALLAX/layer_04_1920 x 1080.png");
}

void Background::renderBackgrounds()
{
	// Layer 0, Orange Background
	mOrangeBG.render(*mRenderer, 0, 0, mWindow.getWidth(), mWindow.getHeight());

	// Layer 1, Sun and Clouds
	mSunClouds.render(*mRenderer, (int)mSunCloudsRect.x, (int)mSunCloudsRect.y, mWindow.getWidth(), mWindow.getHeight());

	// Layer 2, City
	mCity3.render(*mRenderer, (int)mCity3Rect.x + 1280, (int)mCity3Rect.y, mWindow.getWidth(), mWindow.getHeight());
	mCity3.render(*mRenderer, (int)mCity3Rect.x, (int)mCity3Rect.y, mWindow.getWidth(), mWindow.getHeight());

	// Layer 3, City
	mCity2.render(*mRenderer, (int)mCity2Rect.x, (int)mCity2Rect.y, mWindow.getWidth(), mWindow.getHeight());
	mCity2.render(*mRenderer, (int)mCity2Rect.x + 1280, (int)mCity2Rect.y, mWindow.getWidth(), mWindow.getHeight());

	// Layer 4, City
	mCity1.render(*mRenderer, (int)mCity1Rect.x, (int)mCity1Rect.y, mWindow.getWidth(), mWindow.getHeight());
	mCity1.render(*mRenderer, (int)mCity1Rect.x + 1280, (int)mCity1Rect.y, mWindow.getWidth(), mWindow.getHeight());
}

void Background::moveBackgrounds()
{
	mSunCloudsRect.x -= 0.05f;
	mCity3Rect.x -= 2;
	mCity2Rect.x -= 3;
	mCity1Rect.x -= 4;

	if (mSunCloudsRect.x < -1280)
	{
		mSunCloudsRect.x = 0;
	}
	if (mCity3Rect.x < -1280)
	{
		mCity3Rect.x = 0;
	}

	if (mCity2Rect.x < -1280)
	{
		mCity2Rect.x = 0;
	}

	if (mCity1Rect.x < -1280)
	{
		mCity1Rect.x = 0;
	}
}

SDL_Rect& Background::getFloorRect()
{
	return mFloorRect;
}
