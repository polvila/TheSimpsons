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
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, 10, App->GetModuleJsonManager()->GetSDL_RectOf(BACKGROUND), 10.75f);
	//Foreground
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, 8, App->GetModuleJsonManager()->GetSDL_RectOf(FOREGROUND), 10.75f);
	
	//Doors
	App->GetModuleRender()->Blit(graphicsStage1, 1041, 16, 7, App->GetModuleJsonManager()->GetSDL_RectOf(PARKING_DOOR), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 336, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR1), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 520, 96, 7, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR2), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 723, 83, 7, App->GetModuleJsonManager()->GetSDL_RectOf(DOOR3), 10.75f);

	//Fence
	App->GetModuleRender()->Blit(graphicsStage1, 768, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 832, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 896, 80, 7, App->GetModuleJsonManager()->GetSDL_RectOf(FENCE), 10.75f);

	//Streetlights
	App->GetModuleRender()->Blit(graphicsStage1, 226, 69, 2, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 450, 69, 2, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 1330, -67, 7, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 1642, -67, 7, App->GetModuleJsonManager()->GetSDL_RectOf(STREETLIGHT), 10.75f);

	//Tree
	App->GetModuleRender()->Blit(graphicsStage1, 1240, 32, 7, App->GetModuleJsonManager()->GetSDL_RectOf(TREE1), 10.75f);

	//Hamster
	App->GetModuleRender()->Blit(graphicsNpc, 646, 102, 7, &App->GetModuleJsonManager()->GetAnimationOf(HAMSTER)->GetCurrentFrame(), 10.75f);


	return UPDATE_CONTINUE;
}

