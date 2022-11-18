#pragma once
#include <SDL2/SDL_mixer.h>

#include "Util.h"

enum SoundName
{
	PONG_PLAYER,
	PONG_SCREEN,
	PONG_SCORE,
	PLAYER_HURT,
	SHOOT,
	TOTAL_SOUNDS
};

class Sound
{
public:
	Sound(Message& message);
	~Sound();
	void playMusic();
	void playSound(SoundName name);

private:
	Message& mMsg;
	Mix_Music* mMusic{ nullptr };
	Mix_Chunk* mSounds[TOTAL_SOUNDS]{nullptr};
};

