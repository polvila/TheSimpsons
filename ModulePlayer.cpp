#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(bool start_enabled)
{
	position.x = 100;
	position.y = 100;
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
		App->GetModuleRender()->Blit(graphics, position.x, position.y, 5,
			&App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame(), 10.75f);
	}
	else if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= 1;
		App->GetModuleRender()->Blit(graphics, position.x, position.y, 5,
			&App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame(), 10.75f, SDL_FLIP_HORIZONTAL);
	}
	else
		App->GetModuleRender() ->Blit(graphics, position.x, position.y, 5, 
			App->GetModuleJsonManager()->GetSDL_RectOf(HOMER_IDLE), 10.75f);


	return UPDATE_CONTINUE;
}

