#include "ModuleJsonManager.h"
#include "Globals.h"

ModuleJsonManager::ModuleJsonManager(bool start_enabled) : Module(start_enabled)
{
}

ModuleJsonManager::~ModuleJsonManager()
{
}

bool ModuleJsonManager::Init()
{
	assetsInfo = json_value_get_object(json_parse_file(inputFile));
	textures = json_object_get_object(assetsInfo, "textures");
	audio = json_object_get_object(assetsInfo, "audio");
	colliders = json_object_get_object(assetsInfo, "colliders");
	stage1 = json_object_get_object(textures, "stage1");
	homer = json_object_get_object(textures, "homer");
	npc = json_object_get_object(textures, "npc");

	stage1Elements = json_object_get_object(stage1, "elements");
	homerElements = json_object_get_object(homer, "elements");
	npcElements = json_object_get_object(npc, "elements");

	if(stage1Elements != nullptr && homerElements != nullptr &&
		npcElements != nullptr && audio != nullptr)
	{
		FillTextureMap();
		FillSpritesMap();
		FillAnimationsMap();
		FillTransparentPixelColorsMap();
		FillAudioMap();
		FillCollidersMap();
		return true;
	}
	return false;
}

bool ModuleJsonManager::CleanUp()
{
	for (map<SimpsonsTexture, char*>::iterator it = texturePathsMap.begin(); it != texturePathsMap.end(); ++it) {
		RELEASE(it->second);
	}
	for (map<SimpsonsTexture, SDL_Color*>::iterator it = transparentPixelColorsMap.begin(); it != transparentPixelColorsMap.end(); ++it) {
		RELEASE(it->second);
	}
	for (map<SimpsonsSprite, SDL_Rect*>::iterator it = spritesMap.begin(); it != spritesMap.end(); ++it) {
		RELEASE(it->second);
	}
	for (map<SimpsonsAnimation, Animation*>::iterator it = animationsMap.begin(); it != animationsMap.end(); ++it) {
		RELEASE(it->second);
	}
	for (map<SimpsonsAudio, char*>::iterator it = audioPathsMap.begin(); it != audioPathsMap.end(); ++it) {
		RELEASE(it->second);
	}
	for (map<SimpsonsCollider, pair<int, int>*>::iterator it = collidersMap.begin(); it != collidersMap.end(); ++it) {
		RELEASE(it->second);
	}

	return true;
}

char* ModuleJsonManager::GetTexturePathOf(SimpsonsTexture texture)
{
	return texturePathsMap[texture];
}

SDL_Color* ModuleJsonManager::GetTransparentPixelColor(SimpsonsTexture texture)
{
	return transparentPixelColorsMap[texture];
}

SDL_Rect* ModuleJsonManager::GetSDL_RectOf(SimpsonsSprite sprite)
{
	return spritesMap[sprite];
}

Animation* ModuleJsonManager::GetAnimationOf(SimpsonsAnimation animation)
{
	return animationsMap[animation];
}

char* ModuleJsonManager::GetAudioPathOf(SimpsonsAudio audio)
{
	return audioPathsMap[audio];
}

pair<int, int>* ModuleJsonManager::GetColliderSizeOf(SimpsonsCollider collider)
{
	return collidersMap[collider];
}

void ModuleJsonManager::FillTextureMap()
{
	texturePathsMap[STAGE1] = const_cast<char*>(json_object_get_string(stage1, "path"));
	texturePathsMap[HOMER] = const_cast<char*>(json_object_get_string(homer, "path"));
	texturePathsMap[NPC] = const_cast<char*>(json_object_get_string(npc, "path"));
}

