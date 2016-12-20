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
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, App->GetModuleJsonManager()->GetSDL_RectOf(FOREGROUND), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 1041, 16, App->GetModuleJsonManager()->GetSDL_RectOf(PARKING_DOOR), 10.75f);
	App->GetModuleRender()->Blit(graphicsStage1, 0, 0, App->GetModuleJsonManager()->GetSDL_RectOf(BACKGROUND), 10.75f);
	



	return UPDATE_CONTINUE;
}

