#ifndef __MODULELADY_H__
#define __MODULELADY_H__

#include "Module.h"
#include "Timer.h"

class ModuleLady : public Module
{
public:
	ModuleLady(bool start_enabled = true);
	~ModuleLady();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;
	
private:
	void ChangePositionLady();
	void FindNewPositionAndStopLady();
	void BlitLady() const;
	void UpdateNextPositionLady();

	SDL_Texture* graphicsNpc = nullptr;

	Timer ladyTimer;
	int nextPositionLady = 0;
	int actualPositionLady = 325;
	bool ladyStopped = false;
	bool ladyLookingRight = false;
};

#endif // __MODULELADY_H__