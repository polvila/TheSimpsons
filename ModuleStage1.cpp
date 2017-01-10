#include "ModuleStage1.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

// Reference at https://www.youtube.com/watch?v=3mZKoejwKOs&t=1047s

ModuleStage1::ModuleStage1(bool start_enabled) : Module(start_enabled), CollisionObserver()
{
}

ModuleStage1::~ModuleStage1()
{
}

bool ModuleStage1::Start()
{
	LOG("Loading Stage 1");

	graphicsStage1 = App->GetModuleTextures()->Load(
		App->GetModuleJsonManager()->GetTexturePathOf(STAGE1),
		App->GetModuleJsonManager()->GetTransparentPixelColor(STAGE1)
	);

	LOG("Loading NPC");

	graphicsNpc = App->GetModuleTextures()->Load(
		App->GetModuleJsonManager()->GetTexturePathOf(NPC),
		App->GetModuleJsonManager()->GetTransparentPixelColor(NPC)
	);

	birdTimer.Start();
	
	LOG("Play background music");

	App->GetModuleAudio()->PlayFx(
		App->GetModuleAudio()->LoadFx(
			App->GetModuleJsonManager()->GetAudioPathOf(STAGE1_AUDIO)));

	CreateStageColliders();

	return true;
}

bool ModuleStage1::CleanUp()
{
	LOG("Unloading Stage 1");
	App->GetModuleTextures()->Unload(graphicsStage1);

	LOG("Unloading Npc");
	App->GetModuleTextures()->Unload(graphicsNpc);

	return true;
}

int ModuleStage1::GetMinXPlayerPosition() const
{
	return GetWorldXPosition(minPlayerPercentageCameraMovement.x);
}

update_status ModuleStage1::Update()
{
	BlitBackground();
	BlitForeground();
	BlitDoors();
	BlitFence();
	BlitStreetlights();
	BlitTree();
	BlitHamster();
	BlitMartin();
	BlitSkinner();
	BlitHowie();
	BlitBird();

	MoveCamera();

	return UPDATE_CONTINUE;
}


void ModuleStage1::BlitBackground() const
{
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, 160, App->GetModuleJsonManager()->GetSDL_RectOf(BACKGROUND));
}

void ModuleStage1::BlitForeground() const
{
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, 158, App->GetModuleJsonManager()->GetSDL_RectOf(FOREGROUND));
}

void ModuleStage1::BlitDoors() const
{
	App->GetModuleRender()->Blit(graphicsStage1, 1041, 16, 157, App->GetModuleJsonManager()->GetSDL_RectOf(PARKING_DOOR));
	App->GetModuleRender()->Blit(graphicsStage1, 336, 80, 157, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR1));
	App->GetModuleRender()->Blit(graphicsStage1, 520, 96, 157, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR2));
	App->GetModuleRender()->Blit(graphicsStage1, 723, 83, 157, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR3));
}

void ModuleStage1::BlitFence() const
{
	App->GetModuleRender()->Blit(graphicsStage1, 768, 80, 157, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE));
	App->GetModuleRender()->Blit(graphicsStage1, 832, 80, 157, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE));
	App->GetModuleRender()->Blit(graphicsStage1, 896, 80, 157, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE));
}

void ModuleStage1::BlitStreetlights() const
{
	App->GetModuleRender()->Blit(graphicsStage1, 226, 69, 16, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT));
	App->GetModuleRender()->Blit(graphicsStage1, 450, 69, 16, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT));
	App->GetModuleRender()->Blit(graphicsStage1, 1330, -67, 157, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT));
	App->GetModuleRender()->Blit(graphicsStage1, 1642, -67, 157, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT));
}

void ModuleStage1::BlitTree() const
{
	App->GetModuleRender()->Blit(graphicsStage1, 1240, 32, 157, App->GetModuleJsonManager()->GetSDL_RectOf(TREE1));
}

void ModuleStage1::BlitHamster() const
{
	App->GetModuleRender()->Blit(graphicsNpc, 642, 97, 157, &App->GetModuleJsonManager()->GetAnimationOf(HAMSTER)->GetCurrentFrame());
}