void ModuleJsonManager::FillSpritesMap()
{
	FillSDL_RectFrom(spritesMap[BACKGROUND] = new SDL_Rect(), stage1Elements, "background");
	FillSDL_RectFrom(spritesMap[FOREGROUND] = new SDL_Rect(), stage1Elements, "foreground");
	FillSDL_RectFrom(spritesMap[DOOR1] = new SDL_Rect(), stage1Elements, "door1");
	FillSDL_RectFrom(spritesMap[DOOR2] = new SDL_Rect(), stage1Elements, "door2");
	FillSDL_RectFrom(spritesMap[DOOR3] = new SDL_Rect(), stage1Elements, "door3");
	FillSDL_RectFrom(spritesMap[FENCE] = new SDL_Rect(), stage1Elements, "fence");
	FillSDL_RectFrom(spritesMap[CAR] = new SDL_Rect(), stage1Elements, "car");
	FillSDL_RectFrom(spritesMap[WHEEL] = new SDL_Rect(), stage1Elements, "wheel");
	FillSDL_RectFrom(spritesMap[STREETLIGHT] = new SDL_Rect(), stage1Elements, "streetlight");
	FillSDL_RectFrom(spritesMap[PARKING_DOOR] = new SDL_Rect(), stage1Elements, "parkingDoor");
	FillSDL_RectFrom(spritesMap[CLOUD1] = new SDL_Rect(), stage1Elements, "cloud1");
	FillSDL_RectFrom(spritesMap[CLOUD2] = new SDL_Rect(), stage1Elements, "cloud2");
	FillSDL_RectFrom(spritesMap[CLOUD3] = new SDL_Rect(), stage1Elements, "cloud3");
	FillSDL_RectFrom(spritesMap[CLOUD4] = new SDL_Rect(), stage1Elements, "cloud4");
	FillSDL_RectFrom(spritesMap[CLOUD5] = new SDL_Rect(), stage1Elements, "cloud5");
	FillSDL_RectFrom(spritesMap[CLOUD6] = new SDL_Rect(), stage1Elements, "cloud6");
	FillSDL_RectFrom(spritesMap[TREE1] = new SDL_Rect(), stage1Elements, "tree1");
	FillSDL_RectFrom(spritesMap[TREE2] = new SDL_Rect(), stage1Elements, "tree2");
	FillSDL_RectFrom(spritesMap[LADY_DOWN_IDLE] = new SDL_Rect(), npcElements, "ladyDownIdle");
}

void ModuleJsonManager::FillAnimationsMap()
{
	FillAnimationFrom(animationsMap[LEAVES] = new Animation(), stage1Elements, "leaves");
	FillAnimationFrom(animationsMap[HOMER_IDLE] = new Animation(), homerElements, "idle");
	FillAnimationFrom(animationsMap[HOMER_YAWN] = new Animation(), homerElements, "yawn");
	SetHomerYawnAnimation();
	FillAnimationFrom(animationsMap[HOMER_WALK] = new Animation(), homerElements, "walk");
	FillAnimationFrom(animationsMap[HOMER_WALK_UP] = new Animation(), homerElements, "walkUp");
	FillAnimationFrom(animationsMap[HOMER_ATTACK1] = new Animation(), homerElements, "attack1");
	FillAnimationFrom(animationsMap[HOMER_ATTACK2] = new Animation(), homerElements, "attack2");
	FillAnimationFrom(animationsMap[HOMER_JUMP_UP] = new Animation(), homerElements, "jumpUp");
	FillAnimationFrom(animationsMap[HOMER_JUMP_DOWN] = new Animation(), homerElements, "jumpDown");
	FillAnimationFrom(animationsMap[LADY_TOP] = new Animation(), npcElements, "ladyTop");
	FillAnimationFrom(animationsMap[LADY_DOWN_WALK] = new Animation(), npcElements, "ladyDownWalk");
	FillAnimationFrom(animationsMap[BIRD_IDLE] = new Animation(), npcElements, "birdIdle");
	FillAnimationFrom(animationsMap[BIRD_CLEAN] = new Animation(), npcElements, "birdClean");
	FillAnimationFrom(animationsMap[HOWIE_FRONT_WALK] = new Animation(), npcElements, "howieFrontWalk");
	FillAnimationFrom(animationsMap[MARTIN_IDLE] = new Animation(), npcElements, "martinIdle");
	FillAnimationFrom(animationsMap[SKINNER_IDLE1] = new Animation(), npcElements, "skinnerIdle");
	FillAnimationFrom(animationsMap[HAMSTER] = new Animation(), npcElements, "hamster");
}

void ModuleJsonManager::FillTransparentPixelColorsMap()
{
	FillTransparentPixelColorFrom(transparentPixelColorsMap[STAGE1] = new SDL_Color(), stage1);
	FillTransparentPixelColorFrom(transparentPixelColorsMap[HOMER] = new SDL_Color(), homer);
	FillTransparentPixelColorFrom(transparentPixelColorsMap[NPC] = new SDL_Color(), npc);
}

void ModuleJsonManager::FillAudioMap()
{
	audioPathsMap[STAGE1_AUDIO] = const_cast<char*>(json_object_get_string(audio, "stage1"));
}

