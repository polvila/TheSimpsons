#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");	
	renderer = SDL_CreateRenderer(App->GetModuleWindow()->GetWindow(), -1, GetFlagsWithVsync(VSYNC));
	return AssertRenderCreation();
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	HandleDebugCamera();
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	if (renderer != nullptr)
		SDL_DestroyRenderer(renderer);

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed) const
{
	SDL_Rect rect = {};
	SetRect(&rect, texture, x, y, section, speed);
	//TODO: Add a PriorityQueueElement to priorty element and call the next method on Update for each element
	return true;//TryToBlitToScreen(texture, section, rect);
}

void ModuleRender::SetRect(SDL_Rect* rect, SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed) const
{
	SetRectPosition(rect, x, y, speed);
	TryToSetRectSize(rect, texture, section);
	SetRectSizeProportionalToScreenSize(rect);
}

SDL_Rect ModuleRender::GetCamera() const
{
	return camera;
}

SDL_Renderer* ModuleRender::GetRenderer() const
{
	return renderer;
}

void ModuleRender::SetCameraPosition(int x, int y)
{
	camera.x = x;
	camera.y = y;
}

Uint32 ModuleRender::GetFlagsWithVsync(bool active)
{
	Uint32 flags = 0;

	if (active)
		flags |= SDL_RENDERER_PRESENTVSYNC;

	return flags;
}

bool ModuleRender::AssertRenderCreation() const
{
	if (renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void ModuleRender::HandleDebugCamera() const
{
	int speed = 1;

	SetCameraPositionWithKey(SDL_SCANCODE_UP, GetCamera().x, GetCamera().y + speed);
	SetCameraPositionWithKey(SDL_SCANCODE_DOWN, GetCamera().x, GetCamera().y - speed);
	SetCameraPositionWithKey(SDL_SCANCODE_LEFT, GetCamera().x + speed, GetCamera().y);
	SetCameraPositionWithKey(SDL_SCANCODE_RIGHT, GetCamera().x - speed, GetCamera().y);
}

void ModuleRender::SetCameraPositionWithKey(SDL_Scancode scancodeKey, int x, int y)
{
	if (App->GetModuleInput()->GetKey(scancodeKey) == KEY_REPEAT)
		App->GetModuleRender()->SetCameraPosition(x, y);
}

void ModuleRender::SetRectPosition(SDL_Rect* rect, int x, int y, float speed) const
{
	rect->x = static_cast<int>(camera.x * speed) + x * SCREEN_SIZE;
	rect->y = static_cast<int>(camera.y * speed) + y * SCREEN_SIZE;
}

void ModuleRender::TryToSetRectSize(SDL_Rect* rect, SDL_Texture* texture, SDL_Rect* section)
{
	if (section != NULL)
		SetRectSize(rect, section->w, section->h);
	else
		SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
}

void ModuleRender::SetRectSize(SDL_Rect* rect, int w, int h)
{
	rect->w = w;
	rect->h = h;
}

void ModuleRender::SetRectSizeProportionalToScreenSize(SDL_Rect* rect)
{
	rect->w *= SCREEN_SIZE;
	rect->h *= SCREEN_SIZE;
}

bool ModuleRender::TryToBlitToScreen(SDL_Texture* texture, SDL_Rect* section, SDL_Rect rect) const
{
	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		return false;
	}
	return true;
}
