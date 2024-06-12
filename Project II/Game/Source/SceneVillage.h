#ifndef __SCENEVILLAGE_H__
#define __SCENEVILLAGE_H__

#include "Module.h"
#include "Player.h"
#include "Npcs.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SceneMenu.h"
#include <vector>

struct SDL_Texture;

class SceneVillage : public Module
{
public:

	SceneVillage(bool enabled = true);

	// Destructor
	virtual ~SceneVillage();

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

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void OnExitCollision(PhysBody* bodyA, PhysBody* bodyB);

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	GuiControlButton* gcButtom;

	bool pause = false;
	bool isTorchActive = false;
	bool checkPoint = false;
	iPoint lastTorchPos = { 0, 0 };
	bool notUseChechPoint = false;
	bool win = false;
	int sceneNum = 1;
	iPoint spawnPosition = { 0, 0 };
	bool piedraHecha = false;
	PhysBody* piedra = nullptr;
	PhysBody* sensorPiedra = nullptr;
	bool wasStartPressed = false;
private:
	SDL_Texture* backgroundTexture;
	SDL_Texture* backgroundTexture2;
	SDL_Texture* aldea;
	SDL_Texture* piedraTexture;
	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	bool cutsceneStarted = false;

	float cameraSmoothingFactor = 0.04f;

	int levelWidth = 90 * 64;
	int levelHeight = 12 * 64;


	int playerX, playerY, cameraX, cameraY;
	bool changingLevel = false;
	bool isLoading = false;

	bool movement;


	int VillageMusic;
};

#endif // __SCENE_H__