#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneFloor1.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneFloor1::SceneFloor1(bool enabled) : Module(enabled)
{
	name.Create("sceneFloor1");
}

// Destructor
SceneFloor1::~SceneFloor1()
{}

pugi::xml_node configNodeFloor1;

// Called before render is available
bool SceneFloor1::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//if (config.child("player")) {
	//	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//	player->parameters = config.child("player");
	//}

	for (pugi::xml_node itemNode = config.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		Npcs* npc = (Npcs*)app->entityManager->CreateEntity(EntityType::NPCS);
		npc->parameters = itemNode;
	}

	//if (config.child("map")) {
	//	//Get the map name from the config file and assigns the value in the module
	//	app->map->mapName = config.child("map").attribute("name").as_string();
	//	app->map->path = config.child("map").attribute("path").as_string();
	//}

	configNodeFloor1 = config;

	return ret;
}

// Called before the first frame
bool SceneFloor1::Start()
{
	if (configNodeFloor1.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNodeFloor1.child("map").attribute("name").as_string();
		app->map->path = configNodeFloor1.child("map").attribute("path").as_string();
	}
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();

	//Load the player in the map
	app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(96), PIXEL_TO_METERS(640)), 0);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic(configNodeFloor1.child("villageAmbient").attribute("path").as_string());
	
	return true;
}

// Called each loop iteration
bool SceneFloor1::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneFloor1::Update(float dt)
{
	/*app->render->DrawTexture(backgroundTexture2, 0, 0, &bg, SDL_FLIP_NONE, 0.0f);*/

	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	SetCameraPosition(playerX-550, 48);
	printf("\r cameraX: %d cameraY: %d playerX: %d playerY %d", cameraX, cameraY, playerX, playerY);
	ClampCamera();

	app->render->camera.x += (-cameraX - app->render->camera.x);
	app->render->camera.y += (-cameraY - app->render->camera.y);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	//if (app->render->camera.x - playerX - 100 <= -200 && app->render->camera.x - playerX - 100 >= -12850) {
	//	app->render->camera.x = -(playerX - 100);

	//}
	//if (app->render->camera.x - playerX - 100 <= -12900) {
	//	app->render->camera.x = -6333;
	//}
	return true;
}

// Called each loop iteration
bool SceneFloor1::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		pause = !pause;
		app->hud->onSettings = false;
	}

	return ret;
}

// Called before quitting
bool SceneFloor1::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void SceneFloor1::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneFloor1::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;
	
	

}

bool SceneFloor1::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneFloor1::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneFloor1::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}