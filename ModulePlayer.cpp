#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleJsonManager.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleStage1.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled), CollisionObserver()
{
	position.x = 80;
	position.y = 0;
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

	pair<int, int>* homerColliderSize = 
		App->GetModuleJsonManager()->GetColliderSizeOf(HOMER_COLLIDER);
	collider = App->GetModuleCollision()->AddCollider(
		{ 0, 0, homerColliderSize->first, homerColliderSize->second }
			, PLAYER, this);

	yawnTimer.Start();

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
	{
		if(yawnTimer.GetTimerTicks()/1000 >= 2)
		{
			currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_YAWN);
			if (currentAnimation->Finished())
			{
				currentAnimation->Reset();
				yawnTimer.Clear();
				yawnTimer.Start();
			}
		}
		else
			currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_IDLE);
	}else 
	{
		if (currentAnimation == App->GetModuleJsonManager()->GetAnimationOf(HOMER_YAWN))
			currentAnimation->Reset();
		yawnTimer.Clear();
		yawnTimer.Start();
	}	
}

void ModulePlayer::Attack()
{
	if ((App->GetModuleInput()->GetKey(SDL_SCANCODE_H) == KEY_DOWN || attackInProgress) &&
		jumpInProgress == false)
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
	if ((App->GetModuleInput()->GetKey(SDL_SCANCODE_J) == KEY_REPEAT || jumpInProgress) &&
		attackInProgress == false)
	{
		if (!jumpTimer.IsRunning())
		{
			jumpInProgress = true;
			jumpTimer.Start();
			yVelocity = 280;
		}
		int time = jumpTimer.GetTimerTicks();
		JumpFunction(time);

		if (position.y == 107)
			falling = true;

		if(falling)
			currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_JUMP_DOWN);
		else
			currentAnimation = App->GetModuleJsonManager()->GetAnimationOf(HOMER_JUMP_UP);

		if (position.y <= 0 && time != 0)
		{
			jumpTimer.Clear();
			jumpInProgress = false, falling = false;
			position.y = 0;
		}
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
	collider->SetPos(position.x - collider->rect.w/2, realGrondYPosition - collider->rect.h);
	App->GetModuleRender()->Blit(graphics, blitCoordinates.x, blitCoordinates.y, static_cast<int>(zPosition),
		currentFrame, lookingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void ModulePlayer::OnCollision(Collider* collider1, Collider* collider2)
{
	if (collider1 == collider || collider2 == collider) //player has collided
	{ 
		Collider* wall = (collider2 == collider)? collider1 : collider2;
		
		if (App->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			position.x -= xSpeed;

		if (App->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			position.x += xSpeed;

		if (App->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			zPosition -= zSpeed;

		if (App->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			zPosition += zSpeed;

	}
}
