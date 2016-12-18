#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include <list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleJsonManager;

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
	ModuleAudio* GetModuleAudio() const;
	ModuleJsonManager* GetModuleJsonManager() const;
private:

	ModuleInput* input;
	ModuleWindow* window;
	ModuleRender* renderer;
	ModuleTextures* textures;
	ModuleAudio* audio;
	ModuleJsonManager* jsonManager;

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__