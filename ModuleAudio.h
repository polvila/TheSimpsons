#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <vector>
#include "Module.h"
#include "SDL_mixer/include/SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();
	
	bool Init() override;
	bool CleanUp() override;

	bool AssertInitAudio() const;
	bool AssertLoadOggSupport() const;
	bool AssertInitMixer() const;
	void FadeOutActualMusic(float fade_time) const;
	bool AssertFadeInMusic(const char* path, float fade_time) const;
	bool AssertPlayMusic(const char* path) const;
	bool PlayMusicWithFade(const char* path, float fade_time) const;
	bool TryToPlayMusic(const char* path, float fade_time) const;
	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);
	unsigned int AssertLoadWav(const char* path, Mix_Chunk* chunk);
	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	Mix_Music*	music = nullptr;
	std::vector<Mix_Chunk*>	fx;
};

#endif // __MODULEAUDIO_H__