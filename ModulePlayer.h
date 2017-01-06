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

	iPoint GetPosition() const;
	int GetRealYPosition() const;

private:

	void SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit) const;
	int CalculeRealYPosition() const;

	SDL_Texture* graphics = nullptr;
	Animation* currentAnimation = nullptr;
	iPoint position = {0,0};
	float zPosition = 0.0f;
	int realYPosition = 0;
	bool lookingRight = true;
};

#endif // __MODULEPLAYER_H__