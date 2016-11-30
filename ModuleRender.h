#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_scancode.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed = 1.0f) const;
	void SetRect(SDL_Rect rect, SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed) const;
	SDL_Rect GetCamera() const;
	void SetCameraPosition(int x, int y);

private:

	static Uint32 GetFlagsWithVsync(bool active);
	bool AssertRenderCreation() const;
	void HandleDebugCamera() const;
	static void SetCameraPositionWithKey(SDL_Scancode scancodeKey, int x, int y);
	void SetRectPosition(SDL_Rect* rect, int x, int y, float speed) const;
	void TryToSetRectSize(SDL_Rect rect, SDL_Texture* texture, SDL_Rect* section) const;
	void SetRectSize(SDL_Rect* rect, int w, int h) const;
	void SetRectSizeProportionalToScreenSize(SDL_Rect* rect) const;
	bool TryToBlitToScreen(SDL_Texture* texture, SDL_Rect* section, SDL_Rect rect) const;

	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
};

#endif // __MODULERENDER_H__