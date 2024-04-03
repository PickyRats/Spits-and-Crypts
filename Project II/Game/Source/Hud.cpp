#include "Hud.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Defs.h"
#include "Log.h"
#include "SceneMenu.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Hud::Hud(bool enabled) : Module(enabled)
{
	name.Create("hud");
}

Hud::~Hud()
{}

pugi::xml_node configNode3;

bool Hud::Awake(pugi::xml_node& config)
{
	LOG("Loading Hud");
	bool ret = true;
	configNode3 = config;

	return ret;
}

bool Hud::Start()
{
	// Charge all textures
	pause = app->tex->Load(configNode3.child("pause").attribute("texturepath").as_string());
	exitNormal = app->tex->Load(configNode3.child("exitNormal").attribute("texturepath").as_string());
	exitHover = app->tex->Load(configNode3.child("exitHover").attribute("texturepath").as_string());
	exitClick = app->tex->Load(configNode3.child("exitClick").attribute("texturepath").as_string());
	resumeNormal = app->tex->Load(configNode3.child("resumeNormal").attribute("texturepath").as_string());
	resumeHover = app->tex->Load(configNode3.child("resumeHover").attribute("texturepath").as_string());
	resumeClick = app->tex->Load(configNode3.child("resumeClick").attribute("texturepath").as_string());
	settingsNormal = app->tex->Load(configNode3.child("settingsNormal").attribute("texturepath").as_string());
	settingsHover = app->tex->Load(configNode3.child("settingsHover").attribute("texturepath").as_string());
	settingsClick = app->tex->Load(configNode3.child("settingsClick").attribute("texturepath").as_string());
	backToTitleNormal = app->tex->Load(configNode3.child("backToTitleNormal").attribute("texturepath").as_string());
	backToTitleHover = app->tex->Load(configNode3.child("backToTitleHover").attribute("texturepath").as_string());
	backToTitleClick = app->tex->Load(configNode3.child("backToTitleClick").attribute("texturepath").as_string());
	settingsExitNormal = app->tex->Load(configNode3.child("settingsExitNormal").attribute("texturepath").as_string());
	settingsExitHover = app->tex->Load(configNode3.child("settingsExitHover").attribute("texturepath").as_string());
	settingsExitClick = app->tex->Load(configNode3.child("settingsExitClick").attribute("texturepath").as_string());
	settingsReturnNormal = app->tex->Load(configNode3.child("settingsReturnNormal").attribute("texturepath").as_string());
	settingsReturnHover = app->tex->Load(configNode3.child("settingsReturnHover").attribute("texturepath").as_string());
	settingsReturnClick = app->tex->Load(configNode3.child("settingsReturnClick").attribute("texturepath").as_string());
	settingsMusicNormal = app->tex->Load(configNode3.child("settingsMusicNormal").attribute("texturepath").as_string());
	settingsMusicHover = app->tex->Load(configNode3.child("settingsMusicHover").attribute("texturepath").as_string());
	settingsMusicClick = app->tex->Load(configNode3.child("settingsMusicClick").attribute("texturepath").as_string());
	settingsFxNormal = app->tex->Load(configNode3.child("settingsFxNormal").attribute("texturepath").as_string());
	settingsFxHover = app->tex->Load(configNode3.child("settingsFxHover").attribute("texturepath").as_string());
	settingsFxClick = app->tex->Load(configNode3.child("settingsFxClick").attribute("texturepath").as_string());
	settingsFullScreenNormal = app->tex->Load(configNode3.child("settingsFullScreenNormal").attribute("texturepath").as_string());
	settingsFullScreenHover = app->tex->Load(configNode3.child("settingsFullScreenHover").attribute("texturepath").as_string());
	settingsFullScreenClick = app->tex->Load(configNode3.child("settingsFullScreenClick").attribute("texturepath").as_string());
	settingsVSyncNormal = app->tex->Load(configNode3.child("settingsVSyncNormal").attribute("texturepath").as_string());
	settingsVSyncHover = app->tex->Load(configNode3.child("settingsVSyncHover").attribute("texturepath").as_string());
	settingsVSyncClick = app->tex->Load(configNode3.child("settingsVSyncClick").attribute("texturepath").as_string());
	settingsTick = app->tex->Load(configNode3.child("settingsTick").attribute("texturepath").as_string());
	settingsSlider = app->tex->Load(configNode3.child("settingsSlider").attribute("texturepath").as_string());
	settingsBoxNormal = app->tex->Load(configNode3.child("settingsBoxNormal").attribute("texturepath").as_string());
	settingsBoxHover = app->tex->Load(configNode3.child("settingsBoxHover").attribute("texturepath").as_string());
	settings = app->tex->Load(configNode3.child("settings").attribute("texturepath").as_string());


	//Create Buttons
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, NULL, resumeNormal, resumeHover, resumeClick, { 657, 305, 281, 64 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, NULL, settingsNormal, settingsHover, settingsClick, { 657, 418, 279, 64 }, this);
	backToTitleButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, NULL, backToTitleNormal, backToTitleHover, backToTitleClick, { 599, 531, 399, 64 }, this);

	settingsReturnButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, NULL, returnNormal, returnHover, returnClick, { 133, 92, 63, 63 }, this);
	settingsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	settingsFullScreenButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, NULL, settingsBoxNormal, settingsBoxHover, settingsTick, { 661, 494, 89, 89 }, this);
	settingsVSyncButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, NULL, settingsBoxNormal, settingsBoxHover, settingsTick, { 661, 613, 89, 89 }, this);
	settingsMusicButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 10, NULL, settingsSlider, settingsSlider, settingsSlider, { 1124, 259, 30, 80 }, this, { 663, 259, 613, 80 });
	settingsFxButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 11, NULL, settingsSlider, settingsSlider, settingsSlider, { 1124, 376, 30, 80 }, this, { 663, 376, 613, 80 });

	app->sceneMenu->loadGameButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->startButton->state = GuiControlState::HIDDEN;
	//app->sceneMenu->continueButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->settingsButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->creditsButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->exitButton->state = GuiControlState::HIDDEN;

	exitButton->state = GuiControlState::HIDDEN;
	resumeButton->state = GuiControlState::HIDDEN;
	settingsButton->state = GuiControlState::HIDDEN;
	backToTitleButton->state = GuiControlState::HIDDEN;
	settingsReturnButton->state = GuiControlState::HIDDEN;
	settingsExitButton->state = GuiControlState::HIDDEN;
	settingsFullScreenButton->state = GuiControlState::HIDDEN;
	settingsVSyncButton->state = GuiControlState::HIDDEN;
	settingsMusicButton->state = GuiControlState::HIDDEN;
	settingsFxButton->state = GuiControlState::HIDDEN;

	return true;
}

