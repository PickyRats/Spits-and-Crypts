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

	tiles[0].position = { iPoint(96, 96) };
	tiles[0].isSelected = true;

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

	app->render->camera.x += (-cameraX - app->render->camera.x) * cameraSmoothingFactor;
	app->render->camera.y += (-cameraY - app->render->camera.y) * cameraSmoothingFactor;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	SelectTile();

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

void SceneCombat::SelectTile()
{
	bool isLadder = app->map->pathfinding->IsLadder(iPoint{ tiles[tileIndex].position.x / 96, tiles[tileIndex].position.y / 96 });
	bool rightIsLadder = app->map->pathfinding->IsLadder(iPoint{ (tiles[tileIndex].position.x + 96) / 96, tiles[tileIndex].position.y / 96 });
	bool rightIsWalkable = app->map->pathfinding->IsWalkable(iPoint{ (tiles[tileIndex].position.x + 96 ) / 96 , tiles[tileIndex].position.y / 96 });
	bool leftIsLadder = app->map->pathfinding->IsLadder(iPoint{ (tiles[tileIndex].position.x - 96) / 96, tiles[tileIndex].position.y / 96 });
	bool leftIsWalkable = app->map->pathfinding->IsWalkable(iPoint{ (tiles[tileIndex].position.x - 96) / 96 , tiles[tileIndex].position.y / 96 });

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if (tileIndex == 0) movingDirection = 0;
		if ((rightIsWalkable ||rightIsLadder) && movingDirection != 11 && (movingDirection != 12 && !tiles[tileIndex + 1].isSelected) || (movingDirection != 13 && !tiles[tileIndex - 1].isSelected) || isLadder)
		{
			tileIndex++;

			tiles[tileIndex].isSelected = true;
			tiles[tileIndex].position = { iPoint(tiles[tileIndex - 1].position.x + 96, tiles[tileIndex - 1].position.y) };
			movingDirection = 10;
		}
		else
		{
			tiles[tileIndex].isSelected = false;
			tileIndex--;
			movingDirection = 11;
		}

	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		if (tileIndex == 0) movingDirection = 0;
		if ((leftIsWalkable || leftIsLadder) && movingDirection != 13 && (movingDirection != 10 && !tiles[tileIndex + 1].isSelected) || (movingDirection != 11 && !tiles[tileIndex - 1].isSelected) || isLadder)
		{
			tileIndex++;

			tiles[tileIndex].isSelected = true;
			tiles[tileIndex].position = { iPoint(tiles[tileIndex - 1].position.x - 96, tiles[tileIndex - 1].position.y) };
			movingDirection = 12;
		}
		else
		{

			tiles[tileIndex].isSelected = false;
			tileIndex--;
			movingDirection = 13;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if (!hasClimbedUp && isLadder)
		{
			if (movingDirection == 0 || movingDirection == 10 || movingDirection == 11 || movingDirection == 12 || movingDirection == 13 || (movingDirection == 4 && hasClimbedDown))
			{
				tileIndex++;

				tiles[tileIndex].isSelected = true;
				tiles[tileIndex].position = { iPoint(tiles[tileIndex - 1].position.x, tiles[tileIndex - 1].position.y - 96 - 96) };
			}
			else
			{
				tiles[tileIndex].isSelected = false;
				tileIndex--;
			}
			movingDirection = 3;
			hasClimbedUp = true;
			hasClimbedDown = false;
			//tiles[tileIndex].isLadder = true;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (!hasClimbedDown && isLadder)
		{
			if (movingDirection == 0 || movingDirection == 10 || movingDirection == 11 || movingDirection == 12 || movingDirection == 13 || (movingDirection == 3 && !hasClimbedUp))
			{
				tileIndex++;

				tiles[tileIndex].isSelected = true;
				tiles[tileIndex].position = { iPoint(tiles[tileIndex - 1].position.x, tiles[tileIndex - 1].position.y + 96 + 96) };
			}
			else
			{
				tiles[tileIndex].isSelected = false;
				tileIndex--;
			}
			movingDirection = 4;
			hasClimbedDown = true;
			hasClimbedUp = false;
			//tiles[tileIndex].isLadder = true;
		}
	}

	for (int i = 0; i <= tileIndex; i++)
	{
		if (i == tileIndex && tiles[i].isSelected) app->render->DrawTexture(selectedTileTexture, tiles[i].position.x, tiles[i].position.y);
		else if (tiles[i].isSelected) app->render->DrawTexture(tileTexture, tiles[i].position.x, tiles[i].position.y);
		//printf("\n Tile x: %d Tile y: %d selected: %d", tiles[i].position.x, tiles[i].position.y, tiles[i].isSelected);
	}
	printf("\r Tile index: %d Moving Direction: %d Up: %d Down: %d positionX: %d positionY: %d ladder: %d", tileIndex, movingDirection, hasClimbedUp, hasClimbedDown, tiles[tileIndex].position.x, tiles[tileIndex].position.y, isLadder);
}
