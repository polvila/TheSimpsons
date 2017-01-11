#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleJsonManager.h"
#include "ModuleStage1.h"
#include "ModulePlayer.h"
#include "ModuleLady.h"
#include "ModuleFadeToBlack.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(jsonManager = new ModuleJsonManager());

	// Game Modules
	modules.push_back(stage1 = new ModuleStage1(false));
	modules.push_back(player = new ModulePlayer(false));
	modules.push_back(lady = new ModuleLady(false));

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(fade = new ModuleFadeToBlack());
}

Application::~Application()
{
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not enabled

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if ((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	fade->FadeToBlack(static_cast<Module*>(App->GetModuleStage1()), nullptr, 3.0f);
	
	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->PreUpdate();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->Update();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if ((*it)->IsEnabled() == true)
			ret = (*it)->CleanUp();

	return ret;
}

ModuleWindow* Application::GetModuleWindow() const
{
	return window;
}

ModuleInput* Application::GetModuleInput() const
{
	return input;
}

ModuleRender* Application::GetModuleRender() const
{
	return renderer;
}

ModuleTextures* Application::GetModuleTextures() const
{
	return textures;
}

ModuleAudio* Application::GetModuleAudio() const
{
	return audio;
}

ModuleJsonManager* Application::GetModuleJsonManager() const
{
	return jsonManager;
}

ModuleStage1* Application::GetModuleStage1() const
{
	return stage1;
}

ModulePlayer* Application::GetModulePlayer() const
{
	return player;
}

ModuleCollision* Application::GetModuleCollision() const
{
	return collision;
}

ModuleFadeToBlack* Application::GetModuleFadeToBlack() const
{
	return fade;
}

ModuleLady* Application::GetModuleLady() const
{
	return lady;
}
