#ifndef __SCENECOMBAT_H__
#define __SCENECOMBAT_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "SceneMenu.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include <vector>

struct SDL_Texture;

struct Tile
{
	iPoint position = { 0, 0 };
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

	void EndCombat();

	// Enemy attack
	void EnemyAttack();

	void ResetPlayerTurn();

	void ResetEnemyTurn();

	// Verify if the tile is occupied
	bool IsTileOccupied();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void SetCameraPosition(int x, int y);

	void ClampCamera();

	// Move the player following the path generated
	void MovePlayer(Entity* entity);

	// Update the path to follow
	void UpdatePath();

	// Select the tile where the player is going to move
	void SelectTiles();

	void ResetTilesArray(int max);

	// Change the turn
	void ChangeTurn();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	GuiControlButton* gcButtom;

	bool pause = false;
	bool win = false;

	bool isMoving = false;

	SDL_Texture* cursorTexture;
	iPoint tilePosition;
	iPoint currentPosition;
	iPoint destinationPosition;
	int movingDirection = 0; //0 = none, 1 = right, 2 = left, 3 = up, 4 = down

	Tile tiles[100];
	int currentTile = 1;
	int tilesCount = 0;
	int maxTiles = 12;

	Enemy* enemy[2];

	Entity* currentEntity = nullptr;

	int currentPlayerIndex = 0;
	int currentEnemyIndex = 0;

	Entity* players[5];
	Entity* enemies[5];

private:
	SDL_Texture* backgroundTexture;
	SDL_Texture* backgroundTexture2;
	SDL_Texture* floor1background;
	SDL_Rect bg;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	float cameraSmoothingFactor = 0.04f;

	int levelWidth = 107 * 32;
	int levelHeight = 50 * 32;

	int playerX, playerY, cameraX, cameraY;
	bool changingLevel = false;
	bool isLoading = false;
	SDL_Texture* tileTexture;
	SDL_Texture* tileEnemyTexture;
	SDL_Texture* selectedTileTexture;
	bool isPlayerTurn = true;

	bool playerCanAttack = false;
	bool enemyCanAttack = false;

	int enemyAttackIndex = 0;

	bool startEnemyTurn = false;
	int nearestPlayer = 0;
};

#endif // __SCENECOMBAT_H__