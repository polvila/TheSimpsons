#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(bool start_enabled)
{
	position.x = 80;
	position.y = 208;
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
		position.x += 2;
		currentFrame = &App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame();
		SetBlitCoordinates(blitCoordinates, currentFrame);
		App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
			currentFrame, CAMERA_VELOCITY);
		lookingRight = true;
	}
	else if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= 2;
		currentFrame = &App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK)->GetCurrentFrame();
		SetBlitCoordinates(blitCoordinates, currentFrame);
		App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
			currentFrame, CAMERA_VELOCITY, SDL_FLIP_HORIZONTAL);
		lookingRight = false;
	}
	else
	{
		currentFrame = App->GetModuleJsonManager()->GetSDL_RectOf(HOMER_IDLE);
		SetBlitCoordinates(blitCoordinates, currentFrame);

		App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
			currentFrame, CAMERA_VELOCITY, !lookingRight? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit) const
{
	blitCoordinates.x = position.x - (rectToBlit->w / 2);
	blitCoordinates.y = position.y - (rectToBlit->h);
}

