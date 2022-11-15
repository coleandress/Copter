#include "Font.h"

Font::Font()
{
	TTF_Init();
	mFonts[VIGA] = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 24); //this opens a font style and sets a size
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
