#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleStage1.h"

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
	SDL_Rect* currentFrame = nullptr;
	int speed = 2;

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		if (position.x < 1614) position.x += speed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
		lookingRight = true;
	}
	
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		if(position.x > App->GetModuleStage1()->GetMinXPlayerPosition()) position.x -= speed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
		lookingRight = false;
	}

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		if(position.y < 256) position.y += speed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
	}

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		if(position.y > 153) position.y -= speed;
		Animation* walkUp = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK_UP);
		currentAnimation = walkUp;
	}

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_IDLE);

	currentFrame = &currentAnimation->GetCurrentFrame();
	SetBlitCoordinates(blitCoordinates, currentFrame);
	App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, layer,
	                             currentFrame, lookingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

	return UPDATE_CONTINUE;
}

iPoint ModulePlayer::GetPosition() const
{
	return position;
}

void ModulePlayer::SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit) const
{
	blitCoordinates.x = position.x - (rectToBlit->w / 2);
	blitCoordinates.y = position.y - (rectToBlit->h);
}

