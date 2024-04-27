#include "SceneIntro.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Log.h"


SceneIntro::SceneIntro(bool enabled) : Module(enabled)
{
	name.Create("sceneintro");
}

SceneIntro::~SceneIntro()
{}
pugi::xml_node configNode4;
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	configNode4 = config;

	return ret;
}

bool SceneIntro::Start()
{
	logo = app->tex->Load("Assets/Textures/Screens/logo.png");
	timer = Timer();
	timer.Start();
	logo_audio = app->audio->LoadFx(configNode4.child("logoFx").attribute("path").as_string());
	app->audio->PlayFx(logo_audio);
	return true;
}

bool SceneIntro::Update(float dt)
{
	if (timer.ReadSec() > 1 && !changingScene)
	{
		app->fade->Fade(this, (Module*)app->sceneMenu, 30);
		changingScene = true;
	}
	app->render->DrawTexture(logo, 0, 0);

	return true;
}

bool SceneIntro::CleanUp()
{
	LOG("Freeing SceneIntro");
	app->tex->UnLoad(logo);
	app->audio->UnloadFx(logo_audio);
	
	return true;
}
