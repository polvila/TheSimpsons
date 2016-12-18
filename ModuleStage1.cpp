#include "ModuleStage1.h"

ModuleStage1::ModuleStage1(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleStage1::~ModuleStage1()
{
}

bool ModuleStage1::Start()
{
	return true;
}

update_status ModuleStage1::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleStage1::CleanUp()
{
	return true;
}
