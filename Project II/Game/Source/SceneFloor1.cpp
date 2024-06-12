#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneFloor1.h"
#include "SceneCombat.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "DialogManager.h"
#include "DialogTriggerEntity.h"
#include "Hud.h"
#include "Puzzle.h"
#include "Puzzle2.h"
#include "Player.h"
#include "puertas.h"
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
	int i = 0;
	//if (config.child("player")) {
	//	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//	player->parameters = config.child("player");
	//}

	for (pugi::xml_node itemNode = config.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		paalaya = (Npcs*)app->entityManager->CreateEntity(EntityType::NPCS);
		paalaya->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("Piece"); itemNode; itemNode = itemNode.next_sibling("Piece"))
	{
		PiezasPuzle* pieces = (PiezasPuzle*)app->entityManager->CreateEntity(EntityType::PIEZAS);
		pieces->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("puerta"); itemNode; itemNode = itemNode.next_sibling("puerta"))
	{
		puertas[i] = (Puertas*)app->entityManager->CreateEntity(EntityType::PUERTAS);
		puertas[i]->parameters = itemNode;
		i++;
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
	app->particleManager->Enable();
  
	app->puzzle->Enable();
	wall = app->physics->CreateRectangle(37 * 64, 34 * 64, 10, 2 * 64, STATIC);
	wall->ctype = ColliderType::WALL;
	wall2 = app->physics->CreateRectangle(109*64, 28*64, 10, 3 * 64, STATIC);
	wall2->ctype = ColliderType::WALL;
	wall3 = app->physics->CreateRectangle(168 * 64, 20 * 64, 10, 3 * 64, STATIC);
	wall3->ctype = ColliderType::WALL;

	//Load the player in the map
	app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(playerStartPosition.x), PIXEL_TO_METERS(playerStartPosition.y)), 0);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(backgroundTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->render->camera.x = 0;
	app->render->camera.y = 2368;

	//carga assets
	floor1background = app->tex->Load("Assets/Textures/Screens/floor1background.png");
	
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
	//activar puzzle 2 cuando el player llegue a la posicion X: 10281 Y 12790
	if (app->map->player->position.x >= 9100)
	{
		app->puzzle2->Enable();
	}
	//una vez completado el puzzle 2, se desactiva y se activa el puzzle 3
	if (app->puzzle2->puzzleCompleted)
	{
		app->puzzle2->Disable();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2)==KEY_DOWN)
	{
		//Hazme una fade a scenelight
		app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneLight, 60.0f);
		app->puzzle2->Disable();
	}

	//dibuja background
	//app->render->DrawTexture(floor1background, 0, 0, NULL, SDL_FLIP_NONE, 1);
	//app->render->DrawTexture(floor1background, 2940, 0, NULL, SDL_FLIP_NONE, 1);
	//app->render->DrawTexture(floor1background, 5880, 0, NULL, SDL_FLIP_NONE, 1);


	playerX = app->map->player->position.x;
	playerY = app->map->player->position.y;

	SetCameraPosition(playerX-550, playerY-360);
	/*printf("\r cameraX: %d cameraY: %d playerX: %d playerY %d", cameraX, cameraY, playerX, playerY);*/
	ClampCamera();

	app->render->camera.x += (-cameraX - app->render->camera.x);
	app->render->camera.y += (-cameraY - app->render->camera.y);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	if (app->sceneFloor1->puertas[1]->puzle1Completed && canDelete)
	{
		DeleteWall();
	}
	if (app->sceneCombat->combatCompleted && canDelete2)
	{
		DeleteWall2();
	}
	if (app->sceneFloor1->puertas[2]->puzle1Completed && canDelete3)
	{
		DeleteWall3();
	}
	return true;
}

// Called each loop iteration
bool SceneFloor1::PostUpdate()
{
	GamePad& pad = app->input->pads[0];
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN||pad.start==KEY_DOWN && !wasStartPressed) {
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
bool SceneFloor1::CleanUp()
{
	LOG("Freeing scene");
	app->map->Disable();

	app->tex->UnLoad(floor1background);
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

void SceneFloor1::DeleteWall()
{
	wall2->body->SetActive(false);
	canDelete = false;
}
void SceneFloor1::DeleteWall2()
{
	wall->body->SetActive(false);
	canDelete2 = false;
}
void SceneFloor1::DeleteWall3()
{
	wall3->body->SetActive(false);
	canDelete2 = false;
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

void SceneFloor1::PlayMusic()
{
	app->audio->PlayMusic(configNodeFloor1.child("Floor1Music").attribute("path").as_string());
}
