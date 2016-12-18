
#include "JsonManager.h"
#include "Globals.h"


JsonManager::JsonManager()
{
	rootValue = json_parse_file(inputFile);
	assetsInfo = json_value_get_object(rootValue);
	textures = json_object_get_object(assetsInfo, "textures");
	stage1 = json_object_get_object(textures, "stage");
	stage1Elements = json_object_get_object(stage1, "elements");

	backgroundObj = json_object_get_object(stage1Elements, "background");
	background->x = static_cast<int>(json_object_get_number(backgroundObj, "x"));


	LOG("BACK X : %d", background->x);
	
	stagePath = json_object_get_string(stage1, "path");

}

JsonManager::~JsonManager()
{
	
}

SDL_Rect* JsonManager::GetSDL_RectOf(Texture texture)
{
	return nullptr;
}

SDL_Texture* JsonManager::GetSDL_TextureOf(Texture texture)
{
	return nullptr;
}




