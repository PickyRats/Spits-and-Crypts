#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiControlCheckBox.h"
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

	void SkillTree();

	bool playerDeadHud = false;
	bool spacePressed = false;
	bool onSettings = false;
	int currentId = 0;
	bool abilityTree = false;
private:


	//PUNTOS

	SDL_Texture* vidas;
	SDL_Texture* points;
	SDL_Texture* Personaje1;
	SDL_Texture* Personaje2;
	SDL_Texture* Selectorazul;
	SDL_Texture* Selectornaranja;
	SDL_Texture* Opacity;
	SDL_Texture* Cuadrojugador;
	SDL_Texture* numeros;

	//SDL_Rect rect = { 0, 0, 9, 51 };
	SDL_Rect lifeRects[11] = { {0, 0, 9, 51}, { 9, 0, 9, 51}, { 18, 0, 9, 51}, { 27, 0, 9, 51}, { 36, 0, 9, 51}, { 45, 0, 9, 51}, { 54, 0, 9, 51}, { 63, 0, 9, 51}, { 72, 0, 9, 51}, { 81, 0, 9, 51}, {90, 0, 9 ,51} };
	SDL_Rect pointsRects[6] = { {0, 0, 82, 121}, {82, 0, 82, 121}, {164, 0, 82, 121}, {246, 0, 82, 121}, {328, 0, 82, 121}, {410, 0, 82, 121} };
	SDL_Rect numerosRects[9] = { {0, 0, 37, 37}, {37, 0, 37, 37}, {74, 0, 37, 37}, {111, 0, 37, 37}, {148, 0, 37, 37}, {185, 0, 37, 37}, {222, 0, 37, 37}, {259, 0, 37, 37}, {296, 0, 37, 37} };

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
	SDL_Texture* settingsControlsButtonNormal;
	SDL_Texture* settingsControlsButtonHover;

	SDL_Texture* settingsAudioButtonNormal;
	SDL_Texture* settingsAudioButtonHover;

	SDL_Texture* settingsOptionsButtonNormal;
	SDL_Texture* settingsOptionsButtonHover;

	SDL_Texture* settingsAudioPanel;
	SDL_Texture* settingsControls;
	SDL_Texture* settingsOptionsPanel;

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
	GuiControlButton* settingsOptionsButton;
	GuiControlButton* settingsFxButton;
	GuiControlButton* settingsFullScreenButton;
	GuiControlCheckBox* settingsVSyncButton;
	GuiControlButton* settingsControlsButton;
	GuiControlButton* settingsAudioButton;

	//HabilityTree

	SDL_Texture* Habilitytree;

	SDL_Texture* Talent_1;
	SDL_Texture* Talent_2;
	SDL_Texture* Talent_3;
	SDL_Texture* Talent_4;
	SDL_Texture* Talent_5;


	SDL_Texture* Unlocked_1;
	SDL_Texture* Unlocked_2;
	SDL_Texture* Unlockable_1;
	SDL_Texture* Unlockable_2;
	SDL_Texture* Buyable_1;
	SDL_Texture* Buyable_2;

	//Talent 1
	bool talent1selected = true;

	//Talent 2
	bool talent2selected = false;
	bool talent2locked = false;

	//Talent 3
	bool talent3selected = false;
	bool talent3locked = false;

	//Talent 4
	bool talent4selected = false;
	bool talent4locked = false;

	//Talent 5
	bool talent5selected = false;
	bool talent5locked = false;

	//Talent 6
	bool talent6selected = false;
	bool talent6locked = false;

	//Talent 7
	bool talent7selected = false;
	bool talent7locked = false;



	bool onSettingsControls = false;
	bool onSettingsAudio = true;
	bool onSettingsOptions = false;
	bool buttonsActivated = false;
};

#endif // __HUD_H__