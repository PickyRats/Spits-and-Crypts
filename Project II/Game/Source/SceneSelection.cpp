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
#include "CutscenePlayer.h"


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

	
	app->cutscenePlayer->file = "Assets/Video/Cinematica_01.mp4";
	app->cutscenePlayer->Enable();
	app->audio->PlayMusic("Assets/Video/Cinematica_01.ogg", 0);
	app->cutscenePlayer->ConvertPixels(0, 1);

	return true;
}

bool SceneSelection::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	app->render->DrawTexture(background, 0, 0);

	if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN|| (pad.left == KEY_DOWN && !wasLeftPressed)) && currentSelection > 0)
	{
		currentSelection--;
		wasLeftPressed = true;
	}
	else if (pad.left != KEY_DOWN)
	{
		wasLeftPressed = false;
	}
	if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || (pad.right == KEY_DOWN && !wasRightPressed)) && currentSelection < 4)
	{
		currentSelection++;
		wasRightPressed = true;
	}
	else if (pad.right != KEY_DOWN)
	{
		wasRightPressed = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || (pad.a == KEY_DOWN && !wasAPressed))
	{
		app->cutscenePlayer->Disable();

		if (currentSelection == 0) app->cutscenePlayer->file = "Assets/Video/Cinematica_02_Maat.mp4";
		else if (currentSelection == 1) app->cutscenePlayer->file = "Assets/Video/Cinematica_02_Thoth.mp4";
		else if (currentSelection == 2) app->cutscenePlayer->file = "Assets/Video/Cinematica_02_Orus.mp4";
		else if (currentSelection == 3) app->cutscenePlayer->file = "Assets/Video/Cinematica_02_Isis.mp4";
		
		app->cutscenePlayer->Enable();
		app->audio->PlayMusic("Assets/Video/Cinematica_02.ogg", 0);
		app->cutscenePlayer->ConvertPixels(0, 1);
		app->fade->Fade(this, (Module*)app->sceneVillage, 60.0f);
		wasAPressed = true;
	}
	else if (pad.a != KEY_DOWN)
	{
		wasAPressed = false;
	}

	if (currentSelection == 0) app->render->DrawTexture(char1, 0, 0);
	if (currentSelection == 1) app->render->DrawTexture(char2, 0, 0);
	if (currentSelection == 2) app->render->DrawTexture(char3, 0, 0);
	if (currentSelection == 3) app->render->DrawTexture(char4, 0, 0);
	
	return true;
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
