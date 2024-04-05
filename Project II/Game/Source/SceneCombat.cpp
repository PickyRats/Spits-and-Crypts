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

	app->map->pathfinding->CreatePath(app->map->WorldToMap(player->position.x, player->position.y), app->map->WorldToMap(tilePosition.x, tilePosition.y));
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		if (i == path->Count() - 1) app->render->DrawTexture(selectedTileTexture, pos.x, pos.y);
		else app->render->DrawTexture(tileTexture, pos.x, pos.y);
	}

	app->render->DrawTexture(cursorTexture, tilePosition.x, tilePosition.y);

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) tilePosition.x += 96;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) tilePosition.x -= 96;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) tilePosition.y -= (96 * 2);
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) tilePosition.y += (96 * 2);

	app->render->camera.x += (-cameraX - app->render->camera.x) * cameraSmoothingFactor;
	app->render->camera.y += (-cameraY - app->render->camera.y) * cameraSmoothingFactor;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	//SelectTile();

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) isMoving = true;
	if (isMoving)
	{
		iPoint nextTilePath;
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		if (path->Count() > 1) {
			nextTilePath = { path->At(1)->x, path->At(1)->y };
			Move(app->map->WorldToMap(player->position.x, player->position.y), nextTilePath);
		}
		else isMoving = false;
	}

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

void SceneCombat::Move(const iPoint& origin, const iPoint& destination) 
{
	float xDiff = destination.x - origin.x;
	float yDiff = destination.y - origin.y;

	player->position.x += (xDiff < 0) ? -2 : (xDiff > 0) ? 2 : 0;
	player->position.y += (yDiff < 0) ? -2 : (yDiff > 0) ? 2 : 0;
}
