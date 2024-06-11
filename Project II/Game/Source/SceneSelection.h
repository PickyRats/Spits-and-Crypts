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
	
	bool IsMouseOverRect(int mouseX, int mouseY, SDL_Rect rect);


private:

	SDL_Texture* background;
	SDL_Texture* char1;
	SDL_Texture* char2;
	SDL_Texture* char3;
	SDL_Texture* char4;

	SDL_Rect rect1 = { 0,100,320,600 };
	SDL_Rect rect2 = { 320,100,320,600 };
	SDL_Rect rect3 = { 640,100,320,600 };
	SDL_Rect rect4 = { 960,100,320,600 };

	bool isHover1 = false;
	bool isHover2 = false;
	bool isHover3 = false;
	bool isHover4 = false;

};

#endif // __SCENESELECTION_H__