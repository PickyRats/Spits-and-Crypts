#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneCombat.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneCombat::SceneCombat(bool enabled) : Module(enabled)
{
	name.Create("sceneCombat");
}

// Destructor
SceneCombat::~SceneCombat()
{}

pugi::xml_node configNodeCombat;

// Called before render is available
bool SceneCombat::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	int i = 0;
	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		enemy[i] = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy[i]->parameters = itemNode;
		i++;
	}

	configNodeCombat = config;
	return ret;
}

// Called before the first frame
bool SceneCombat::Start()
{
	if (configNodeCombat.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNodeCombat.child("map").attribute("name").as_string();
		app->map->path = configNodeCombat.child("map").attribute("path").as_string();
	}
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();

	app->map->player->isCombat = true;
	app->map->player2->isCombat = true;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	tileTexture = app->tex->Load("Assets/Textures/tile.png");
	tileEnemyTexture = app->tex->Load("Assets/Textures/tile_enemy.png");
	selectedTileTexture = app->tex->Load("Assets/Textures/selected_tile.png");
	cursorTexture = app->tex->Load("Assets/Textures/selection_cursor.png");

	tilePosition = { 64, 576 };
	app->map->pathfinding->CreatePath(app->map->WorldToMap(app->map->player->position.x, app->map->player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));

	for (int i = 0; i < 2; i++)
	{
		enemies[i] = enemy[i];
	}
	players[0] = app->map->player;
	players[1] = app->map->player2;
	app->map->player2->isVisible = true;
	app->map->player2->CreateBody();
	return true;
}

// Called each loop iteration
bool SceneCombat::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneCombat::Update(float dt)
{
	//app->render->DrawTexture(backgroundTexture2, 0, 0, &bg, SDL_FLIP_NONE, 0.0f);

	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	SetCameraPosition(0, 0);

	ClampCamera();

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) ChangeTurn();

	if (isPlayerTurn)
	{
		maxTiles = 12;
		UpdatePath();

		SelectTiles();
	}
	else
	{
		maxTiles = 50;

		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			// Draw the path
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(tileEnemyTexture, pos.x, pos.y);

			// Set the direction of the tiles
			if (pos.x > tiles[i - 1].position.x) tiles[i] = { pos, 1 };
			if (pos.x < tiles[i - 1].position.x) tiles[i] = { pos, 2 };
			if (pos.y < tiles[i - 1].position.y) tiles[i] = { pos, 3 };
			if (pos.y > tiles[i - 1].position.y) tiles[i] = { pos, 4 };
		}

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			tilesCount = path->Count();
			app->map->pathfinding->ClearLastPath();
			int nearestPlayer = 0;
			int playerDistance[2];
			for (int i = 0; i < 2; i++)
			{
				if (!players[i]->isDead)
				{
					playerDistance[i] = app->map->pathfinding->CreatePath(app->map->WorldToMap(enemies[currentEnemyIndex]->position.x, enemies[currentEnemyIndex]->position.y), app->map->WorldToMap(players[i]->position.x, players[i]->position.y));
				}
				else playerDistance[i] = 1000;
			}
			if (playerDistance[0] < playerDistance[1]) nearestPlayer = 0;
			else nearestPlayer = 1;
			int destinationTiles = app->map->pathfinding->CreatePath(app->map->WorldToMap(enemies[currentEnemyIndex]->position.x, enemies[currentEnemyIndex]->position.y), app->map->WorldToMap(players[nearestPlayer]->position.x, players[nearestPlayer]->position.y));
			int movementTiles = destinationTiles - enemies[currentEnemyIndex]->attackRange - 1;

			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				// Draw the path
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(tileEnemyTexture, pos.x, pos.y);

				// Set the direction of the tiles
				if (pos.x > tiles[i - 1].position.x) tiles[i] = { pos, 1 };
				if (pos.x < tiles[i - 1].position.x) tiles[i] = { pos, 2 };
				if (pos.y < tiles[i - 1].position.y) tiles[i] = { pos, 3 };
				if (pos.y > tiles[i - 1].position.y) tiles[i] = { pos, 4 };
			}

			tilesCount = path->Count();

			// Add movement tiles if the enemy can't reach the player
			for (int i = enemies[currentEnemyIndex]->attackRange + 1; i > 0; --i)
			{
				if (tiles[tilesCount - i].position.y != players[currentPlayerIndex]->position.y) movementTiles++;
			}

			if (enemies[currentEnemyIndex]->attackRange >= destinationTiles - 1) EnemyAttack();
			else if (enemies[currentEnemyIndex]->currentPoints > 0 && movementTiles <= enemies[currentEnemyIndex]->currentPoints)
			{
				for (int i = enemies[currentEnemyIndex]->currentPoints; i < tilesCount; ++i)
				{
					tiles[i + 1] = { iPoint(0,0), 0 };
				}
				tilesCount = movementTiles;
				enemyCanAttack = true;
				MovePlayer(enemies[currentEnemyIndex]);
				printf("Enemy is moving %d tiles parcial\n", movementTiles);
				enemies[currentEnemyIndex]->currentPoints -= movementTiles;
			}
			else if (enemies[currentEnemyIndex]->currentPoints > 0)
			{
				for (int i = enemies[currentEnemyIndex]->currentPoints; i < tilesCount; ++i)
				{
					tiles[i + 1] = { iPoint(0,0), 0 };
				}
				tilesCount = enemies[currentEnemyIndex]->currentPoints;
				MovePlayer(enemies[currentEnemyIndex]);
				printf("Enemy is moving %d tiles total\n", enemies[currentEnemyIndex]->currentPoints);
				enemies[currentEnemyIndex]->currentPoints = 0;
			}
			else printf("No points\n");
		}
		
	}

	app->render->camera.x += (-cameraX - app->render->camera.x) * cameraSmoothingFactor;
	app->render->camera.y += (-cameraY - app->render->camera.y) * cameraSmoothingFactor;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !isMoving)
	{
		players[currentPlayerIndex]->currentPoints -= tilesCount;
		MovePlayer(players[currentPlayerIndex]);
	}

	if (isPlayerTurn && isMoving) MovePlayer(players[currentPlayerIndex]);
	else if (!isPlayerTurn && isMoving) MovePlayer(enemies[currentEnemyIndex]);

	return true;
}



