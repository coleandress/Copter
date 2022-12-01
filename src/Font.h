#pragma once
#include "LTexture.h"
#include "Util.h"

enum FONTS
{
	VIGA,
	TOTAL_FONTS
};

class Font
{
public:
	Font(Message& msg);
	~Font();
	LTexture& getTexture();
	TTF_Font* getFont(FONTS name);

private:
	Message& mMsg;
	LTexture mTexture{};
	TTF_Font* mFonts[TOTAL_FONTS]{ nullptr };
};

