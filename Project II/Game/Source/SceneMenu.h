#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:

	SceneMenu(bool enabled = true);

	virtual ~SceneMenu();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	int buttonFxHover;
	int buttonFxClick;
	bool fxHoverPlayed = false;
	bool fxClickPlayed = false;
	bool fullScreen = false;
	bool vSync = true;

	GuiControlButton* playButton;
	GuiControlButton* continueButton;
	GuiControlButton* settingsButton;
	GuiControlButton* creditsButton;
	GuiControlButton* exitButtonMenu;
	GuiControlButton* exitButtonSettings;
	GuiControlButton* exitButtonCredits;
	GuiControlButton* returnButtonSettings;
	GuiControlButton* returnButtonCredits;

	bool isProgressSaved = false;
private:

	SDL_Texture* background;
	SDL_Texture* menu;

	SDL_Texture* playNormal;
	SDL_Texture* playHover;
	SDL_Texture* playClick;

	SDL_Texture* continueDisabled;
	SDL_Texture* continueNormal;
	SDL_Texture* continueHover;
	SDL_Texture* continueClick;

	SDL_Texture* settingsNormal;
	SDL_Texture* settingsHover;
	SDL_Texture* settingsClick;	

	SDL_Texture* creditsNormal;
	SDL_Texture* creditsHover;
	SDL_Texture* creditsClick;

	SDL_Texture* exitNormal;
	SDL_Texture* exitHover;
	SDL_Texture* exitClick;

	SDL_Texture* returnNormal;
	SDL_Texture* returnHover;
	SDL_Texture* returnClick;

	SDL_Texture* settings;
	SDL_Texture* credits;

	SDL_Texture* settingsTick;

	SDL_Texture* settingsSlider;

	SDL_Texture* settingsMusicNormal;

	SDL_Texture* settingsBoxNormal;
	SDL_Texture* settingsBoxHover;

	GuiControlButton* settingsReturnButton;
	GuiControlButton* settingsExitButton;
	GuiControlButton* settingsFullScreenButton;
	GuiControlButton* settingsVSyncButton;
	GuiControlButton* settingsMusicButton;
	GuiControlButton* settingsFxButton;

	GuiControlButton* creditsReturnButton;
	GuiControlButton* creditsExitButton;
	
	bool onMenu = true;
	bool onSettings = false;
	bool onCredits = false;

};

#endif // __SCENEMENU_H__