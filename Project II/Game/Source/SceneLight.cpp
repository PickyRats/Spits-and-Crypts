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

	notificationTexture = app->tex->Load("Assets/Textures/Puzzle/triangulo.png");
	lightMirrorTexture = app->tex->Load("Assets/Textures/Puzzle/light_mirror.png");
	lightRayTexture = app->tex->Load("Assets/Textures/Puzzle/light_ray.png");
	backgroundTexture = app->tex->Load("Assets/Maps/Fondo_Sala6.png");

	//load audios
	trampillafx = app->audio->LoadFx("Assets/Audio/Fx/trampilla.wav");
	mirror_rotationFx = app->audio->LoadFx("Assets/Audio/Fx/rotacion_espejo.wav");

	//Load the player in the map
	app->map->player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(96), PIXEL_TO_METERS(1000)), 0);

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

	mirrorBody = app->physics->CreateRectangleSensor(5 * 64 + 32, 18 * 64 - 32, 64, 64, bodyType::STATIC);
	mirrorBody->ctype = ColliderType::LIGHT1;
	trapdoorBody = app->physics->CreateRectangleSensor(8 * 64 + 32, 18 * 64 - 32, 64, 64, bodyType::STATIC);
	trapdoorBody->ctype = ColliderType::LIGHT2;

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

	timer = Timer();
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
	GamePad& pad = app->input->pads[0];
	app->render->DrawTexture(backgroundTexture, 0, 0, NULL, SDL_FLIP_NONE);
	app->render->DrawTexture(backgroundTexture, 64, 0, NULL, SDL_FLIP_NONE);
	playerX = app->map->player->position.x;
	//playerY = app->map->player->position.y;
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT||(pad.up==KEY_DOWN ))
	{
		playerY -= 10;
		
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || (pad.down == KEY_DOWN ))
	{
		playerY += 10;
	
	}
	
	SetCameraPosition(playerX - 550, playerY);

	ClampCamera();
	UpdateCameraShake();

	if (cameraInitialized)
	{
		app->render->camera.x += (-cameraX - app->render->camera.x);
		app->render->camera.y += (-cameraY - app->render->camera.y);
	}

	DrawLightMirrors();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || (pad.l1 == KEY_DOWN && !wasL1Pressed))
	{
		isInteractingMirror = false;
		isInteractingTrapdoor = false;
		if (interactMirror) isInteractingMirror = true;
		else if (interactTrapdoor) isInteractingTrapdoor = true;
		wasL1Pressed = true;
	}
	else if (pad.l1 != KEY_DOWN)
	{
		wasL1Pressed = false;
	}

	// Change mirror and trapdoor
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN || (pad.r1 == KEY_DOWN && !wasR1Pressed))
	{
		if (isInteractingMirror)
		{
			if (mirrorIndex > 0) mirrorIndex--;
			else mirrorIndex = 4;
		}
		else if (isInteractingTrapdoor)
		{
			if (trapdoorIndex > 0) trapdoorIndex--;
			else trapdoorIndex = 1;
		}
		wasR1Pressed = true;
	}
	else if (pad.r1 != KEY_DOWN)
	{
		wasR1Pressed = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN || (pad.l2 == KEY_DOWN && !wasL2Pressed))
	{
		if (isInteractingMirror)
		{
			if (mirrorIndex < 4) mirrorIndex++;
			else mirrorIndex = 0;
		}
		else if (isInteractingTrapdoor)
		{
			if (trapdoorIndex < 1) trapdoorIndex++;
			else trapdoorIndex = 0;
		}
		wasL2Pressed = true;
	}
	else if (pad.l2 != KEY_DOWN)
	{
		wasL2Pressed = false;
	}

	// Rotate mirror
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN|| (pad.r2 == KEY_DOWN && !wasR2Pressed))
	{
		if (isInteractingMirror)
		{
			if (mirrorIndex == 0)
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
			}
			else if (mirrorIndex == 1)
			{
				if (lightMirrors[1].type == 0) lightMirrors[1].type = 1;
				else
				{
					lightMirrors[1].type = 0;
					lightMirrors[1].rotation = 90;
				}
			}
			else if (mirrorIndex == 2)
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
			}
			else if (mirrorIndex == 3)
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
			}
			else if (mirrorIndex == 4)
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
			}
			app->audio->PlayFx(mirror_rotationFx);
			
		}
		else if (isInteractingTrapdoor)
		{
			// Rotate trapdoor
			if (trapdoorIndex == 0)
			{
				if (trapdoors[0].rotation == 0) trapdoors[0].rotation = -90;
				else trapdoors[0].rotation = 0;

			}
			else if (trapdoorIndex == 1)
			{
				if (trapdoors[1].rotation == 0) trapdoors[1].rotation = -90;
				else trapdoors[1].rotation = 0;
			}
			app->audio->PlayFx(trampillafx);
		}
		SetRays();
		wasR2Pressed = true;
	}
	else if (pad.r2 != KEY_DOWN)
	{
		wasR2Pressed = false;

	}
	ShowNotification();
	
	return true;
}

