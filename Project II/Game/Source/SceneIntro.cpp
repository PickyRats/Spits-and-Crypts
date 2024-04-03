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

bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMenu");
	bool ret = true;

	return ret;
}

bool SceneIntro::Start()
{
	logo = app->tex->Load("Assets/Textures/Screens/logo.png");
	timer = Timer();
	timer.Start();
	logo_audio = app->audio->LoadFx("Assets/Audio/Fx/logo.wav");
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
	
	return true;
}
