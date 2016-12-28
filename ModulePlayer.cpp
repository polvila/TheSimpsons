#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(bool start_enabled)
{
	position.x = 68;
	position.y = 144;
	layer = 5;
}

ModulePlayer::~ModulePlayer()
{
}

bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->GetModuleTextures()->Load(
		App->GetModuleJsonManager()->GetTexturePathOf(HOMER),
		App->GetModuleJsonManager()->GetTransparentPixelColor(HOMER)
	);

	return true;
}

bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->GetModuleTextures()->Unload(graphics);

	return true;
}

update_status ModulePlayer::Update()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += 1;
		App->GetModuleRender()->Blit(graphics, position.x, position.y, layer,
			&App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame(), CAMERA_VELOCITY);
	}
	else if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= 1;
		App->GetModuleRender()->Blit(graphics, position.x, position.y, layer,
			&App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame(), CAMERA_VELOCITY, SDL_FLIP_HORIZONTAL);
	}
	else
		App->GetModuleRender() ->Blit(graphics, position.x, position.y, layer, 
			App->GetModuleJsonManager()->GetSDL_RectOf(HOMER_IDLE), CAMERA_VELOCITY);


	return UPDATE_CONTINUE;
}

