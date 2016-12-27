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


	SDL_Texture* graphicsStage1 = nullptr;
	SDL_Texture* graphicsNpc = nullptr;

private:
	JSON_Value* root_value;
	JSON_Array* assets;
};

#endif // __MODULESTAGE1_H__