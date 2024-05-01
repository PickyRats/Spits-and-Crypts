#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "Timer.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro(bool enabled = true);

	virtual ~SceneIntro();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();


private:

	SDL_Texture* logo;
	SDL_Texture* logo1;
	int y = 729;
	double a = 40;
	Timer timer;
	bool changingScene = false;
	int logo_audio;

};

#endif // __SCENEINTRO_H__