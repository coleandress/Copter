#pragma once
#include <SDL2/SDL_mixer.h>

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
	Sound();
	~Sound();
	void playMusic();
	void playSound(SoundName name);

private:
	Mix_Music* mMusic{ nullptr };
	Mix_Chunk* mSounds[TOTAL_SOUNDS]{nullptr};
};