bool Hud::Update(float dt)
{
	//Pause menu
	if (app->scene->pause) {
		//If pause menu is activated, show buttons
		if (resumeButton->state == GuiControlState::HIDDEN)
		{
			resumeButton->state = GuiControlState::NORMAL;
			settingsButton->state = GuiControlState::NORMAL;
			backToTitleButton->state = GuiControlState::NORMAL;
			exitButton->state = GuiControlState::NORMAL;
		}
		if(!onSettings){
			//Render pause
			app->render->DrawTexture(pause, 0, 0, NULL, SDL_FLIP_NONE, 0);
			//Check if buttons are focused or pressed. If pressed, do the action. With sound effects. 
			if (resumeButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (resumeButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
				}
				app->scene->pause = false;
			}
			else if (settingsButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
				}
				onSettings = true;
				settingsReturnButton->state = GuiControlState::NORMAL;
				settingsExitButton->state = GuiControlState::NORMAL;
				settingsFullScreenButton->state = GuiControlState::NORMAL;
				settingsVSyncButton->state = GuiControlState::NORMAL;
				settingsMusicButton->state = GuiControlState::NORMAL;
				settingsFxButton->state = GuiControlState::NORMAL;		
			}
			else if (backToTitleButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (backToTitleButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
				}

				backToTitleButton->state = GuiControlState::HIDDEN;
				resumeButton->state = GuiControlState::HIDDEN;
				settingsButton->state = GuiControlState::HIDDEN;
				exitButton->state = GuiControlState::HIDDEN;

				app->fade->Fade((Module*)app->scene, (Module*)app->sceneMenu, 30.0f);
				app->map->Disable();
				app->particleManager->Disable();
				app->entityManager->Disable();
				app->hud->Disable();
				app->scene->pause = false;
			}
			else if (exitButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (exitButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
				}
				SDL_Quit();
			}
			else {
				app->sceneMenu->fxHoverPlayed = false;
				app->sceneMenu->fxClickPlayed = false;
			}
		}
		//Settings menu on pause
		else if (onSettings) {
			//Hide previous buttons
			resumeButton->state = GuiControlState::HIDDEN;
			settingsButton->state = GuiControlState::HIDDEN;
			backToTitleButton->state = GuiControlState::HIDDEN;
			exitButton->state = GuiControlState::HIDDEN;

			//Render settings menu
			app->render->DrawTexture(settings, 0, 0, NULL, SDL_FLIP_NONE, 0);
			
			//Check if buttons are focused or pressed. If pressed, do the action. With sound effects.
			if (settingsReturnButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsReturnButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
					onSettings = false;
					resumeButton->state = GuiControlState::NORMAL;
					settingsButton->state = GuiControlState::NORMAL;
					backToTitleButton->state = GuiControlState::NORMAL;
					exitButton->state = GuiControlState::NORMAL;
				}
			}
			else if (settingsExitButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsExitButton->state == GuiControlState::PRESSED)
			{
				
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
					onSettings = false;
					SDL_Quit();
				}
			}
			else if (settingsFullScreenButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsFullScreenButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
					if (app->sceneMenu->fullScreen == false) app->sceneMenu->fullScreen = true;
					else app->sceneMenu->fullScreen = false;
				}
			}
			else if (settingsVSyncButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxHover);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsVSyncButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->buttonFxClick);
					app->sceneMenu->fxClickPlayed = true;
					if (app->sceneMenu->vSync == false) app->sceneMenu->vSync = true;
					else app->sceneMenu->vSync = false;
				}
			}
			else
			{
				app->sceneMenu->fxHoverPlayed = false;
				app->sceneMenu->fxClickPlayed = false;
			}
			//FullScreen and VSync
			if (app->sceneMenu->fullScreen)	SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			else SDL_SetWindowFullscreen(app->win->window, 0);

			if (app->sceneMenu->vSync)	app->render->vsync = true;
			else	app->render->vsync = false;
		}
	}
	else
	{
		//If pause menu is deactivated, hide buttons

		resumeButton->state = GuiControlState::HIDDEN;
		settingsButton->state = GuiControlState::HIDDEN;
		backToTitleButton->state = GuiControlState::HIDDEN;
		exitButton->state = GuiControlState::HIDDEN;
		settingsReturnButton->state = GuiControlState::HIDDEN;
		settingsExitButton->state = GuiControlState::HIDDEN;
		settingsFullScreenButton->state = GuiControlState::HIDDEN;
		settingsVSyncButton->state = GuiControlState::HIDDEN;
		settingsMusicButton->state = GuiControlState::HIDDEN;
		settingsFxButton->state = GuiControlState::HIDDEN;
	}

	return true;
}


bool Hud::CleanUp()
{
	LOG("Freeing Hud");

	return true;
}