// Called each loop iteration
bool SceneCombat::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		pause = !pause;
		app->hud->onSettings = false;
	}

	return ret;
}

// Called before quitting
bool SceneCombat::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void SceneCombat::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneCombat::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;
	
}

bool SceneCombat::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneCombat::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneCombat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}

void SceneCombat::MovePlayer(Entity* entity)
{
	iPoint* currentPosition = &entity->position;

	// Set the destination position
	if (!isMoving)
	{
		if (tiles[currentTile].direction == 1)
		{
			destinationPosition = { currentPosition->x + 64, currentPosition->y };
			movingDirection = 1;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 2)
		{
			destinationPosition = { currentPosition->x - 64, currentPosition->y };
			movingDirection = 2;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 3)
		{
			destinationPosition = { currentPosition->x, currentPosition->y - 64 - 64 - 64 };
			movingDirection = 3;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 4)
		{
			destinationPosition = { currentPosition->x, currentPosition->y + 64 + 64 + 64 };
			movingDirection = 4;
			isMoving = true;
		}
	}

	// Check if the player has reached the destination
	if (currentTile <= tilesCount)
	{
		if (currentPosition->x == destinationPosition.x && currentPosition->y == destinationPosition.y)
		{
			movingDirection = 0;
			currentTile++;
			isMoving = false;
			MovePlayer(entity);
		}
	}
	else // Reset the path
	{
		isMoving = false;
		entity->currentPoints = entity->totalPoints;
		ChangeTurn();
	}

	// Move the player
	if (movingDirection == 1) currentPosition->x += 2;
	if (movingDirection == 2) currentPosition->x -= 2;
	if (movingDirection == 3) currentPosition->y -= 2;
	if (movingDirection == 4) currentPosition->y += 2;
}

