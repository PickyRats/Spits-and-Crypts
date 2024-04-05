#ifndef __SCENECOMBAT_H__
#define __SCENECOMBAT_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include <vector>

struct SDL_Texture;

struct Tile
{
	iPoint position;
	bool isSelected = false;
	int direction = 0; //0 = none, 1 = right, 2 = left, 3 = up, 4 = down
};

class SceneCombat : public Module
{
public:

	SceneCombat(bool enabled = true);

	// Destructor
	virtual ~SceneCombat();

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

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	void SelectTile();

	void MovePlayer();

	int GetTileIndex(iPoint tilePosition);

	bool IsSelected(iPoint tilePosition);

	bool IsAdjacent(iPoint tilePosition);

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	Player* player;	
	GuiControlButton* gcButtom;

	bool pause = false;
	bool isTorchActive = false;
	bool checkPoint = false;
	iPoint lastTorchPos = { 0, 0 };
	bool notUseChechPoint = false;
	bool win = false;

	int tileIndex = 0;
	bool hasClimbedUp = false;
	bool hasClimbedDown = false;

	iPoint currentPosition = { 0, 0 };
	iPoint initialPosition = { 0, 0 };
	iPoint destinationPosition = { 0, 0 };
	int currentTile = 1;
	bool isMoving = false;
	int movingDirection = 0;

private:
	SDL_Texture* backgroundTexture;
	SDL_Texture* backgroundTexture2;
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
	SDL_Texture* tileTexture;
	SDL_Texture* selectedTileTexture;
	Tile tiles[100];
};

#endif // __SCENECOMBAT_H__