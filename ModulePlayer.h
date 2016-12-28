#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

private:
	SDL_Texture* graphics = nullptr;
	iPoint position;
	int layer;
};

#endif // __MODULEPLAYER_H__