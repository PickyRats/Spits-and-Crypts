#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneOasisFaraon.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneOasisFaraon::SceneOasisFaraon(bool enabled) : Module(enabled)
{
	name.Create("sceneOasis");
}

// Destructor
SceneOasisFaraon::~SceneOasisFaraon()
{}

pugi::xml_node configNodeOasis;

// Called before render is available
bool SceneOasisFaraon::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	configNodeOasis = config;

	return ret;
}

// Called before the first frame
bool SceneOasisFaraon::Start()
{
	if (configNodeOasis.child("map"))
	{
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNodeOasis.child("map").attribute("name").as_string();
		app->map->path = configNodeOasis.child("map").attribute("path").as_string();
	}
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

	app->audio->PlayMusic(configNodeOasis.child("OasisMusic").attribute("path").as_string());

	return true;
}

// Called each loop iteration
bool SceneOasisFaraon::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneOasisFaraon::Update(float dt)
{
	/*app->render->DrawTexture(backgroundTexture2, 0, 0, &bg, SDL_FLIP_NONE, 0.0f);*/

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
bool SceneOasisFaraon::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		pause = !pause;
		app->hud->onSettings = false;
	}

	return ret;
}

// Called before quitting
bool SceneOasisFaraon::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();
	return true;
}

void SceneOasisFaraon::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneOasisFaraon::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;
	
}

bool SceneOasisFaraon::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneOasisFaraon::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}

bool SceneOasisFaraon::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}
