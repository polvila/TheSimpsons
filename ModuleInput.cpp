#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), mouse_motion({ 0,0 }), mouse({ 0, 0 })
{
	keyboard = new KeyState[MAX_KEYS];
	InitializeKeyboardToIdle();
	InitializeMouseButtonsToIdle();
	InitializeEventMap();
	InitializeEventParameterMap();
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	return InitSDLEvents();
}

// Called before the first frame
bool ModuleInput::Start()
{
	return true;
}

// Called each loop iteration
update_status ModuleInput::PreUpdate()
{
	SetWindowEventsToFalse();
	
	HandleKeyboard();
	HandleMouseButtons();
	HandleSDLEvents();
	
	return GetNewUpdateStatus();
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

const iPoint& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const iPoint& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

bool ModuleInput::InitSDLEvents() const
{
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void ModuleInput::InitializeKeyboardToIdle() const
{
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

void ModuleInput::InitializeMouseButtonsToIdle()
{
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

void ModuleInput::SetWindowEventsToFalse()
{
	memset(windowEvents, false, WE_COUNT * sizeof(bool));
}

void ModuleInput::HandleKeyboard() const
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
			SetKeyToPressed(i);
		else
			SetKeyToReleased(i);
	}
}

void ModuleInput::SetKeyToPressed(int keyId) const
{
	if (keyboard[keyId] == KEY_IDLE)
		keyboard[keyId] = KEY_DOWN;
	else
		keyboard[keyId] = KEY_REPEAT;
}

void ModuleInput::SetKeyToReleased(int keyId) const
{
	if (keyboard[keyId] == KEY_REPEAT || keyboard[keyId] == KEY_DOWN)
		keyboard[keyId] = KEY_UP;
	else
		keyboard[keyId] = KEY_IDLE;
}

void ModuleInput::HandleMouseButtons()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;
		else if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}
}

void ModuleInput::HandleSDLEvents()
{
	mouse_motion = { 0, 0 };

	while (SDL_PollEvent(&event) != 0)
	{
		if(eventMap.find(event.type) != eventMap.end() 
			&& eventParameterMap.find(event.type) != eventParameterMap.end())
			(this->*eventMap.at(event.type)) (eventParameterMap.at(event.type));
	}
}

update_status ModuleInput::GetNewUpdateStatus() const
{
	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

void ModuleInput::SetTrueWindowEvent(int event)
{
	windowEvents[event] = true;
}

void ModuleInput::SetStateMouseButton(int keyState)
{
	mouse_buttons[event.button.button - 1] = static_cast<KeyState>(keyState);
}

void ModuleInput::UpdateMouseMotionAndPosition(int)
{
	mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
	mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
	mouse.x = event.motion.x / SCREEN_SIZE;
	mouse.y = event.motion.y / SCREEN_SIZE;
}

void ModuleInput::InitializeEventMap()
{
	eventMap[SDL_QUIT] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT] = &ModuleInput::SetWindowEvents;
	eventMap[SDL_MOUSEBUTTONDOWN] = &ModuleInput::SetStateMouseButton;
	eventMap[SDL_MOUSEBUTTONUP] = &ModuleInput::SetStateMouseButton;
	eventMap[SDL_MOUSEMOTION] = &ModuleInput::UpdateMouseMotionAndPosition;

	eventMap[SDL_WINDOWEVENT_HIDDEN] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT_MINIMIZED] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT_FOCUS_LOST] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT_SHOWN] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT_FOCUS_GAINED] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT_MAXIMIZED] = &ModuleInput::SetTrueWindowEvent;
	eventMap[SDL_WINDOWEVENT_RESTORED] = &ModuleInput::SetTrueWindowEvent;
}

void ModuleInput::InitializeEventParameterMap()
{
	eventParameterMap[SDL_QUIT] = WE_QUIT;
	eventParameterMap[SDL_WINDOWEVENT] = 0;
	eventParameterMap[SDL_MOUSEBUTTONDOWN] = KEY_DOWN;
	eventParameterMap[SDL_MOUSEBUTTONUP] = 0;
	eventParameterMap[SDL_MOUSEMOTION] = KEY_UP;

	eventParameterMap[SDL_WINDOWEVENT_HIDDEN] = WE_HIDE;
	eventParameterMap[SDL_WINDOWEVENT_MINIMIZED] = WE_HIDE;
	eventParameterMap[SDL_WINDOWEVENT_FOCUS_LOST] = WE_HIDE;
	eventParameterMap[SDL_WINDOWEVENT_SHOWN] = WE_SHOW;
	eventParameterMap[SDL_WINDOWEVENT_FOCUS_GAINED] = WE_SHOW;
	eventParameterMap[SDL_WINDOWEVENT_MAXIMIZED] = WE_SHOW;
	eventParameterMap[SDL_WINDOWEVENT_RESTORED] = WE_SHOW;
}

void ModuleInput::SetWindowEvents(int)
{
	if(eventMap.find(event.window.event) != eventMap.end() 
		&& eventParameterMap.find(event.window.event) != eventParameterMap.end())
		(this->*eventMap.at(event.window.event)) (eventParameterMap.at(event.window.event));
}