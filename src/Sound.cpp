#include "Sound.h"
#include <iostream>

Sound::Sound()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	mMusic = Mix_LoadMUS("resource/sounds/music.mp3");
	//mMusic = Mix_LoadMUS("C:\\dev\\Copter\\resource\\sounds\\pong_player.wav");
	if (!mMusic)
		std::cout << SDL_GetError();
	mSounds[PONG_PLAYER] = Mix_LoadWAV("resource/sounds/pong-player.wav");
	if (!mSounds[PONG_PLAYER])
		std::cout << SDL_GetError();
	mSounds[PONG_SCREEN] = Mix_LoadWAV("resource/sounds/pong-screen.wav");
	mSounds[PONG_SCORE] = Mix_LoadWAV("resource/sounds/pong-score.wav");
	mSounds[PLAYER_HURT] = Mix_LoadWAV("resource/sounds/player-hurt.wav");
	mSounds[SHOOT] = Mix_LoadWAV("resource/sounds/player-shoot.wav");
}

Sound::~Sound()
{
	Mix_FreeMusic(mMusic);
	Mix_FreeChunk(mSounds[PONG_PLAYER]);
	Mix_FreeChunk(mSounds[PONG_SCREEN]);
	Mix_FreeChunk(mSounds[PONG_SCORE]);
	Mix_FreeChunk(mSounds[PLAYER_HURT]);
	Mix_FreeChunk(mSounds[SHOOT]);
	Mix_Quit();
}

void Sound::playMusic()
{
	Mix_FadeInMusic(mMusic, -1, 2000);
}

void Sound::playSound(SoundName name)
{
	Mix_PlayChannel(-1, mSounds[name], false);
}
