#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneTemple.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"
#include "Npcs.h"
#include "DialogTriggerEntity.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneTemple::SceneTemple(bool enabled) : Module(enabled)
{
	name.Create("sceneTemple");
}

// Destructor
SceneTemple::~SceneTemple()
{}

pugi::xml_node configNodeTemple;

// Called before render is available
bool SceneTemple::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//if (config.child("player")) {
	//	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//	player->parameters = config.child("player");
	//}

	//if (config.child("map")) {
	//	//Get the map name from the config file and assigns the value in the module
	//	app->map->mapName = config.child("map").attribute("name").as_string();
	//	app->map->path = config.child("map").attribute("path").as_string();
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

	configNodeTemple = config;

	return ret;
}

// Called before the first frame
bool SceneTemple::Start()
{
	if (configNodeTemple.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNodeTemple.child("map").attribute("name").as_string();
		app->map->path = configNodeTemple.child("map").attribute("path").as_string();
	}
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();

	//Load the player in the map
	app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(96), PIXEL_TO_METERS(640)), 0);
	//ClampCamera();
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	temple = app->tex->Load("Assets/Textures/Screens/templo.png");


	app->render->camera.x = -64;
	app->render->camera.y = -45;

	app->audio->PlayMusic(configNodeTemple.child("TempleMusic").attribute("path").as_string());
	return true;
}

// Called each loop iteration
bool SceneTemple::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTemple::Update(float dt)
{

	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	//SetCameraPosition(0, 64);

	//ClampCamera();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool SceneTemple::PostUpdate()
{
	GamePad& pad = app->input->pads[0];
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.start == KEY_DOWN && !wasStartPressed) {
		pause = !pause;
		app->hud->onSettings = false;
		if (!pause)
		{
			Mix_VolumeMusic(app->sceneMenu->percentageMusic);
		};
		wasStartPressed = true;
	}
	else if (pad.start != KEY_DOWN)
	{
		wasStartPressed = false;
	}
	return ret;
}

// Called before quitting
bool SceneTemple::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();
	app->tex->UnLoad(temple);
	return true;
}

void SceneTemple::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneTemple::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;
	
}

bool SceneTemple::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneTemple::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneTemple::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}
