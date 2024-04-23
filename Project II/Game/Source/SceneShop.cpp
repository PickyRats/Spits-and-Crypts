#include "SceneShop.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"
#include "Npcs.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneShop::SceneShop(bool enabled) : Module(enabled)
{
	name.Create("sceneShop");
}

// Destructor
SceneShop::~SceneShop()
{}

pugi::xml_node configNodeShop;

// Called before render is available
bool SceneShop::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//if (config.child("player")) {
	//	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//	player->parameters = config.child("player");
	//}

	//if (config.child("map")) {
	//	//Get the map name from the config file and assigns the value in the module
	//	app->map->name = config.child("map").attribute("name").as_string();
	//	app->map->path = config.child("map").attribute("path").as_string();
	//}
	for (pugi::xml_node itemNode = config.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		Npcs* npc = (Npcs*)app->entityManager->CreateEntity(EntityType::NPCS);
		npc->parameters = itemNode;
	}

	configNodeShop = config;

	return ret;
}

// Called before the first frame
bool SceneShop::Start()
{
	if (configNodeShop.child("map")) {
    		//Get the map name from the config file and assigns the value in the module
    		app->map->mapName = configNodeShop.child("map").attribute("name").as_string();
   		app->map->path = configNodeShop.child("map").attribute("path").as_string();
		
	}
	app->audio->PlayMusic(configNodeShop.child("music").child("ShopMusic").attribute("path").as_string());
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();
	
	app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(96), PIXEL_TO_METERS(640)), 0);
	
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool SceneShop::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool SceneShop::Update(float dt)
{
	app->render->DrawTexture(backgroundTexture2, 0, 0, &bg, SDL_FLIP_NONE, 0.0f);

	playerX = app->map->player->position.x;
  	playerY = app->map->player->position.y;

	SetCameraPosition(0, 0);

	ClampCamera();

	app->render->camera.x += (-cameraX - app->render->camera.x) * cameraSmoothingFactor;
	app->render->camera.y += (-cameraY - app->render->camera.y) * cameraSmoothingFactor;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool SceneShop::PostUpdate()
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
bool SceneShop::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();
	return true;
}

void SceneShop::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneShop::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;

}

bool SceneShop::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneShop::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneShop::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}
