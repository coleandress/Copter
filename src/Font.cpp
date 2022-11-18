#include "Font.h"

Font::Font(Message& msg)
	: mMsg{ msg }
{
	TTF_Init();
	Util::loadFontFromFile(mMsg, &mFonts[VIGA], "resource/fonts/Viga-Regular.ttf", 24);
}

Font::~Font()
{
	mTexture.free();
	TTF_Quit();
}

LTexture& Font::getTexture()
{
	return mTexture;
}

TTF_Font* Font::getFont(FONTS name)
{
	return mFonts[name];
}
