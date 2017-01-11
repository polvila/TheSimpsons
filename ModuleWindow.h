#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include "SDL/include/SDL_keyboard.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before render is available
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

	SDL_Window* GetWindow() const;

private:

	bool CreateWindow();
	static Uint32 GetFlagsWithFullscreen(bool active);
	void CreateSurfaceInsideTheWindow();
	bool InitSDLVideo() const;
	bool AssertWindowCreation() const;

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
};

#endif // __MODULEWINDOW_H__