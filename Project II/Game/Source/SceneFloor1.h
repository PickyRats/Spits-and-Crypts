#ifndef __SCENEFLOOR1_H__
#define __SCENEFLOOR1_H__

#include "Module.h"
#include "Player.h"
#include "Npcs.h"
#include "PiezasPuzle.h"
#include "GuiControl.h"
#include "SceneMenu.h"
#include "GuiControlButton.h"
#include "Puertas.h"
#include <vector>

struct SDL_Texture;

class SceneFloor1 : public Module
{
public:

	SceneFloor1(bool enabled = true);

	// Destructor
	virtual ~SceneFloor1();

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

	void CreateEntities(pugi::xml_node& config, const char* nodeName, EntityType entityType);

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	void DeleteWall();

	void DeleteWall2();

	void DeleteWall3();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	void PlayMusic();

	GuiControlButton* gcButtom;
	PhysBody* wall;
	PhysBody* wall2;
	PhysBody* wall3;
	bool pause = false;
	bool isTorchActive = false;
	bool checkPoint = false;
	bool canDelete = true;
	bool canDelete2 = true;
	bool canDelete3 = true;
	bool wasStartPressed = false;
	iPoint playerStartPosition = { 128,35*64 };
	bool notUseChechPoint = false;
	bool win = false;
	int sceneNum = 5;


	int levelWidth = 37 * 64;
	int levelHeight = 37 * 64;

	bool combatFinished = false;
	Npcs* paalaya;
	Puertas* puertas[2];
	
private:
	SDL_Texture* backgroundTexture;
	SDL_Texture* backgroundTexture2;
	SDL_Texture* floor1background;
	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	
	float cameraSmoothingFactor = 0.04f;
	
	int playerX, playerY, cameraX, cameraY;
	bool changingLevel = false;
	bool isLoading = false;

	int VillageMusic;
};

#endif // __SCENE_H__