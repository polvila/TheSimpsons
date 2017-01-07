#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision()
{
	collisionTypes[PLAYER][PLAYER] = false;
	collisionTypes[PLAYER][WALL] = true;
	collisionTypes[WALL][WALL] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	bool needCheck = false;
	bool collisionFinded = false;
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end() && !collisionFinded; ++it)
	{
		for (list<Collider*>::iterator it2 = colliders.begin(); it2 != colliders.end() && !collisionFinded; ++it2)
		{
			if (it != it2) {
				if ((*it)->colliderType <= (*it2)->colliderType)
					needCheck = collisionTypes[(*it)->colliderType][(*it2)->colliderType];
				else
					needCheck = collisionTypes[(*it2)->colliderType][(*it)->colliderType];

				if (needCheck && (*it)->CheckCollision((*it2)->rect))
				{
					(*it)->collisionObserver->OnCollision(*it, *it2);
					(*it2)->collisionObserver->OnCollision(*it, *it2);
					collisionFinded = true;
				}
			}
		}
	}

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->GetModuleRender()->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, ColliderType colliderType, CollisionObserver* collisionObserver)
{
	Collider* ret = new Collider(rect, colliderType, collisionObserver);

	colliders.push_back(ret);

	return ret;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (!(abs((rect.x + (rect.w / 2)) - (r.x + (r.w / 2))) > ((rect.w / 2) + (r.w / 2))) && !(abs((rect.y + (rect.h / 2)) - (r.y + (r.h / 2))) > ((rect.h / 2) + (r.h / 2))));
}