void ModuleStage1::BlitMartin() const
{
	App->GetModuleRender()->Blit(graphicsNpc, 833, 115, 134, &App->GetModuleJsonManager()->GetAnimationOf(MARTIN_IDLE)->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
}

void ModuleStage1::BlitSkinner() const
{
	App->GetModuleRender()->Blit(graphicsNpc, 799, 94, 134, &App->GetModuleJsonManager()->GetAnimationOf(SKINNER_IDLE)->GetCurrentFrame());
}

void ModuleStage1::BlitHowie() const
{
	App->GetModuleRender()->Blit(graphicsNpc, 425, 102, 159, &App->GetModuleJsonManager()->GetAnimationOf(HOWIE_FRONT_WALK)->GetCurrentFrame());
}

void ModuleStage1::BlitBird()
{
	if (birdTimer.GetTimerTicks() / 1000 > 2 && birdTimer.GetTimerTicks() / 1000 < 5)
		App->GetModuleRender()->Blit(graphicsNpc, 220, 37, 16, &App->GetModuleJsonManager()->GetAnimationOf(BIRD_IDLE)->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	else
	{
		App->GetModuleRender()->Blit(graphicsNpc, 209, 45, 16, &App->GetModuleJsonManager()->GetAnimationOf(BIRD_CLEAN)->GetCurrentFrame());
		if (birdTimer.GetTimerTicks() / 1000 > 5)
		{
			birdTimer.Clear();
			birdTimer.Start();
		}
	}
}

void ModuleStage1::MoveCamera() const
{
	ControlMaxYCameraPosition();
	ControlMinYCameraPositon();
	ControlMaxXCameraPosition();
}

void ModuleStage1::ControlMaxYCameraPosition() const
{
	int maxWorldPositionCameraYMovement = GetWorldYPosition(maxPlayerPerecentageCameraMovement.y);
	if (App->GetModulePlayer()->GetRealGroundYPosition() > maxWorldPositionCameraYMovement &&
		App->GetModuleRender()->GetCamera().y > maxCameraYPosition)
	{
		int cameraShift = App->GetModulePlayer()->GetRealGroundYPosition() - maxWorldPositionCameraYMovement;

		if (maxCameraYPosition < App->GetModuleRender()->GetCamera().y - cameraShift*SCREEN_SIZE)
			App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x,
				App->GetModuleRender()->GetCamera().y - cameraShift*SCREEN_SIZE);
		else
			App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x,
				maxCameraYPosition);
	}
}

void ModuleStage1::ControlMinYCameraPositon() const
{
	int minWorldPositionCameraYMovement = GetWorldYPosition(minPlayerPercentageCameraMovement.y);
	if (App->GetModulePlayer()->GetRealGroundYPosition() < minWorldPositionCameraYMovement &&
		App->GetModuleRender()->GetCamera().y < minCameraYPosition)
	{
		int cameraShift = minWorldPositionCameraYMovement - App->GetModulePlayer()->GetRealGroundYPosition();
		if (minCameraYPosition > App->GetModuleRender()->GetCamera().y + cameraShift*SCREEN_SIZE)
			App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x,
				App->GetModuleRender()->GetCamera().y + cameraShift*SCREEN_SIZE);
		else
			App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x,
				minCameraYPosition);
	}
}

void ModuleStage1::ControlMaxXCameraPosition() const
{
	int maxWorldPositionCameraXMovement = GetWorldXPosition(maxPlayerPerecentageCameraMovement.x);
	if (App->GetModulePlayer()->GetPosition().x > maxWorldPositionCameraXMovement &&
		App->GetModuleRender()->GetCamera().x > maxCameraXPosition)
	{
		int cameraShift = App->GetModulePlayer()->GetPosition().x - maxWorldPositionCameraXMovement;
		App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x - cameraShift*SCREEN_SIZE,
			App->GetModuleRender()->GetCamera().y);
	}
}

int ModuleStage1::GetWorldYPosition(float playerPerecentageCameraYMovement)
{
	return static_cast<int>(abs(App->GetModuleRender()->GetCamera().y) / SCREEN_SIZE +
		App->GetModuleRender()->GetCamera().h / SCREEN_SIZE * 
			playerPerecentageCameraYMovement);
}

int ModuleStage1::GetWorldXPosition(float playerPerecentageCameraXMovement)
{
	return static_cast<int>(abs(App->GetModuleRender()->GetCamera().x) / SCREEN_SIZE +
		App->GetModuleRender()->GetCamera().w / SCREEN_SIZE *
		playerPerecentageCameraXMovement); 
}

void ModuleStage1::OnCollision(Collider* collider1, Collider* collider2)
{
}

void ModuleStage1::CreateStageColliders()
{
	pair<int, int>* colliderSize = App->GetModuleJsonManager()->GetColliderSizeOf(STREETLIGHT_COLLIDER);
	streetlightCollider = App->GetModuleCollision()->AddCollider(
	{ 226, 238, colliderSize->first, colliderSize->second },
		WALL, this);
	streetlightCollider2 = App->GetModuleCollision()->AddCollider(
	{ 450, 238, colliderSize->first, colliderSize->second },
		WALL, this);

	colliderSize = App->GetModuleJsonManager()->GetColliderSizeOf(RESTAURANT_COLLIDER);
	restaurantShowcaseCollider = App->GetModuleCollision()->AddCollider(
	{ 233, 127, colliderSize->first, colliderSize->second },
		WALL, this);

	colliderSize = App->GetModuleJsonManager()->GetColliderSizeOf(NOISELAND_COLLIDER);
	noiselandShowcaseCollider = App->GetModuleCollision()->AddCollider(
	{ 420, 127, colliderSize->first, colliderSize->second },
		WALL, this);

	colliderSize = App->GetModuleJsonManager()->GetColliderSizeOf(TREE_COLLIDER);
	treeCollider = App->GetModuleCollision()->AddCollider(
	{ 1260, 145, colliderSize->first, colliderSize->second },
		WALL, this);
}
