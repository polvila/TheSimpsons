#ifndef __JSONMANAGER_H__
#define __JSONMANAGER_H__

#include "SDL/include/SDL.h"
#include "parson.h"
#include <map>
#include "Animation.h"
#include "Module.h"

enum SimpsonsTexture
{
	STAGE1,
	HOMER
};

enum SimpsonsSprite
{
	BACKGROUND,
	FOREGROUND,
	DOOR1,
	DOOR2,
	DOOR3,
	FENCE,
	CAR,
	WHEEL,
	STREETLIGHT,
	PARKING_DOOR,
	CLOUD1,
	CLOUD2,
	CLOUD3,
	CLOUD4,
	CLOUD5,
	CLOUD6,
	TREE1,
	TREE2,
	HOMER_IDLE,
	COUNT
};

enum SimpsonsAnimation
{
	LEAVES,
	HOMER_YAWN,
	HOMER_WALK,
	HOMER_WALK_UP,
	HOMER_ATTACK,
};

class ModuleJsonManager : public Module
{
public:
	ModuleJsonManager(bool start_enabled = true);
	~ModuleJsonManager();

	bool Init() override;
	bool CleanUp() override;

	const char* GetSDL_TextureOf(SimpsonsTexture texture);
	SDL_Rect* GetSDL_RectOf(SimpsonsSprite sprite);
	Animation* GetAnimationOf(SimpsonsAnimation animation);


private:

	void FillTextureMap();
	void FillSpritesMap();
	void FillAnimationsMap();

	static void FillSDL_RectFrom(SDL_Rect* sdlRect, const JSON_Object* jsonObject, const char* name);
	static void FillAnimationFrom(Animation* animation, const JSON_Object* jsonObject, const char* name);

	std::map<SimpsonsTexture, const char*> texturePathsMap;
	std::map<SimpsonsSprite, SDL_Rect*> spritesMap;
	std::map<SimpsonsAnimation, Animation*> animationsMap;

	JSON_Object* assetsInfo = nullptr;
	JSON_Object* textures = nullptr;
	JSON_Object* stage1 = nullptr;
	JSON_Object* homer = nullptr;
	JSON_Object* stage1Elements = nullptr;
	JSON_Object* homerElements = nullptr;

	const char* inputFile = "TheSimpsons/assetsInfo.json";
};


#endif // __JSONMANAGER_H__