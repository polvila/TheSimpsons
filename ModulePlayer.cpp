#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleStage1.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
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
	Frontwards();
	Backwards();
	Downwards();
	Upwards();
	Idle();
	Jump();
	Attack();

	BlitAnimationManagement();
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

int ModulePlayer::GetRealGroundYPosition() const
{
	return realGrondYPosition;
}

void ModulePlayer::SetBlitCoordinates(iPoint &blitCoordinates, SDL_Rect* rectToBlit, bool attackInProgress) const
{
	blitCoordinates.x = position.x - (rectToBlit->w / 2);
	blitCoordinates.x += attackInProgress ? (lookingRight? 16 : -16) : 0;
	blitCoordinates.y = realYPosition - (rectToBlit->h);
}

void ModulePlayer::CalculeRealYPosition()
{
	int degrees = 45;
	float maxDownZposition = 362.0f;
	float piRadiansInDegrees = 180.0;
	realGrondYPosition = static_cast<int>(round((maxDownZposition - zPosition) * cos(degrees * M_PI / piRadiansInDegrees)));
	realYPosition = realGrondYPosition - position.y;
}

bool ModulePlayer::AnyMovementKeyPressed()
{
	return App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_IDLE &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_IDLE;
}

void ModulePlayer::Frontwards()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		if (position.x < 1614 && !attackInProgress) position.x += xSpeed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
		lookingRight = true;
	}
}

void ModulePlayer::Backwards()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		if (position.x > App->GetModuleStage1()->GetMinXPlayerPosition() && !attackInProgress)
			position.x -= xSpeed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
		lookingRight = false;
	}
}

void ModulePlayer::Downwards()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		if (zPosition > 0.0f && !attackInProgress) zPosition -= zSpeed;
		Animation* walk = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK);
		currentAnimation = walk;
	}
}

void ModulePlayer::Upwards()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT &&
		App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		if (zPosition < 146.0f && !attackInProgress) zPosition += zSpeed;
		Animation* walkUp = App->GetModuleJsonManager()->GetAnimationOf(HOMER_WALK_UP);
		currentAnimation = walkUp;
	}
}

void ModulePlayer::Idle()
{
	if (AnyMovementKeyPressed())
		currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_IDLE);
}

void ModulePlayer::Attack()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_H) == KEY_DOWN || attackInProgress)
	{
		if (attacksCount == 0)
			currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_ATTACK1);
		else
			currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_ATTACK2);

		if (currentAnimation->Finished())
		{
			attackInProgress = false;
			currentAnimation->Reset();
			attacksCount = (attacksCount + 1) % 3;
		}
		else
			attackInProgress = true;
	}
}

void ModulePlayer::Jump()
{
	if (App->GetModuleInput()->GetKey(SDL_SCANCODE_J) == KEY_REPEAT || jumpInProgress)
	{
		if (!jumpTimer.IsRunning())
		{
			jumpInProgress = true;
			jumpTimer.Start();
			yVelocity = 280;
		}
		int time = jumpTimer.GetTimerTicks();
		JumpFunction(time);
		if (position.y <= 0 && time != 0)
		{
			jumpTimer.Clear();
			jumpInProgress = false;
			position.y = 0;
		}
		currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_JUMP_UP);
	}
}

void ModulePlayer::JumpFunction(int &time)
{
	int yGravity = 11;
	float timeInSeconds = static_cast<float>(time) / 1000;

	yVelocity = yVelocity - static_cast<int>(yGravity * timeInSeconds);
	position.y = static_cast<int>(yVelocity * timeInSeconds); 
}

void ModulePlayer::BlitAnimationManagement()
{
	iPoint blitCoordinates = {};
	currentFrame = &currentAnimation->GetCurrentFrame();
	CalculeRealYPosition();
	SetBlitCoordinates(blitCoordinates, currentFrame, attackInProgress);
	App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, static_cast<int>(zPosition),
		currentFrame, lookingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}
