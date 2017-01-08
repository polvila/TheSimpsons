#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include <list>
#include "Globals.h"
#include "Module.h"
#include "ModuleCollision.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleJsonManager;
class ModuleStage1;
class ModulePlayer;

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
	ModuleStage1* GetModuleStage1() const;
	ModulePlayer* GetModulePlayer() const;
	ModuleCollision* GetModuleCollision() const;
private:

	ModuleInput* input;
	ModuleWindow* window;
	ModuleRender* renderer;
	ModuleTextures* textures;
	ModuleAudio* audio;
	ModuleJsonManager* jsonManager;
	ModuleStage1* stage1;
	ModulePlayer* player;
	ModuleCollision* collision;


	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__