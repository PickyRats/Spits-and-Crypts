#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiControlCheckBox.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

struct SDL_Texture;
struct Slot {
	iPoint position = { 0, 0 };
	SDL_Texture* texture = nullptr;
	bool isEmpty = true;
};

struct Item {
	const char* name = nullptr;
	const char* description = nullptr;
	int health = 0;
	int attack = 0;
	int price = 0;
	SDL_Texture* texture = nullptr;
};

class Hud : public Module
{
public:

	Hud(bool enabled = true);

	virtual ~Hud();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	void Inventory();

	void Shop();

	bool CleanUp();

	void DrawTimer();

	bool playerDeadHud = false;
	bool spacePressed = false;
	bool onSettings = false;
	int currentId = 0;

	Slot inventorySlots[3];
	Item items[3];

	Slot shopSlots[3];
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

	//Inventory

	SDL_Texture* inventoryTexture;
	SDL_Texture* inventoryItem1;
	SDL_Texture* inventoryItem2;
	SDL_Texture* inventoryItem3;
	bool inventory = false;

	//Shop
	SDL_Texture* shopTexture;
	bool shop = false;


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

	bool onSettingsControls = false;
	bool onSettingsAudio = true;
	bool onSettingsOptions = false;
	bool buttonsActivated = false;
};

#endif // __HUD_H__