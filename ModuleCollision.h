#ifndef __MODULECOLLISION_H__
#define __MODULECOLLISION_H__

#include <list>
#include "Module.h"
#include "CollisionObserver.h"

enum ColliderType
{
	PLAYER, WALL, COUNT
};

struct Collider
{
	ColliderType colliderType;
	CollisionObserver* collisionObserver;
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	void* user_data;

	Collider(SDL_Rect rectangle, ColliderType colliderType, CollisionObserver* collisionObserver) :
		colliderType(colliderType), collisionObserver(collisionObserver), rect(rectangle), user_data(nullptr)
	{
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate() override;
	update_status Update() override;

	bool CleanUp() override;

	Collider* AddCollider(const SDL_Rect& rect, ColliderType colliderType, CollisionObserver* collisionObserver);
	void DebugDraw();

private:
	void CheckCollision();

	bool collisionTypes[COUNT][COUNT];
	std::list<Collider*> colliders;
	bool debug = false;
};


#endif // __MODULECOLLISION_H__