void ModuleJsonManager::FillCollidersMap()
{
	JSON_Object* homerCollider = json_object_get_object(colliders, "homer");
	collidersMap[HOMER_COLLIDER] = new pair<int, int>(
		static_cast<int>(json_object_get_number(homerCollider, "w")),
		static_cast<int>(json_object_get_number(homerCollider, "h"))
	);

	JSON_Object* streetLightCollider = json_object_get_object(colliders, "streetlight");
	collidersMap[STREETLIGHT_COLLIDER] = new pair<int, int>(
		static_cast<int>(json_object_get_number(streetLightCollider, "w")),
		static_cast<int>(json_object_get_number(streetLightCollider, "h"))
	);

	JSON_Object* restaurantCollider = json_object_get_object(colliders, "restaurant");
	collidersMap[RESTAURANT_COLLIDER] = new pair<int, int>(
		static_cast<int>(json_object_get_number(restaurantCollider, "w")),
		static_cast<int>(json_object_get_number(restaurantCollider, "h"))
		);

	JSON_Object* noiselandCollider = json_object_get_object(colliders, "noiseland");
	collidersMap[NOISELAND_COLLIDER] = new pair<int, int>(
		static_cast<int>(json_object_get_number(noiselandCollider, "w")),
		static_cast<int>(json_object_get_number(noiselandCollider, "h"))
		);

	JSON_Object* treeCollider = json_object_get_object(colliders, "tree");
	collidersMap[TREE_COLLIDER] = new pair<int, int>(
		static_cast<int>(json_object_get_number(treeCollider, "w")),
		static_cast<int>(json_object_get_number(treeCollider, "h"))
		);
}

void ModuleJsonManager::FillSDL_RectFrom(SDL_Rect* sdlRect, const JSON_Object* jsonObject, const char* name)
{
	sdlRect->x = static_cast<int>(json_object_get_number(
		json_object_get_object(jsonObject, name),
		"x"
	));
	sdlRect->y = static_cast<int>(json_object_get_number(
		json_object_get_object(jsonObject, name),
		"y"
	));
	sdlRect->w = static_cast<int>(json_object_get_number(
		json_object_get_object(jsonObject, name),
		"w"
	));
	sdlRect->h = static_cast<int>(json_object_get_number(
		json_object_get_object(jsonObject, name),
		"h"
	));
}

void ModuleJsonManager::FillAnimationFrom(Animation* animation, const JSON_Object* jsonObject, const char* name)
{
	JSON_Array* animationInfo = json_object_get_array(jsonObject, name);
	JSON_Object* frameCoordinates = json_array_get_object(animationInfo, 0);
	JSON_Object* animationSettings = json_array_get_object(animationInfo, 1);
	int numFrames = static_cast<int>(json_object_get_number(animationSettings, "frames"));
	animation->speed = static_cast<float>(json_object_get_number(animationSettings, "speed"));

	SDL_Rect sdlRect;
	FillSDL_RectWithFrameCoordinates(&sdlRect, frameCoordinates);
	animation->frames.push_back(sdlRect);

	FillNextSDL_Rects(&sdlRect, numFrames, animation);
}

void ModuleJsonManager::FillTransparentPixelColorFrom(SDL_Color* color, const JSON_Object* jsonObject)
{
	JSON_Object* colorObj = json_object_get_object(jsonObject, "transparentPixelColor");
	color->r = static_cast<Uint8>(json_object_get_number(
		colorObj,
		"r"
	));
	color->g = static_cast<Uint8>(json_object_get_number(
		colorObj,
		"g"
	));
	color->b = static_cast<Uint8>(json_object_get_number(
		colorObj,
		"b"
	));
	color->a = static_cast<Uint8>(json_object_get_number(
		colorObj,
		"a"
	));
}

void ModuleJsonManager::FillSDL_RectWithFrameCoordinates(SDL_Rect* sdlRect, JSON_Object* frameCoordinates)
{
	sdlRect->x = static_cast<int>(json_object_get_number(
		frameCoordinates,
		"x"
	));
	sdlRect->y = static_cast<int>(json_object_get_number(
		frameCoordinates,
		"y"
	));
	sdlRect->w = static_cast<int>(json_object_get_number(
		frameCoordinates,
		"w"
	));
	sdlRect->h = static_cast<int>(json_object_get_number(
		frameCoordinates,
		"h"
	));
}

void ModuleJsonManager::FillNextSDL_Rects(SDL_Rect* firstSdlRect, int numFrames, Animation* animation)
{
	for (int i = 0; i < numFrames - 1; i++)
	{
		SDL_Rect nextSdlRect;
		nextSdlRect.x = firstSdlRect->x + (firstSdlRect->w * (i+1));
		nextSdlRect.y = firstSdlRect->y;
		nextSdlRect.w = firstSdlRect->w;
		nextSdlRect.h = firstSdlRect->h;
		animation->frames.push_back(nextSdlRect);
	}
}

void ModuleJsonManager::SetHomerYawnAnimation()
{
	animationsMap[HOMER_YAWN]->loop = false;
	animationsMap[HOMER_YAWN]->frames.push_back(animationsMap[HOMER_YAWN]->frames.at(animationsMap[HOMER_YAWN]->frames.size() - 2));
	animationsMap[HOMER_YAWN]->frames.push_back(animationsMap[HOMER_YAWN]->frames.at(animationsMap[HOMER_YAWN]->frames.size() - 2));
}
