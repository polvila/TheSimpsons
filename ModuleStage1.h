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

	int GetMinXPlayerPosition() const;

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

	void MoveCamera() const;
	static int GetWorldYPosition(float playerPerecentageCameraYMovement);
	static int GetWorldXPosition(float playerPerecentageCameraXMovement);

	JSON_Value* root_value;
	JSON_Array* assets;

	fPoint maxPlayerPerecentageCameraMovement = { 0.63f, 0.92f };
	fPoint minPlayerPercentageCameraMovement = { 0.115f, 0.7f };
	int maxCameraYPosition = -108;
	int minCameraYPosition = 0;
	int maxCameraXPosition = -4122;
};

#endif // __MODULESTAGE1_H__