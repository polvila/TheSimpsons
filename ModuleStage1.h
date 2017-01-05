#ifndef __MODULESTAGE1_H__
#define __MODULESTAGE1_H__

#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "parson.h"
#include "Point.h"

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
	void BlitBackground() const;
	void BlitForeground() const;
	void BlitDoors() const;
	void BlitFence() const;
	void BlitStreetlights() const;
	void BlitTree() const;
	void BlitHamster() const;

	void MoveCamera();

	JSON_Value* root_value;
	JSON_Array* assets;

	iPoint maxPositionAchieved = {300, 220};
	iPoint minPositionAchieved = { 10, 180 };
	int maxDownCameraPosition = -18;
	int maxUpCameraPosition = 0;
};

#endif // __MODULESTAGE1_H__