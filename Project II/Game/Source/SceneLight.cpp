#include "SceneLight.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

using namespace std;

SceneLight::SceneLight(bool enabled) : Module(enabled)
{
	name.Create("sceneLight");
}

// Destructor
SceneLight::~SceneLight()
{}

pugi::xml_node configNodeLight;

// Called before render is available
bool SceneLight::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;


	configNodeLight = config;

	return ret;
}

// Called before the first frame
bool SceneLight::Start()
{
	if (configNodeLight.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->mapName = configNodeLight.child("map").attribute("name").as_string();
		app->map->path = configNodeLight.child("map").attribute("path").as_string();
	}
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();

	lightMirrorTexture = app->tex->Load("Assets/Textures/Puzzle/light_mirror.png");
	lightRayTexture = app->tex->Load("Assets/Textures/Puzzle/light_ray.png");

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


	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			rayTiles[i][j].position = { i * 64, j * 64 };
		}
	}

	lightMirrors[0].position = { 15 * 64, 17 * 64 };
	lightMirrors[0].type = 1;
	lightMirrors[1].position = { 9 * 64, 11 * 64 };
	lightMirrors[1].type = 1;
	lightMirrors[1].rotation = 90;
	lightMirrors[2].position = { 4 * 64, 14 * 64 };
	lightMirrors[2].type = 0;
	lightMirrors[3].position = { 6 * 64, 10 * 64 };
	lightMirrors[3].type = 0;
	lightMirrors[3].rotation = -90;
	lightMirrors[4].position = { 10 * 64, 0 * 64 };
	lightMirrors[4].type = 2;
	lightMirrors[4].rotation = -90;

	lightMirrors[5].position = { 19 * 64, 1 * 64 };
	lightMirrors[5].type = 0;
	lightMirrors[5].rotation = -90;
	lightMirrors[6].position = { 18 * 64, 0 * 64 };
	lightMirrors[6].type = 0;
	lightMirrors[6].rotation = 180;
	lightMirrors[7].position = { 0 * 64, 10 * 64 };
	lightMirrors[7].type = 2;
	lightMirrors[7].rotation = 0;
	lightMirrors[8].position = { 0 * 64, 4 * 64 };
	lightMirrors[8].type = 0;
	lightMirrors[8].rotation = 90;
	lightMirrors[9].position = { 4 * 64, 0 * 64 };
	lightMirrors[9].type = 2;
	lightMirrors[9].rotation = -90;
	lightMirrors[10].position = { 10 * 64, 17 * 64 };
	lightMirrors[10].type = 2;
	lightMirrors[10].rotation = 90;

	trapdoors[0].position = { 15 * 64, 5 * 64 + 32};
	trapdoors[1].position = { 10 * 64, 14 * 64 + 32 + 13};
	trapdoors[1].rotation = -90;

	app->map->EnableLayer("Ray", true);
	app->map->EnableLayer("Espejo1_0", true);
	app->map->EnableLayer("Espejo1_1", false);
	app->map->EnableLayer("Espejo1_2", false);
	app->map->EnableLayer("Espejo1_3", false);
	app->map->EnableLayer("Espejo2_0", false);
	app->map->EnableLayer("Espejo2_1", false);
	app->map->EnableLayer("Espejo2_2", false);
	app->map->EnableLayer("Espejo2_3", false);
	app->map->EnableLayer("Espejo3_0", false);
	app->map->EnableLayer("Espejo3_1", false);
	app->map->EnableLayer("Espejo3_2", false);
	app->map->EnableLayer("Espejo3_3", false);
	app->map->EnableLayer("Espejo4_0", false);
	app->map->EnableLayer("Espejo4_1", false);
	app->map->EnableLayer("Espejo4_2", false);
	app->map->EnableLayer("Espejo5_0", false);
	app->map->EnableLayer("Espejo5_1", false);
	app->map->EnableLayer("Espejo5_2", false);

	playerY = 600;

	app->audio->PlayMusic(configNodeLight.child("villageAmbient").attribute("path").as_string());
	return true;
}

