#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include <list>
#include "Globals.h"
#include "Module.h"
#include "ModuleTextures.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	ModuleWindow* GetModuleWindow() const;
	ModuleInput* GetModuleInput() const;
	ModuleRender* GetModuleRender() const;
	ModuleTextures* GetModuleTextures() const;
private:

	ModuleInput* input;
	ModuleWindow* window;
	ModuleRender* renderer;
	ModuleTextures* textures;
	
	

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__