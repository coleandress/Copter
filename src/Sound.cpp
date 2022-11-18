#include "Sound.h"
#include <iostream>

Sound::Sound(Message& message)
	: mMsg(message)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	Util::loadSoundFromFile(mMsg, &mMusic, "resource/sounds/music.mp3");
	Util::loadSoundFromFile(mMsg, &mSounds[PONG_PLAYER], "resource/sounds/pong-player.wav");
	Util::loadSoundFromFile(mMsg, &mSounds[PONG_SCREEN], "resource/sounds/pong-screen.wav");
	Util::loadSoundFromFile(mMsg, &mSounds[PONG_SCORE], "resource/sounds/pong-score.wav");
	Util::loadSoundFromFile(mMsg, &mSounds[PLAYER_HURT], "resource/sounds/player-hurt.wav");
	Util::loadSoundFromFile(mMsg, &mSounds[SHOOT], "resource/sounds/player-shoot.wav");
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
