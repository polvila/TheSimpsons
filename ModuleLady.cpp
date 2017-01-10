#include "ModuleLady.h"
#include "ModuleJsonManager.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "time.h"

ModuleLady::ModuleLady(bool start_enabled)
{
}

ModuleLady::~ModuleLady()
{
}

bool ModuleLady::Start()
{
	graphicsNpc = App->GetModuleTextures()->Load(
		App->GetModuleJsonManager()->GetTexturePathOf(NPC),
		App->GetModuleJsonManager()->GetTransparentPixelColor(NPC)
	);

	UpdateNextPositionLady();

	srand(static_cast<unsigned int>(time(nullptr)));

	return true;
}

update_status ModuleLady::Update()
{
	if (ladyTimer.GetTimerTicks() / 1000 > 1)
		ladyStopped = false;

	ChangePositionLady();
	BlitLady();

	return UPDATE_CONTINUE;
}

bool ModuleLady::CleanUp()
{
	LOG("Unloading Npc");

	App->GetModuleTextures()->Unload(graphicsNpc);

	return true;
}

void ModuleLady::ChangePositionLady()
{
	int ladyVelocity = 1;
	if (nextPositionLady < actualPositionLady && !ladyStopped)
	{
		ladyLookingRight = false;
		actualPositionLady -= ladyVelocity;
	}
	else if (nextPositionLady > actualPositionLady && !ladyStopped)
	{
		ladyLookingRight = true;
		actualPositionLady += ladyVelocity;
	}
	else
		FindNewPositionAndStopLady();
}

void ModuleLady::FindNewPositionAndStopLady()
{
	if (!ladyStopped)
	{
		UpdateNextPositionLady();
		ladyTimer.Clear();
		ladyTimer.Start();
	}
	ladyStopped = true;
}

void ModuleLady::BlitLady() const
{
	int offsetTopLadyAnim = 7;
	App->GetModuleRender()->Blit(graphicsNpc, actualPositionLady + offsetTopLadyAnim, 78, 159,
		&App->GetModuleJsonManager()->GetAnimationOf(LADY_TOP)->GetCurrentFrame(),
		ladyLookingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
	App->GetModuleRender()->Blit(graphicsNpc, actualPositionLady, 109, 159,
		&App->GetModuleJsonManager()->GetAnimationOf(LADY_DOWN_WALK)->GetCurrentFrame(),
		ladyLookingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void ModuleLady::UpdateNextPositionLady()
{
	int minNextPositionLady = 239;
	int maxNextPositionLady = 86;
	nextPositionLady = rand() % maxNextPositionLady + minNextPositionLady;
	LOG("Rand: %d", nextPositionLady);
}
