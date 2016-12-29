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
	iPoint blitCoordinates = {};
	SDL_Rect* currentFrame;
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += 1;
		currentFrame = &App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame();
		SetBlitCoordinates(blitCoordinates, currentFrame);
		App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
			currentFrame, CAMERA_VELOCITY);
	}
	else if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= 1;
		currentFrame = &App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame();
		SetBlitCoordinates(blitCoordinates, currentFrame);
		App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
			&App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame(), CAMERA_VELOCITY, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		currentFrame = &App->GetModuleJsonManager()->GetAnimationOf(HOMER_YAWN)->GetCurrentFrame();
		SetBlitCoordinates(blitCoordinates, currentFrame);

		App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
			currentFrame, CAMERA_VELOCITY);
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit) const
{
	blitCoordinates.x = position.x - (rectToBlit->w / 2);
	blitCoordinates.y = position.y - (rectToBlit->h);
}

