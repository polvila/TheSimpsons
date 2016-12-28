#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include <list>
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init() override;
	bool CleanUp() override;

	SDL_Texture* const Load(const char* path, SDL_Color* transparentPixelColor);
	void Unload(SDL_Texture* texture);
	
private:

	bool AssertLoadPngSupport() const;
	void TryToCreateTextureFromSurface(SDL_Texture** texture, SDL_Surface* surface);
	std::list<SDL_Texture*> textures;
};

#endif // __MODULETEXTURES_H__