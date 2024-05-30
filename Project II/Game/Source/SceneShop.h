#ifndef __SCENESHOP_H__
#define __SCENESHOP_H__

#include "Module.h"
#include "Player.h"
#include "SceneMenu.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include <vector>

struct SDL_Texture;

class SceneShop : public Module
{
public:

	SceneShop(bool enabled = true);

	// Destructor
	virtual ~SceneShop();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

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

	void CreateEntities(pugi::xml_node& config, const char* nodeName, EntityType entityType);

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	GuiControlButton* gcButtom;

	bool pause = false;
	bool isTorchActive = false;
	bool checkPoint = false;
	iPoint lastTorchPos = { 0, 0 };
	bool notUseChechPoint = false;
	bool win = false;
	int sceneNum = 3;

private:
	SDL_Texture* shop;
	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	float cameraSmoothingFactor = 0.04f;

	int levelWidth = 107 * 32;
	int levelHeight = 50 * 32;

	int level2Width = 140 * 32;
	int level2Height = 45 * 32;

	int playerX, playerY, cameraX, cameraY;
	bool changingLevel = false;
	bool isLoading = false;

};

#endif // __SCENE_H__
