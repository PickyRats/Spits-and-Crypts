#ifndef __SCENECHOZA_H__
#define __SCENECHOZA_H__

#include "Module.h"
#include "Player.h"
#include "Npcs.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SceneMenu.h"
#include <vector>

struct SDL_Texture;
class SceneChoza : public Module
{
public:

	SceneChoza(bool enabled = true);

	// Destructor
	virtual ~SceneChoza();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	GuiControlButton* gcButtom;
	bool wasStartPressed = false;
	bool pause = false;
	bool isTorchActive = false;
	bool checkPoint = false;
	iPoint lastTorchPos = { 0, 0 };
	bool notUseChechPoint = false;
	bool win = false;
	int sceneNum = 6;

private:
	SDL_Texture* backgroundTexture;
	SDL_Texture* backgroundTexture2;
	SDL_Texture* aldea;
	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	float cameraSmoothingFactor = 0.04f;

	int levelWidth = 45 * 64;
	int levelHeight = 12 * 64;

	int playerX, playerY, cameraX, cameraY;
	bool changingLevel = false;
	bool isLoading = false;

	int VillageMusic;
};

#endif // __SCENECHOZA_H__