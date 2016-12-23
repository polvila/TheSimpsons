#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include "SDL/include/SDL.h"

using namespace std;

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f)
	{}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= frames.size())
			current_frame = 0.0f;
		return frames[(int)current_frame];
	}
};

#endif // __ANIMATION_H__