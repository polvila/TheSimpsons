#include "ModuleStage1.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

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

	return true;
}

bool ModuleStage1::CleanUp()
{
	LOG("Unloading Stage 1");

	App->GetModuleTextures()->Unload(graphicsStage1);

	return true;
}


update_status ModuleStage1::Update()
{
	//Background
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, 10, App->GetModuleJsonManager()->GetSDL_RectOf(BACKGROUND), CAMERA_VELOCITY);
	//Foreground
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, 8, App->GetModuleJsonManager()->GetSDL_RectOf(FOREGROUND), CAMERA_VELOCITY);
	
	//Doors
	App->GetModuleRender()->Blit(graphicsStage1, 1041, 16, 7, App->GetModuleJsonManager()->GetSDL_RectOf(PARKING_DOOR), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 336, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR1), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 520, 96, 7, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR2), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 723, 83, 7, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR3), CAMERA_VELOCITY);

	//Fence
	App->GetModuleRender()->Blit(graphicsStage1, 768, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 832, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 896, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE), CAMERA_VELOCITY);

	//Streetlights
	App->GetModuleRender()->Blit(graphicsStage1, 226, 69, 2, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 450, 69, 2, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 1330, -67, 7, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), CAMERA_VELOCITY);
	App->GetModuleRender()->Blit(graphicsStage1, 1642, -67, 7, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), CAMERA_VELOCITY);

	//Tree
	App->GetModuleRender()->Blit(graphicsStage1, 1240, 32, 7, App->GetModuleJsonManager()->GetSDL_RectOf(TREE1), CAMERA_VELOCITY);

	//Hamster
	App->GetModuleRender()->Blit(graphicsNpc, 646, 102, 7, &App->GetModuleJsonManager()->GetAnimationOf(HAMSTER)->GetCurrentFrame(), CAMERA_VELOCITY);


	return UPDATE_CONTINUE;
}

