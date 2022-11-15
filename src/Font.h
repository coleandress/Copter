#pragma once
#include "LTexture.h"

enum FONTS
{
	VIGA,
	TOTAL_FONTS
};

class Font
{
public:
	Font();
	~Font();
	LTexture& getTexture();
	TTF_Font* getFont(FONTS name);

private:
	LTexture mTexture;
	TTF_Font* mFonts[TOTAL_FONTS]{ nullptr };
};

