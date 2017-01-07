#include "ModuleStage1.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

// Reference at https://www.youtube.com/watch?v=3mZKoejwKOs&t=1047s

ModuleStage1::ModuleStage1(bool start_enabled) : Module(start_enabled)
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

	graphicsNpc = App->GetModuleTextures()->Load(
		App->GetModuleJsonManager()->GetTexturePathOf(NPC),
		App->GetModuleJsonManager()->GetTransparentPixelColor(NPC)
	);

	//TODO: Uncomment to listen the stage song
	//App->GetModuleAudio()->PlayFx(
	//	App->GetModuleAudio()->LoadFx(
	//		App->GetModuleJsonManager()->GetAudioPathOf(STAGE1_AUDIO)));

	return true;
}

bool ModuleStage1::CleanUp()
{
	LOG("Unloading Stage 1");

	App->GetModuleTextures()->Unload(graphicsStage1);

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
	App->GetModuleRender()->Blit(graphicsNpc, 646, 102, 157, &App->GetModuleJsonManager()->GetAnimationOf(HAMSTER)->GetCurrentFrame());
}

void ModuleStage1::MoveCamera() const
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

	int minWorldPositionCameraYMovement = GetWorldYPosition(minPlayerPercentageCameraMovement.y);
	if (App->GetModulePlayer()->GetRealGroundYPosition() < minWorldPositionCameraYMovement &&
		App->GetModuleRender()->GetCamera().y < minCameraYPosition)
	{
		int cameraShift = minWorldPositionCameraYMovement - App->GetModulePlayer()->GetRealGroundYPosition();
		if(minCameraYPosition > App->GetModuleRender()->GetCamera().y + cameraShift*SCREEN_SIZE)
			App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x,
												  App->GetModuleRender()->GetCamera().y + cameraShift*SCREEN_SIZE);
		else
			App->GetModuleRender()->SetCameraPosition(App->GetModuleRender()->GetCamera().x,
				minCameraYPosition);
	}
	
	int maxWorldPositionCameraXMovement = GetWorldXPosition(maxPlayerPerecentageCameraMovement.x);
	if(App->GetModulePlayer()->GetPosition().x > maxWorldPositionCameraXMovement &&
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
