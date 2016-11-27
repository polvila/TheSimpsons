#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

ModuleWindow::ModuleWindow() : Module()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");

	if(InitSDLVideo() && CreateWindow())
	{
		CreateSurfaceInsideTheWindow();
		return true;
	}
	return false;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	if (window != nullptr)
		SDL_DestroyWindow(window);

	SDL_Quit();
	return true;
}

SDL_Window* ModuleWindow::GetWindow() const
{
	return window;
}

bool ModuleWindow::CreateWindow()
{
	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 
		GetFlagsWithFullscreen(FULLSCREEN));
	return AssertWindowCreation();
}

Uint32 ModuleWindow::GetFlagsWithFullscreen(bool active)
{
	Uint32 flags = SDL_WINDOW_SHOWN;

	if (active)
		flags |= SDL_WINDOW_FULLSCREEN;

	return flags;
}

void ModuleWindow::CreateSurfaceInsideTheWindow()
{
	screen_surface = SDL_GetWindowSurface(window);
}

bool ModuleWindow::InitSDLVideo() const
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool ModuleWindow::AssertWindowCreation() const
{
	if (window == nullptr)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}



