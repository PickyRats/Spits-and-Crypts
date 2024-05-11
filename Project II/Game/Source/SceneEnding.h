#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Module.h"
#include "Timer.h"

struct SDL_Texture;

class SceneEnding : public Module
{
public:

	SceneEnding(bool enabled = true);

	virtual ~SceneEnding();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();


private:

	SDL_Texture* endingImage;
	SDL_Texture* credits;
	int y = 729;
	double a = 40;
	Timer timer;
	bool changingScene = false;
	int logo_audio;

};

#endif // __SCENEENDING_H__