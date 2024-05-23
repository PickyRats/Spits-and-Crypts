#include "SceneChoza.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneVillage.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"
#include "DialogManager.h"
#include "DialogTriggerEntity.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneChoza::SceneChoza(bool enabled) : Module(enabled)
{
	name.Create("sceneChoza");
}

// Destructor
SceneChoza::~SceneChoza()
{}

pugi::xml_node configNodeChoza;

// Called before render is available
bool SceneChoza::Awake(pugi::xml_node& config)
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

	for (pugi::xml_node itemNode = config.child("dialogTrigger"); itemNode; itemNode = itemNode.next_sibling("dialogTrigger"))
	{
		DialogTrigger* dialogTrigger = (DialogTrigger*)app->entityManager->CreateEntity(EntityType::DIALOG_TRIGGER);
		dialogTrigger->parameters = itemNode;
	}

	//if (config.child("map")) {
	//	//Get the map name from the config file and assigns the value in the module
	//	app->map->mapName = config.child("map").attribute("name").as_string();
	//	app->map->path = config.child("map").attribute("path").as_string();
	//}

	configNodeChoza = config;

	return ret;
}

// Called before the first frame
bool SceneChoza::Start()
{
	if (configNodeChoza.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNodeChoza.child("map").attribute("name").as_string();
		app->map->path = configNodeChoza.child("map").attribute("path").as_string();
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

	aldea = app->tex->Load("Assets/Textures/Screens/choza.png");

	app->audio->PlayMusic(configNodeChoza.child("villageAmbient").attribute("path").as_string());
	return true;
}

// Called each loop iteration
bool SceneChoza::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneChoza::Update(float dt)
{
	app->render->DrawTexture(aldea, -5, 0, NULL, SDL_FLIP_NONE, 1);

	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	SetCameraPosition(playerX - 550, 48);

	ClampCamera();

	app->render->camera.x += (-cameraX - app->render->camera.x);
	app->render->camera.y += (-cameraY - app->render->camera.y);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool SceneChoza::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		pause = !pause;
		app->hud->onSettings = false;
		if (!pause)
		{
			Mix_VolumeMusic(app->sceneMenu->percentageMusic);
		};
	}

	return ret;
}

// Called before quitting
bool SceneChoza::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();
	app->tex->UnLoad(aldea);
	return true;
}

void SceneChoza::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneChoza::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;

}

bool SceneChoza::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneChoza::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneChoza::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}