// Called each loop iteration
bool SceneLight::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLight::Update(float dt)
{

	playerX = app->map->player->position.x;
	//playerY = app->map->player->position.y;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) playerY-=10;
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) playerY+=10;

	SetCameraPosition(playerX - 550, playerY);

	ClampCamera();

	app->render->camera.x += (-cameraX - app->render->camera.x);
	app->render->camera.y += (-cameraY - app->render->camera.y);

	//SetRays();

	//DrawLightRays();
	DrawLightMirrors();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{

		if (lightMirrors[0].type == 0) lightMirrors[0].type = 1;
		else if (lightMirrors[0].type == 1)
		{
			lightMirrors[0].type = 2;
			lightMirrors[0].rotation = 90;
		}
		else
		{
			lightMirrors[0].type = 0;
			lightMirrors[0].rotation = 0;
		}
		SetRays();
	}
	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		if (lightMirrors[1].type == 0) lightMirrors[1].type = 1;
		else
		{
			lightMirrors[1].type = 0;
			lightMirrors[1].rotation = 90;
		}
		SetRays();
	}
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		if (lightMirrors[2].type == 0 && lightMirrors[2].rotation == 0) lightMirrors[2].rotation = 90;
		else if (lightMirrors[2].type == 0 && lightMirrors[2].rotation == 90)
		{
			lightMirrors[2].type = 1;
			lightMirrors[2].rotation = 0;
		}
		else if (lightMirrors[2].type == 1)
		{
			lightMirrors[2].type = 2;
			lightMirrors[2].rotation = 90;
		}
		else
		{
			lightMirrors[2].type = 0;
			lightMirrors[2].rotation = 0;
		}
		SetRays();
	}
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (lightMirrors[3].type == 0)
		{
			lightMirrors[3].type = 2;
			lightMirrors[3].rotation = 0;
		}
		else
		{
			lightMirrors[3].type = 0;
			lightMirrors[3].rotation = -90;
		}
		SetRays();
	}
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		if (lightMirrors[4].type == 0)
		{
			lightMirrors[4].type = 2;
			lightMirrors[4].rotation = -90;
		}
		else
		{
			lightMirrors[4].type = 0;
			lightMirrors[4].rotation = 180;
		}
		SetRays();
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (trapdoors[0].rotation == 0) trapdoors[0].rotation = -90;
		else trapdoors[0].rotation = 0;
		SetRays();
	}
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		if (trapdoors[1].rotation == 0) trapdoors[1].rotation = -90;
		else trapdoors[1].rotation = 0;
		SetRays();
	}

	return true;
}

// Called each loop iteration
bool SceneLight::PostUpdate()
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
bool SceneLight::CleanUp()
{
	LOG("Freeing scene");

	app->map->Disable();

	return true;
}

