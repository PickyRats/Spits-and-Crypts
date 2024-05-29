#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiControlCheckBox.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlSlider.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:

	SceneMenu(bool enabled = true);

	virtual ~SceneMenu();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool SaveState(pugi::xml_node node);

	bool CleanUp();
	uint FxButton1;
	uint FxButton2;

	bool fxHoverPlayed = false;
	bool fxClickPlayed = false;
	bool fullScreen = false;
	bool vSync = true;

	bool wasDownPressed = false;
	bool wasUpPressed = false;
	bool wasR1Pressed = false;
	bool wasL1Pressed = false;

	int currentId= 1;
	int percentageMusic;
	GuiControlButton* startButton;
	GuiControlButton* loadGameButton;
	GuiControlButton* settingsButton;
	GuiControlButton* creditsButton;
	GuiControlButton* exitButton;
	GuiControlButton* exitButtonSettings;
	GuiControlButton* exitButtonCredits;
	GuiControlButton* returnButtonSettings;
	GuiControlButton* returnButtonCredits;

	bool isProgressSaved = false;
private:

	SDL_Texture* background;
	SDL_Texture* background2;
	SDL_Texture* menu;

	SDL_Texture* playNormal;
	SDL_Texture* playHover;
	SDL_Texture* playClick;

	SDL_Texture* loadGameDisabled;
	SDL_Texture* loadGameNormal;
	SDL_Texture* loadGameHover;
	SDL_Texture* loadGameClick;

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
	SDL_Texture* controlsHint;
	SDL_Texture* credits;

	SDL_Texture* settingsTick;
	SDL_Texture* settingsSlider;
	SDL_Texture* settingsAudioPanel;
	SDL_Texture* settingsOptionsPanel;
	SDL_Texture* settingsOptionsButtonNormal;
	SDL_Texture* settingsOptionsButtonHover;
	SDL_Texture* settingsAudioButtonNormal;
	SDL_Texture* settingsAudioButtonHover;
	SDL_Texture* settingsControlsButtonNormal;
	SDL_Texture* settingsControlsButtonHover;
	SDL_Texture* settingsMusicNormal;
	SDL_Texture* settingsBoxNormal;
	SDL_Texture* settingsBoxHover;
	SDL_Texture* settingsControls;

	GuiControlButton* settingsExitButton;
	GuiControlButton* settingsOptionsButton;
	GuiControlButton* settingsAudioButton;
	GuiControlButton* settingsControlsButton;
	GuiControlButton* settingsFullScreenButton;
	GuiControlCheckBox* settingsVSyncButton;
	GuiControlSlider* settingsMusicButton;
	GuiControlSlider* settingsFxButton;
	

	GuiControlButton* creditsExitButton;

	int x = 1280;
	double a = 50;

	//add music to the menu
	uint MenuMusic;
	
	bool onMenu = true;
	bool onSettings = false;
	bool onSettingsControls = false;
	bool onSettingsAudio = true;
	bool onSettingsOptions = false;
	bool buttonsActivated = false;
	bool onCredits = false;

};

#endif // __SCENEMENU_H__