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
	name.Create("scene");
}

// Destructor
SceneCombat::~SceneCombat()
{}

// Called before render is available
bool SceneCombat::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}

	return ret;
}

// Called before the first frame
bool SceneCombat::Start()
{
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	tileTexture = app->tex->Load("Assets/Textures/tile.png");
	selectedTileTexture = app->tex->Load("Assets/Textures/selected_tile.png");
	cursorTexture = app->tex->Load("Assets/Textures/selection_cursor.png");

	tilePosition = { 96, 96 };
	app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
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
	app->render->DrawTexture(backgroundTexture2, 0, 0, &bg, SDL_FLIP_NONE, 0.0f);

	playerX = player->position.x;
	playerY = player->position.y;

	SetCameraPosition(0, 0);

	ClampCamera();

	UpdatePath();

	SelectTiles();

	app->render->camera.x += (-cameraX - app->render->camera.x) * cameraSmoothingFactor;
	app->render->camera.y += (-cameraY - app->render->camera.y) * cameraSmoothingFactor;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !isMoving) MovePlayer();
	if (isMoving) MovePlayer();

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

void SceneCombat::MovePlayer()
{
	currentPosition = player->position;

	// Set the destination position
	if (!isMoving)
	{
		if (tiles[currentTile].direction == 1)
		{
			destinationPosition = { player->position.x + 96, player->position.y };
			movingDirection = 1;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 2)
		{
			destinationPosition = { player->position.x - 96, player->position.y };
			movingDirection = 2;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 3)
		{
			destinationPosition = { player->position.x, player->position.y - 96 - 96 };
			movingDirection = 3;
			isMoving = true;
		}
		if (tiles[currentTile].direction == 4)
		{
			destinationPosition = { player->position.x, player->position.y + 96 + 96 };
			movingDirection = 4;
			isMoving = true;
		}
	}

	// Check if the player has reached the destination
	if (currentTile <= tilesCount)
	{
		if (currentPosition == destinationPosition)
		{
			movingDirection = 0;
			currentTile++;
			isMoving = false;
			MovePlayer();
		}
	}
	else // Reset the path
	{
		isMoving = false;
		tiles[0] = { tiles[tilesCount - 1].position, 0 };
		for (int i = 1; i < currentTile; i++)
		{
			tiles[currentTile - i] = { iPoint(0,0), 0 };
		}
		tilesCount = 0;
		movingDirection = 0;
		currentTile = 1;
		tilePosition = tiles[0].position;
		app->map->pathfinding->ClearLastPath();
		app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	}

	// Move the player
	if (movingDirection == 1) player->position.x += 2;
	if (movingDirection == 2) player->position.x -= 2;
	if (movingDirection == 3) player->position.y -= 2;
	if (movingDirection == 4) player->position.y += 2;
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
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && !isMoving)
	{
		tilePosition.x += 96;
		app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && !isMoving)
	{
		tilePosition.x -= 96;
		app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !isMoving)
	{
		tilePosition.y -= (96 * 2);
		app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && !isMoving)
	{
		tilePosition.y += (96 * 2);
		app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	}
}