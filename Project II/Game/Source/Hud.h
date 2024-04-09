#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

struct SDL_Texture;

class Hud : public Module
{
public:

	Hud(bool enabled = true);

	virtual ~Hud();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void DrawTimer();

	bool playerDeadHud = false;
	bool spacePressed = false;
	bool onSettings = false;
private:

	//HUD EXPLORACION
	SDL_Texture* character1;
	SDL_Texture* fuego;
	SDL_Texture* life1;
	SDL_Texture* life2;
	SDL_Texture* PP;
	SDL_Texture* sword1;
	SDL_Texture* sword2;
	SDL_Texture* number14;
	SDL_Texture* L3Move;
	SDL_Texture* execute;
	SDL_Texture* Xaccept;
	SDL_Texture* playerverd;
	SDL_Texture* enemyrojo;
	SDL_Texture* L1;
	SDL_Texture* R1;




	SDL_Texture* exitNormal;
	SDL_Texture* exitHover;
	SDL_Texture* exitClick;

	SDL_Texture* returnNormal;
	SDL_Texture* returnHover;
	SDL_Texture* returnClick;

	SDL_Texture* pause;

	SDL_Texture* resumeNormal;
	SDL_Texture* resumeHover;
	SDL_Texture* resumeClick;

	SDL_Texture* settingsNormal;
	SDL_Texture* settingsHover;
	SDL_Texture* settingsClick;

	SDL_Texture* backToTitleNormal;
	SDL_Texture* backToTitleHover;
	SDL_Texture* backToTitleClick;

	//Settings
	SDL_Texture* settingsExitNormal;
	SDL_Texture* settingsExitHover;
	SDL_Texture* settingsExitClick;

	SDL_Texture* settingsReturnNormal;
	SDL_Texture* settingsReturnHover;
	SDL_Texture* settingsReturnClick;

	SDL_Texture* settingsMusicNormal;
	SDL_Texture* settingsMusicHover;
	SDL_Texture* settingsMusicClick;

	SDL_Texture* settingsFxNormal;
	SDL_Texture* settingsFxHover;
	SDL_Texture* settingsFxClick;

	SDL_Texture* settingsFullScreenNormal;
	SDL_Texture* settingsFullScreenHover;
	SDL_Texture* settingsFullScreenClick;

	SDL_Texture* settingsVSyncNormal;
	SDL_Texture* settingsVSyncHover;
	SDL_Texture* settingsVSyncClick;

	SDL_Texture* settingsTick;

	SDL_Texture* settingsSlider;

	SDL_Texture* settingsBoxNormal;
	SDL_Texture* settingsBoxHover;

	SDL_Texture* settings;


	GuiControlButton* exitButton;
	GuiControlButton* returnButton;
	GuiControlButton* resumeButton;
	GuiControlButton* settingsButton;
	GuiControlButton* backToTitleButton;

	GuiControlButton* settingsExitButton;
	GuiControlButton* settingsReturnButton;
	GuiControlButton* settingsMusicButton;
	GuiControlButton* settingsFxButton;
	GuiControlButton* settingsFullScreenButton;
	GuiControlButton* settingsVSyncButton;
};

#endif // __HUD_H__