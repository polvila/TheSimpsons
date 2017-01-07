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
	HOMER,
	NPC
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
	LADY_DOWN_IDLE,
	HOWIE_JUMP,
	COUNT
};

enum SimpsonsAnimation
{
	LEAVES,
	HOMER_IDLE,
	HOMER_YAWN,
	HOMER_WALK,
	HOMER_WALK_UP,
	HOMER_ATTACK1,
	HOMER_ATTACK2,
	HOMER_JUMP_UP,
	HOMER_JUMP_DOWN,
	LADY_TOP,
	LADY_DOWN_WALK,
	BIRD_IDLE,
	BIRD_JUMP,
	BIRD_CLEAN,
	BIRD_FLY,
	HOWIE_FRONT_WALK,
	HOWIE_WALK,
	HOWIE_IDLE,
	MARTIN_IDLE,
	MARTIN_RUN,
	SKINNER_IDLE1,
	SKINNER_WALK,
	SKINNER_IDLE2,
	SKINNER_RUN,
	HAMSTER
};

enum SimpsonsAudio
{
	STAGE1_AUDIO
};

class ModuleJsonManager : public Module
{
public:
	ModuleJsonManager(bool start_enabled = true);
	~ModuleJsonManager();

	bool Init() override;
	bool CleanUp() override;

	char* GetTexturePathOf(SimpsonsTexture texture);
	SDL_Color* GetTransparentPixelColor(SimpsonsTexture texture);
	SDL_Rect* GetSDL_RectOf(SimpsonsSprite sprite);
	Animation* GetAnimationOf(SimpsonsAnimation animation);
	char* GetAudioPathOf(SimpsonsAudio audio);

private:

	void FillTextureMap();
	void FillSpritesMap();
	void FillAnimationsMap();
	void FillTransparentPixelColorsMap();
	void FillAudioMap();

	static void FillSDL_RectFrom(SDL_Rect* sdlRect, const JSON_Object* jsonObject, const char* name);
	static void FillAnimationFrom(Animation* animation, const JSON_Object* jsonObject, const char* name);
	static void FillTransparentPixelColorFrom(SDL_Color* color, const JSON_Object* jsonObject);

	static void FillSDL_RectWithFrameCoordinates(SDL_Rect* sdlRect, JSON_Object* frameCoordinates);
	static void FillNextSDL_Rects(SDL_Rect* firstSdlRect, int numFrames, Animation* animation);

	std::map<SimpsonsTexture, char*> texturePathsMap;
	std::map<SimpsonsTexture, SDL_Color*> transparentPixelColorsMap;
	std::map<SimpsonsSprite, SDL_Rect*> spritesMap;
	std::map<SimpsonsAnimation, Animation*> animationsMap;
	std::map<SimpsonsAudio, char*> audioPathsMap;

	JSON_Object* assetsInfo = nullptr;
	JSON_Object* textures = nullptr;
	JSON_Object* audio = nullptr;
	JSON_Object* stage1 = nullptr;
	JSON_Object* homer = nullptr;
	JSON_Object* npc = nullptr;

	JSON_Object* stage1Elements = nullptr;
	JSON_Object* homerElements = nullptr;
	JSON_Object* npcElements = nullptr;

	const char* inputFile = "TheSimpsons/assetsInfo2.json";
};


#endif // __JSONMANAGER_H__