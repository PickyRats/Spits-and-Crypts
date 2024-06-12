#ifndef __SCENESELECTION_H__
#define __SCENESELECTION_H__

#include "Module.h"
#include "SDL/include/SDL.h"


struct SDL_Texture;

class SceneSelection : public Module
{
public:

	SceneSelection(bool enabled = true);

	virtual ~SceneSelection();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	int currentSelection = 0;

private:

	SDL_Texture* background;
	SDL_Texture* char1;
	SDL_Texture* char2;
	SDL_Texture* char3;
	SDL_Texture* char4;

};

#endif // __SCENESELECTION_H__