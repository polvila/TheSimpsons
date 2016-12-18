#ifndef __JSONMANAGER_H__
#define __JSONMANAGER_H__

#include "SDL/include/SDL.h"
#include "parson.h"


enum Texture
{
	GROUND,
	BACKGROUND,
	FOREGROUND
};

class JsonManager
{
public:


	static SDL_Rect* GetSDL_RectOf(Texture texture);
	SDL_Texture* GetSDL_TextureOf(Texture texture);


private:
	JsonManager();
	~JsonManager();

	JSON_Value* rootValue;
	JSON_Object* assetsInfo;
	JSON_Object* textures;
	JSON_Object* stage1;
	JSON_Object* stage1Elements;

	JSON_Object* backgroundObj;
	SDL_Rect* background = new SDL_Rect();

	const char* stagePath;


	const char* inputFile = "assetsInfo.json";

};


#endif // __JSONMANAGER_H__