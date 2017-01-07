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
	position.y = 0;;
	zPosition = 68;
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
	int xSpeed = 2;
	float zSpeed = 2.857f;
	LOG("Pos player z: %f", zPosition);
	LOG("Pos player real y: %d", realYPosition);
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		if (position.x < 1614 && !attackInProgress) position.x += xSpeed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
		lookingRight = true;
	}
	
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		if(position.x > App->GetModuleStage1()->GetMinXPlayerPosition() && !attackInProgress) 
			position.x -= xSpeed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
		lookingRight = false;
	}

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		if(zPosition > 0.0f && !attackInProgress) zPosition -= zSpeed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
	}

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		if (zPosition < 146.0f && !attackInProgress) zPosition += zSpeed;
		Animation* walkUp = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK_UP);
		currentAnimation = walkUp;
	}

	if (AnyMovementKeyPressed())
		currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_IDLE);

	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_H) == KEY_DOWN || attackInProgress)
	{
		currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_ATTACK2);
		if (currentAnimation->Finished())
		{
			attackInProgress = false;
			currentAnimation->Reset();
		}
		else
			attackInProgress = true;
	}

	currentFrame = &currentAnimation->GetCurrentFrame();
	realYPosition = CalculeRealYPosition();
	SetBlitCoordinates(blitCoordinates, currentFrame, attackInProgress);
	App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, static_cast<int>(zPosition),
	                             currentFrame, lookingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

	return UPDATE_CONTINUE;
}

iPoint ModulePlayer::GetPosition() const
{
	return position;
}

int ModulePlayer::GetRealYPosition() const
{
	return realYPosition;
}

void ModulePlayer::SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit, bool attackInProgress) const
{
	blitCoordinates.x = position.x - (rectToBlit->w / 2);
	blitCoordinates.x += attackInProgress ? (lookingRight? 16 : -16) : 0;
	blitCoordinates.y = realYPosition - (rectToBlit->h);
}

int ModulePlayer::CalculeRealYPosition() const
{
	return static_cast<int>( round( (362.0f - zPosition) * cos(45 * M_PI / 180.0) ) ) + position.y;
}

bool ModulePlayer::AnyMovementKeyPressed()
{
	return App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_IDLE;
}
