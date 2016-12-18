#ifndef __MODULESTAGE1_H__
#define __MODULESTAGE1_H__

#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "parson.h"

class ModuleStage1 : public Module
{
public:
	ModuleStage1(bool start_enabled = true);
	~ModuleStage1();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;


	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	SDL_Rect foreground;

private:
	JSON_Value* root_value;
	JSON_Array* assets;
};




#endif // __MODULESTAGE1_H__