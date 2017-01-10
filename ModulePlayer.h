#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Timer.h"
#include "ModuleCollision.h"

class ModulePlayer : public Module, CollisionObserver
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	iPoint GetPosition() const;
	int GetRealYPosition() const;
	int GetRealGroundYPosition() const;

private:

	void SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit, bool attackInProgress) const;
	void CalculeRealYPosition();
	static bool AnyMovementKeyPressed();
	void Frontwards();
	void Backwards();
	void Downwards();
	void Upwards();
	void Idle();
	void Attack();
	void Jump();
	void JumpFunction(int &time);
	void BlitAnimationManagement();
	void OnCollision(Collider* collider1, Collider* collider2) override;

	SDL_Texture* graphics = nullptr;
	Animation* currentAnimation = nullptr;
	SDL_Rect* currentFrame = nullptr;
	iPoint position = {0,0};
	float zPosition = 0.0f;
	float zSpeed = 2.857f;
	int xSpeed = 2;
	int realYPosition = 0;
	int realGrondYPosition = 0;
	bool lookingRight = true;
	bool attackInProgress = false;
	int attacksCount = 0;
	bool jumpInProgress = false;
	Timer jumpTimer;
	Timer yawnTimer;
	int yVelocity = 0;
	bool falling = false;

	Collider* collider = nullptr;
};

#endif // __MODULEPLAYER_H__