void SceneLight::ShowNotification()
{
	if (!notificationCreated)
	{
		if (isInteractingMirror)
		{
			if (cameraY > lightMirrors[mirrorIndex].position.y) isUp = true;
			else isUp = false;
			if (cameraY + offset[mirrorIndex] < lightMirrors[mirrorIndex].position.y) isDown = true;
			else isDown = false;
		}
		else if (isInteractingTrapdoor)
		{
			if (cameraY > trapdoors[trapdoorIndex].position.y) isUp = true;
			else isUp = false;
			if (cameraY + offset[trapdoorIndex + 4] < trapdoors[trapdoorIndex].position.y) isDown = true;
			else isDown = false;
		}
		
		if (isUp || isDown)
		{
			notificationCreated = true;
			timer.Start();
		}
	}
	else
	{
		if (timer.ReadMSec() < 400)
		{
			if (isInteractingMirror)
			{
				if (isUp) app->render->DrawTexture(notificationTexture, lightMirrors[mirrorIndex].position.x + 20, 100, NULL, SDL_FLIP_NONE, 0);
				else if (isDown) app->render->DrawTexture(notificationTexture, lightMirrors[mirrorIndex].position.x + 20, 620, NULL, SDL_FLIP_VERTICAL, 0);
			}
			else if (isInteractingTrapdoor)
			{
				if (isUp) app->render->DrawTexture(notificationTexture, trapdoors[trapdoorIndex].position.x + 10, 100, NULL, SDL_FLIP_NONE, 0);
				else if (isDown) app->render->DrawTexture(notificationTexture, trapdoors[trapdoorIndex].position.x + 10, 620, NULL, SDL_FLIP_VERTICAL, 0);
			}
		}
		else if (timer.ReadMSec() >= 600)
		{
			notificationCreated = false;
			isUp = false;
			isDown = false;
		}
	}
	
	
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
		}
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

						app->map->EnableLayer("Espejo5_0", false);
						app->map->EnableLayer("Espejo5_1", false);
						app->map->EnableLayer("Espejo5_2", false);
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
		app->render->DrawTexture(
			lightMirrorTexture, 
			lightMirrors[i].position.x, 
			lightMirrors[i].position.y, 
			&lightMirrorRect[lightMirrors[i].type], 
			flip, 
			1.0f, 
			lightMirrors[i].rotation
		);
	}
	for (int i = 0; i < 2; i++)
	{
		app->render->DrawTexture(
			lightMirrorTexture, 
			trapdoors[i].position.x, 
			trapdoors[i].position.y, 
			&lightMirrorRect[3], 
			SDL_FLIP_NONE, 
			1.0f, 
			trapdoors[i].rotation
		);
	}
	
	if (isInteractingMirror)
	{
		app->render->DrawTexture(
			lightMirrorTexture, 
			lightMirrors[mirrorIndex].position.x, 
			lightMirrors[mirrorIndex].position.y, 
			&lightMirrorRect[lightMirrors[mirrorIndex].type + 4], 
			SDL_FLIP_NONE, 
			1.0f, 
			lightMirrors[mirrorIndex].rotation
		);
	}
	if (isInteractingTrapdoor)
	{
		app->render->DrawTexture(
			lightMirrorTexture,
			trapdoors[trapdoorIndex].position.x,
			trapdoors[trapdoorIndex].position.y,
			&lightMirrorRect[7],
			SDL_FLIP_NONE,
			1.0f,
			trapdoors[trapdoorIndex].rotation
		);
	}

	app->render->DrawTexture(lightMirrorTexture, 5 * 64, 17 * 64, &lightMirrorRect[8]);
	app->render->DrawTexture(lightMirrorTexture, 8 * 64, 17 * 64, &lightMirrorRect[9]);
}

void SceneLight::StartCameraShakeX(float duration, float intensity)
{
	shakingCameraX = true;
	shakeTimer = duration;
	shakeIntensity = intensity;
	cameraInitialized = false;
}

void SceneLight::StartCameraShakeY(float duration, float intensity)
{
	shakingCameraY = true;
	shakeTimer = duration;
	shakeIntensity = intensity;
	cameraInitialized = false;
}

void SceneLight::UpdateCameraShake()
{
	// Update camera shake
	if (shakingCameraX)
	{
		if (shakeTimer > 0)
		{
			float offsetX = sin(shakeTimer * 20.0f) * shakeIntensity;
			app->render->camera.x += static_cast<int>(offsetX);
			shakeTimer -= 1.0f;
		}
		else shakingCameraX = false;
	}
	else if (shakingCameraY)
	{
		if (shakeTimer > 0)
		{
			float offsetY = sin(shakeTimer * 20.0f) * shakeIntensity;
			app->render->camera.y += static_cast<int>(offsetY);
			shakeTimer -= 1.0f;
		}
		else shakingCameraY = false;
	}
	else cameraInitialized = true;
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
