#include "SceneSelection.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Log.h"
#include "SceneMenu.h"


SceneSelection::SceneSelection(bool enabled) : Module(enabled)
{
	name.Create("sceneselection");
}

SceneSelection::~SceneSelection()
{}

bool SceneSelection::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneSelection");
	bool ret = true;

	return ret;
}

bool SceneSelection::Start()
{
	background = app->tex->Load("Assets/Textures/Screens/Intro/Dioses_Sprites_Fondo.png");
	char1 = app->tex->Load("Assets/Textures/Screens/Intro/Dioses_Sprites_Maat.png");
	char2 = app->tex->Load("Assets/Textures/Screens/Intro/Dioses_Sprites_Thoth.png");
	char3 = app->tex->Load("Assets/Textures/Screens/Intro/Dioses_Sprites_Horus.png");
	char4 = app->tex->Load("Assets/Textures/Screens/Intro/Dioses_Sprites_Isis.png");

	app->sceneMenu->loadGameButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->startButton->state = GuiControlState::HIDDEN;
	//app->sceneMenu->continueButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->settingsButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->creditsButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->exitButton->state = GuiControlState::HIDDEN;

	return true;
}

bool SceneSelection::Update(float dt)
{
	app->render->DrawTexture(background, 0, 0);
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	isHover1 = IsMouseOverRect(mouseX, mouseY, rect1);
	isHover2 = IsMouseOverRect(mouseX, mouseY, rect2);
	isHover3 = IsMouseOverRect(mouseX, mouseY, rect3);
	isHover4 = IsMouseOverRect(mouseX, mouseY, rect4);

	if (isHover1) app->render->DrawTexture(char1, 0, 0);
	else if (isHover2) app->render->DrawTexture(char2, 0, 0);
	else if (isHover3) app->render->DrawTexture(char3, 0, 0);
	else if (isHover4) app->render->DrawTexture(char4, 0, 0);

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
	{
		app->fade->Fade(this, (Module*)app->sceneVillage, 60.0f);
	}
	
	return true;
}

bool SceneSelection::IsMouseOverRect(int mouseX, int mouseY, SDL_Rect rect) {
	return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
		mouseY >= rect.y && mouseY <= rect.y + rect.h;
}

bool SceneSelection::CleanUp()
{
	LOG("Freeing SceneSelection");
	app->tex->UnLoad(background);
	app->tex->UnLoad(char1);
	app->tex->UnLoad(char2);
	app->tex->UnLoad(char3);
	app->tex->UnLoad(char4);
	
	return true;
}
