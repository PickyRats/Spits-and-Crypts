#include "SceneEnding.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Puzzle.h"
#include "Puzzle2.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Log.h"


SceneEnding::SceneEnding(bool enabled) : Module(enabled)
{
	name.Create("sceneending");
}

SceneEnding::~SceneEnding()
{}
pugi::xml_node configNodeEnding;
bool SceneEnding::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneEnding");
	bool ret = true;

	configNodeEnding = config;

	return ret;
}

bool SceneEnding::Start()
{
	endingImage = app->tex->Load("Assets/Textures/Screens/Ending/defeat_screen.png");
	//credits = app->tex->Load("Assets/Textures/Screens/logo1.png");
	timer = Timer();
	timer.Start();
	//logo_audio = app->audio->LoadFx(configNodeEnding.child("logoFx").attribute("path").as_string());
	app->audio->PlayMusic(configNodeEnding.child("musicEnding").attribute("path").as_string());
	app->puzzle->Disable();
	app->puzzle2->Disable();
	return true;
}

bool SceneEnding::Update(float dt)
{
	LOG("%d", timer.ReadSec());

	if (timer.ReadSec() > 5)
	{
		app->fade->Fade(this, (Module*)app->sceneVillage, 30);
	}

	app->render->DrawTexture(endingImage, 0, 0 , NULL, SDL_FLIP_NONE, 0);
	/*
	if (y > 189) {
		a = a * exp(-0.075);
		y -= a;
	}
	app->render->DrawTexture(credits, 462, y);*/

	return true;
}

bool SceneEnding::CleanUp()
{
	LOG("Freeing SceneEnding");
	app->tex->UnLoad(endingImage);
	app->tex->UnLoad(credits);
	app->audio->UnloadFx(logo_audio);

	
	return true;
}
