#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(uint32 id, SDL_Rect bounds, SDL_Texture* textureDisabled, SDL_Texture* textureNormal, SDL_Texture* textureFocused, SDL_Texture* texturePressed);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* textureDisabled;
	SDL_Texture* textureNormal;
	SDL_Texture* textureFocused; 
	SDL_Texture* texturePressed;
};

#endif // __GUICONTROLBUTTON_H__