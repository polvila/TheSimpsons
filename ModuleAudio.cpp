#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"

#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

using namespace std;

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOG("Loading Audio Mixer");
	SDL_Init(0);

	return AssertInitAudio() && AssertLoadOggSupport() && AssertInitMixer();
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != nullptr)
		Mix_FreeMusic(music);

	for (vector<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); ++it)
		Mix_FreeChunk(*it);

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

bool ModuleAudio::AssertInitAudio() const
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool ModuleAudio::AssertLoadOggSupport() const
{
	int flags = MIX_INIT_OGG;
	if ((Mix_Init(flags) & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		return false;
	}
	return true;
}

bool ModuleAudio::AssertInitMixer() const
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

void ModuleAudio::FadeOutActualMusic(float fade_time) const
{
	if (fade_time > 0.0f)
		Mix_FadeOutMusic(static_cast<int>(fade_time * 1000.0f));
	else
		Mix_HaltMusic();

	// this call blocks until fade out is done
	Mix_FreeMusic(music);
}

bool ModuleAudio::AssertFadeInMusic(const char* path, float fade_time) const
{
	if (Mix_FadeInMusic(music, -1, static_cast<int>(fade_time * 1000.0f)) < 0)
	{
		LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
		return false;
	}
	return true;
}

bool ModuleAudio::AssertPlayMusic(const char* path) const
{
	if (Mix_PlayMusic(music, -1) < 0)
	{
		LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
		return false;
	}
	LOG("Successfully playing %s", path);
	return true;
}

bool ModuleAudio::PlayMusicWithFade(const char* path, float fade_time) const
{
	return (fade_time > 0.0f) ? AssertFadeInMusic(path, fade_time) :
								AssertPlayMusic(path);
}

bool ModuleAudio::TryToPlayMusic(const char* path, float fade_time) const
{
	if (music == nullptr)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		return false;
	}
	return PlayMusicWithFade(path, fade_time);
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	if (music != nullptr)
		FadeOutActualMusic(fade_time);

	music = Mix_LoadMUS(path);
	return TryToPlayMusic(path, fade_time);
}

unsigned int ModuleAudio::AssertLoadWav(const char* path, Mix_Chunk* chunk)
{
	if (chunk == nullptr)
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError())
	else
	{
		fx.push_back(chunk);
		return fx.size() - 1;
	}
	return 0;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	Mix_Chunk* chunk = Mix_LoadWAV(path);
	return AssertLoadWav(path, chunk);
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	if (id < fx.size())
	{
		Mix_PlayChannel(-1, fx[id], repeat);
		return true;
	}
	return false;
}