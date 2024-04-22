#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(uint32 id, SDL_Rect bounds, SDL_Texture* textureDisabled, SDL_Texture* textureNormal, SDL_Texture* textureFocused, SDL_Texture* texturePressed, SDL_Rect sliderBounds);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	int sliderPosition = 0;
	SDL_Rect sliderBounds;

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* textureDisabled;
	SDL_Texture* textureNormal;
	SDL_Texture* textureFocused; 
	SDL_Texture* texturePressed;
	
	int initialMouseX = 0;
	
};

#endif // __GUICONTROLSLIDER_H__