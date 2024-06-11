#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "App.h"
#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 80

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	bool pingpong = false;
	bool flip = false;
	int loopCount = 0;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int pingpongCount = 0;
	int pingpongDirection = 1;

	pugi::xml_document animFile;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}
	
	void ResetLoopCount()
	{
		loopCount = 0;
	}


	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed * (app->GetDT()/1000) * 60;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;
			

			if (pingpong) {
				pingpongDirection = -pingpongDirection;
				if (currentFrame == 0) {
					pingpong = false;
				}
			}
				
		}
	}

	int GetLoopCount() const 
	{
		return loopCount;
	}
	
	int GetPingPongCount() const 
	{
		return pingpongCount;
	}
	
	float GetCurrentFrameCount() const
	{
		return currentFrame;
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void LoadAnimations(const char* animationFrame, const char* entity)
	{
		pugi::xml_parse_result result = animFile.load_file("config.xml");
		if (result != NULL)
		{
			pugi::xml_node animationName = animFile.child("config").child("animations").child(entity).child(animationFrame);
			loop = animationName.attribute("loop").as_bool();
			speed = animationName.attribute("speed").as_float();
			for (pugi::xml_node animation = animationName.child("animation"); animation; animation = animation.next_sibling("animation"))
			{
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
			}
		}
	}
};

#endif