void SceneLight::SetRays()
{
	// Mirror 1
	if (lightMirrors[0].type == 0)
	{
		app->map->EnableLayer("Espejo1_2", false);
		app->map->EnableLayer("Espejo1_3", false);
		app->map->EnableLayer("Espejo1_1", true);

		// Mirror 2
		if (lightMirrors[1].type == 1)
		{
			app->map->EnableLayer("Espejo2_0", false);
			app->map->EnableLayer("Espejo2_1", false);
			app->map->EnableLayer("Espejo2_3", true);

			app->map->EnableLayer("Espejo3_0", false);
			app->map->EnableLayer("Espejo3_1", false);
			app->map->EnableLayer("Espejo3_2", false);
			app->map->EnableLayer("Espejo3_3", false);

			app->map->EnableLayer("Espejo4_0", false);
			app->map->EnableLayer("Espejo4_1", false);
			app->map->EnableLayer("Espejo4_2", false);

			app->map->EnableLayer("Espejo5_0", false);
			app->map->EnableLayer("Espejo5_1", false);
			app->map->EnableLayer("Espejo5_2", false);
		}
		else if (lightMirrors[1].type == 0)
		{
			app->map->EnableLayer("Espejo2_3", false);
			app->map->EnableLayer("Espejo2_0", true);

			// Trapdoor
			if (trapdoors[0].rotation == -90)
			{
				app->map->EnableLayer("Espejo2_1", true);

				// Mirror 3

				if (lightMirrors[2].type == 1)
				{
					app->map->EnableLayer("Espejo3_2", false);
					app->map->EnableLayer("Espejo3_1", true);
				}
				else if (lightMirrors[2].type == 2)
				{
					app->map->EnableLayer("Espejo3_1", false);
					app->map->EnableLayer("Espejo3_3", true);

					if (lightMirrors[3].type == 0)
					{
						app->map->EnableLayer("Espejo4_0", false);
						app->map->EnableLayer("Espejo4_1", true);
					}
					else if (lightMirrors[3].type == 2)
					{
						app->map->EnableLayer("Espejo4_1", false);
						app->map->EnableLayer("Espejo4_0", true);

						if (lightMirrors[4].type == 0)
						{ 
							app->map->EnableLayer("Espejo5_1", false);
							app->map->EnableLayer("Espejo5_2", false);
							app->map->EnableLayer("Espejo5_0", true);
						}
						else if (lightMirrors[4].type == 2)
						{
							app->map->EnableLayer("Espejo5_0", false);
							if (trapdoors[1].rotation == -90)
							{
								app->map->EnableLayer("Espejo5_2", false);
								app->map->EnableLayer("Espejo5_1", true);
							}
							else if (trapdoors[1].rotation == 0)
							{
								app->map->EnableLayer("Espejo5_1", false);
								app->map->EnableLayer("Espejo5_2", true);
							}
						}
					}
				}
				else if (lightMirrors[2].type == 0 && lightMirrors[2].rotation == 0)
				{
					app->map->EnableLayer("Espejo3_3", false);
					app->map->EnableLayer("Espejo3_0", true);

					app->map->EnableLayer("Espejo4_0", false);
					app->map->EnableLayer("Espejo4_1", false);
					app->map->EnableLayer("Espejo4_2", false);

					app->map->EnableLayer("Espejo5_0", false);
					app->map->EnableLayer("Espejo5_1", false);
					app->map->EnableLayer("Espejo5_2", false);
				}
				else if (lightMirrors[2].type == 0 && lightMirrors[2].rotation == 90)
				{
					app->map->EnableLayer("Espejo3_0", false);
					app->map->EnableLayer("Espejo3_2", true);
				}
			}
			else if (trapdoors[0].rotation == 0)
			{
				app->map->EnableLayer("Espejo2_1", false);
				app->map->EnableLayer("Espejo3_0", false);
				app->map->EnableLayer("Espejo3_1", false);
				app->map->EnableLayer("Espejo3_2", false);
				app->map->EnableLayer("Espejo3_3", false);

				app->map->EnableLayer("Espejo4_0", false);
				app->map->EnableLayer("Espejo4_1", false);
				app->map->EnableLayer("Espejo4_2", false);

				app->map->EnableLayer("Espejo5_0", false);
				app->map->EnableLayer("Espejo5_1", false);
				app->map->EnableLayer("Espejo5_2", false);
			}
		}
	}
	else if (lightMirrors[0].type == 1)
	{
		app->map->EnableLayer("Espejo1_1", false);
		app->map->EnableLayer("Espejo1_0", true);

		app->map->EnableLayer("Espejo2_3", false);
		app->map->EnableLayer("Espejo2_0", false);
		app->map->EnableLayer("Espejo2_1", false);

		app->map->EnableLayer("Espejo3_0", false);
		app->map->EnableLayer("Espejo3_1", false);
		app->map->EnableLayer("Espejo3_2", false);
		app->map->EnableLayer("Espejo3_3", false);

		app->map->EnableLayer("Espejo4_0", false);
		app->map->EnableLayer("Espejo4_1", false);
		app->map->EnableLayer("Espejo4_2", false);

		app->map->EnableLayer("Espejo5_0", false);
		app->map->EnableLayer("Espejo5_1", false);
		app->map->EnableLayer("Espejo5_2", false);
	}
	else if (lightMirrors[0].type == 2)
	{
		app->map->EnableLayer("Espejo1_0", false);

		// Trapdoor
		if (trapdoors[0].rotation == -90)
		{
			app->map->EnableLayer("Espejo1_2", true);
			app->map->EnableLayer("Espejo1_3", false);
		}
		else if (trapdoors[0].rotation == 0)
		{
			app->map->EnableLayer("Espejo1_3", true);
			app->map->EnableLayer("Espejo1_2", false);
		}
	}
	
}

void SceneLight::DrawLightMirrors()
{
	for (int i = 0; i < 11; i++)
	{
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (i == 7) flip = SDL_FLIP_HORIZONTAL;
		app->render->DrawTexture(lightMirrorTexture, lightMirrors[i].position.x, lightMirrors[i].position.y, &lightMirrorRect[lightMirrors[i].type], flip, 1.0f, lightMirrors[i].rotation);
	}
	for (int i = 0; i < 2; i++)
	{
		app->render->DrawTexture(lightMirrorTexture, trapdoors[i].position.x, trapdoors[i].position.y, &lightMirrorRect[3], SDL_FLIP_NONE, 1.0f, trapdoors[i].rotation);
	}
}

void SceneLight::SetCameraPosition(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void SceneLight::ClampCamera()
{
	// Clamp camera

	if (cameraX < 0) cameraX = 0;
	else if (cameraX + windowW > levelWidth) cameraX = levelWidth - windowW;

	if (cameraY < 0) cameraY = 0;
	else if (cameraY + windowH > levelHeight) cameraY = levelHeight - windowH;

}

bool SceneLight::LoadState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.attribute("status").as_bool();

	return true;
}

bool SceneLight::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("status");
	status.append_attribute("status").set_value(1);

	return true;
}
