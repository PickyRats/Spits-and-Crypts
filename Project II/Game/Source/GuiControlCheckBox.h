#ifndef __GUICONTROLCHECKBOX_H__
#define __GUICONTROLCHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlCheckBox : public GuiControl
{

public:

	GuiControlCheckBox(uint32 id, SDL_Rect bounds, SDL_Texture* textureDisabled, SDL_Texture* textureNormal, SDL_Texture* textureFocused, SDL_Texture* texturePressed);
	virtual ~GuiControlCheckBox();

	// Called each loop iteration
	bool Update(float dt);
	bool pressed = false;

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

#endif // __GUICONTROLCHECKBOX_H__