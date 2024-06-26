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
#include "EntityManager.h"
#include "Physics.h"
#include "Entity.h"	
#include "Puzzle2.h"
#include "CutscenePlayer.h"


#include "Defs.h"
#include "Log.h"

using namespace std;

SceneVillage::SceneVillage(bool enabled) : Module(enabled)
{
	name.Create("sceneVillage");
}

// Destructor
SceneVillage::~SceneVillage()
{}

pugi::xml_node configNode;

// Called before render is available
bool SceneVillage::Awake(pugi::xml_node& config)
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

	spawnPosition = { 96, 675 };

	configNode = config;

	return ret;
}

// Called before the first frame
bool SceneVillage::Start()
{
	if (configNode.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNode.child("map").attribute("name").as_string();
		app->map->path = configNode.child("map").attribute("path").as_string();
	}
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();
	app->particleManager->Enable();
	app->cutscenePlayer->Disable();

	//Load the player in the map
	app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(spawnPosition.x), PIXEL_TO_METERS(spawnPosition.y)), 0);
	app->map->player->inicio = true;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	int i = 1;
	if (!piedraHecha)
	{
		piedra = app->physics->CreateRectangle(320, 640, 100, 100, DYNAMIC);
		piedra->ctype = ColliderType::ROCK;
		
		piedraHecha = true;
	}

	piedra->body->SetGravityScale(15);

	piedraTexture = app->tex->Load("Assets/Textures/Items/piedra.png");
	rockfx = app->audio->LoadFx("Assets/Audio/Fx/RockFx.wav");
	aldea = app->tex->Load("Assets/Textures/Screens/aldea.png");

	app->audio->PlayMusic(configNode.child("villageAmbient").attribute("path").as_string());

	app->hud->mission10Active = true;
	return true;
}

// Called each loop iteration
bool SceneVillage::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneVillage::Update(float dt)
{
	//app->render->DrawTexture(aldea, -5, 0, NULL, SDL_FLIP_NONE, 1);

	int piedraX = METERS_TO_PIXELS(piedra->body->GetPosition().x);
	int piedraY = METERS_TO_PIXELS(piedra->body->GetPosition().y);
	if (!cutsceneStarted) app->hud->DrawTile(piedraTexture, { piedraX - 50, piedraY - 64});

	if (piedraX>=400 && !cutsceneStarted)
	{
		cutsceneStarted = true;
		movement = true;
		piedra->body->SetActive(false);
		app->physics->world->DestroyBody(piedra->body);
		app->map->player->isWalking = false;
		app->audio->PauseFx(app->map->player->stepsFx);
		app->cutscenePlayer->file = "Assets/Video/Cinematica_03.mp4";
		app->cutscenePlayer->Enable();
		app->audio->PlayMusic("Assets/Video/Cinematica_03.ogg", 0);
		app->cutscenePlayer->ConvertPixels(0, 1);
		app->audio->PlayMusic(configNode.child("villageAmbient").attribute("path").as_string());
		app->cutscenePlayer->Disable();
		app->hud->mission1Complete = true;
		if (!app->hud->mission2Complete)
		{
			app->hud->mission20Active = true;
		}
		app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(27*64), PIXEL_TO_METERS(spawnPosition.y)), 0);
	}
	if (movement)
	{
		app->map->player->inicio = false;
	}

	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	SetCameraPosition(playerX-550, 48);

	ClampCamera();

	app->render->camera.x += (-cameraX - app->render->camera.x);
	app->render->camera.y += (-cameraY - app->render->camera.y);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool SceneVillage::PostUpdate()
{
	GamePad& pad = app->input->pads[0];
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || (pad.start == KEY_DOWN && !wasStartPressed)) {
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
bool SceneVillage::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();
	app->tex->UnLoad(aldea);
	return true;
}

void SceneVillage::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneVillage::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;
	
}

bool SceneVillage::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneVillage::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneVillage::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}