void SceneCombat::UpdatePath()
{
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		// Draw the path
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		if (i == path->Count() - 1) app->render->DrawTexture(selectedTileTexture, pos.x, pos.y);
		else if (!isMoving || currentTile < i + 2) app->render->DrawTexture(tileTexture, pos.x, pos.y);

		// Set the direction of the tiles
		if (pos.x > tiles[i - 1].position.x) tiles[i] = { pos, 1 };
		if (pos.x < tiles[i - 1].position.x) tiles[i] = { pos, 2 };
		if (pos.y < tiles[i - 1].position.y) tiles[i] = { pos, 3 };
		if (pos.y > tiles[i - 1].position.y) tiles[i] = { pos, 4 };
	}

	tilesCount = path->Count();

	//Draw the cursor
	app->render->DrawTexture(cursorTexture, tilePosition.x, tilePosition.y);
}

void SceneCombat::SelectTiles()
{
	maxTiles = players[0]->currentPoints;
	Entity* player = players[currentPlayerIndex];
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && !isMoving)
	{

		tilePosition.x += 64;
		if (!IsTileOccupied())
		{
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && !isMoving)
	{
		tilePosition.x -= 64;
		if (!IsTileOccupied())
		{
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !isMoving)
	{
		tilePosition.y -= (64 * 3);
		if (!IsTileOccupied())
		{
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));

		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && !isMoving)
	{
		tilePosition.y += (64 * 3);
		if (!IsTileOccupied())
		{
			ResetTilesArray(100);
			app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		}
	}
}

void SceneCombat::ResetTilesArray(int max)
{
	for (int i = 1; i < max; i++)
	{
		tiles[max - i] = { iPoint(0,0), 0 };
	}
}

void SceneCombat::ChangeTurn()
{
	if (isPlayerTurn)
	{
		tiles[0] = { enemies[currentEnemyIndex]->position, 0 };
		ResetTilesArray(currentTile);
		tilesCount = 0;
		movingDirection = 0;
		currentTile = 1;
		tilePosition = tiles[0].position;
		app->map->pathfinding->ClearLastPath();
		app->map->pathfinding->CreatePath(app->map->WorldToMap(enemies[currentEnemyIndex]->position.x, enemies[currentEnemyIndex]->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		if (players[currentPlayerIndex + 1] != nullptr && !players[currentPlayerIndex + 1]->isDead) currentPlayerIndex++;
		else currentPlayerIndex = 0;
	}
	else
	{
		if (enemyCanAttack) EnemyAttack();
		tiles[0] = { players[currentPlayerIndex]->position, 0};
		ResetTilesArray(currentTile);
		tilesCount = 0;
		movingDirection = 0;
		currentTile = 1;
		tilePosition = tiles[0].position;
		app->map->pathfinding->ClearLastPath();
		app->map->pathfinding->CreatePath(app->map->WorldToMap(players[currentPlayerIndex]->position.x, players[currentPlayerIndex]->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
		if (enemies[currentEnemyIndex + 1] != nullptr && !enemies[currentEnemyIndex + 1]->isDead) currentEnemyIndex++;
		else currentEnemyIndex = 0;
	}

	isPlayerTurn = !isPlayerTurn;
}

void SceneCombat::EnemyAttack()
{
	enemyCanAttack = false;
	players[currentPlayerIndex]->health -= enemies[currentEnemyIndex]->attackDamage;
	printf("Enemy is attacking player life: %d \n", players[currentPlayerIndex]->health);
}

bool SceneCombat::IsTileOccupied()
{
	for (int i = 0; i < 2; i++)
	{
		if (players[i]->position.x == tilePosition.x && players[i]->position.y == tilePosition.y) return true;
	}
	for (int i = 0; i < 2; i++)
	{
		if (enemies[i]->position.x == tilePosition.x && enemies[i]->position.y == tilePosition.y) return true;
	}
	return false;
}