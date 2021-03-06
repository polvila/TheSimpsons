#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include <queue>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct PriorityQueueElement {
	SDL_Texture* texture;
	int z;
	SDL_Rect* section;
	SDL_Rect rect;
	SDL_RendererFlip flipType;

	bool operator<(PriorityQueueElement nextElement) const
	{
		return z < nextElement.z;
	}
};

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

	void Blit(SDL_Texture* texture, int x, int y, int z, SDL_Rect* section, SDL_RendererFlip flipType = SDL_FLIP_NONE);
	void SetRect(SDL_Rect* rect, SDL_Texture* texture, int x, int y, SDL_Rect* section) const;
	SDL_Rect GetCamera() const;
	SDL_Renderer* GetRenderer() const;
	void SetCameraPosition(int x, int y);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true) const;
	void CleanPriorityQueue();

private:

	static Uint32 GetFlagsWithVsync(bool active);
	bool AssertRenderCreation() const;
	void HandleDebugCamera() const;
	static void SetCameraPositionWithKey(SDL_Scancode scancodeKey, int x, int y);
	void SetRectPosition(SDL_Rect* rect, int x, int y) const;
	static void TryToSetRectSize(SDL_Rect* rect, SDL_Texture* texture, SDL_Rect* section);
	static void SetRectSize(SDL_Rect* rect, int w, int h);
	static void SetRectSizeProportionalToScreenSize(SDL_Rect* rect);
	bool TryToBlitToScreen(SDL_Texture* texture, SDL_Rect* section, SDL_Rect rect, SDL_RendererFlip flipType) const;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;

	std::priority_queue<PriorityQueueElement> priorityQueue;
};

#endif // __MODULERENDER_H__