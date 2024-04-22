#include "Hud.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneVillage.h"
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneFloor1.h"
#include "SceneTemple.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Defs.h"
#include "Log.h"
#include "SceneMenu.h"
#include "SceneCombat.h"

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
	// Combat Mode UI
	character1 = app->tex->Load(configNode3.child("character1").attribute("texturepath").as_string());
	fuego = app->tex->Load(configNode3.child("fuego").attribute("texturepath").as_string());
	life1 = app->tex->Load(configNode3.child("life1").attribute("texturepath").as_string());
	life2 = app->tex->Load(configNode3.child("life2").attribute("texturepath").as_string());
	PP = app->tex->Load(configNode3.child("PP").attribute("texturepath").as_string());
	sword1 = app->tex->Load(configNode3.child("sword1").attribute("texturepath").as_string());
	sword2 = app->tex->Load(configNode3.child("sword2").attribute("texturepath").as_string());
	number14 = app->tex->Load(configNode3.child("number14").attribute("texturepath").as_string());
	L3Move = app->tex->Load(configNode3.child("L3Move").attribute("texturepath").as_string());
	execute = app->tex->Load(configNode3.child("execute").attribute("texturepath").as_string());
	Xaccept = app->tex->Load(configNode3.child("Xaccept").attribute("texturepath").as_string());
	playerverd = app->tex->Load(configNode3.child("playerverd").attribute("texturepath").as_string());
	enemyrojo = app->tex->Load(configNode3.child("enemyrojo").attribute("texturepath").as_string());

	//
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
	settingsControls = app->tex->Load(configNode3.child("settingsControls").attribute("texturepath").as_string());
	settingsAudioPanel = app->tex->Load(configNode3.child("settingsAudioPanel").attribute("texturepath").as_string());
	settingsOptionsPanel = app->tex->Load(configNode3.child("settingsOptionsPanel").attribute("texturepath").as_string());
	//settingsMusicNormal = app->tex->Load(configNode3.child("settingsMusicNormal").attribute("texturepath").as_string());
	/*settingsMusicHover = app->tex->Load(configNode3.child("settingsMusicHover").attribute("texturepath").as_string());*/
	//settingsMusicClick = app->tex->Load(configNode3.child("settingsMusicClick").attribute("texturepath").as_string());
	//settingsFxNormal = app->tex->Load(configNode3.child("settingsFxNormal").attribute("texturepath").as_string());
	//settingsFxHover = app->tex->Load(configNode3.child("settingsFxHover").attribute("texturepath").as_string());
	//settingsFxClick = app->tex->Load(configNode3.child("settingsFxClick").attribute("texturepath").as_string());
	//settingsFullScreenNormal = app->tex->Load(configNode3.child("settingsFullScreenNormal").attribute("texturepath").as_string());
	//settingsFullScreenHover = app->tex->Load(configNode3.child("settingsFullScreenHover").attribute("texturepath").as_string());
	//settingsFullScreenClick = app->tex->Load(configNode3.child("settingsFullScreenClick").attribute("texturepath").as_string());
	//settingsVSyncNormal = app->tex->Load(configNode3.child("settingsVSyncNormal").attribute("texturepath").as_string());
	//settingsVSyncHover = app->tex->Load(configNode3.child("settingsVSyncHover").attribute("texturepath").as_string());
	/*settingsVSyncClick = app->tex->Load(configNode3.child("settingsVSyncClick").attribute("texturepath").as_string());*/
	settingsTick = app->tex->Load(configNode3.child("settingsTick").attribute("texturepath").as_string());
	settingsSlider = app->tex->Load(configNode3.child("settingsSlider").attribute("texturepath").as_string());
	settingsBoxNormal = app->tex->Load(configNode3.child("settingsBoxNormal").attribute("texturepath").as_string());
	settingsBoxHover = app->tex->Load(configNode3.child("settingsBoxHover").attribute("texturepath").as_string());
	settings = app->tex->Load(configNode3.child("settings").attribute("texturepath").as_string());

	settingsControlsButtonNormal = app->tex->Load(configNode3.child("settingsControlsButtonNormal").attribute("texturepath").as_string());
	settingsControlsButtonHover = app->tex->Load(configNode3.child("settingsControlsButtonHover").attribute("texturepath").as_string());
	settingsAudioButtonNormal = app->tex->Load(configNode3.child("settingsAudioButtonNormal").attribute("texturepath").as_string());
	settingsAudioButtonHover = app->tex->Load(configNode3.child("settingsAudioButtonHover").attribute("texturepath").as_string());
	settingsOptionsButtonNormal = app->tex->Load(configNode3.child("settingsOptionsButtonNormal").attribute("texturepath").as_string());
	settingsOptionsButtonHover = app->tex->Load(configNode3.child("settingsOptionsButtonHover").attribute("texturepath").as_string());

	//Create Buttons
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, NULL, resumeNormal, resumeHover, resumeClick, { 657, 305, 281, 64 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, NULL, settingsNormal, settingsHover, settingsClick, { 657, 418, 279, 64 }, this);
	backToTitleButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, NULL, backToTitleNormal, backToTitleHover, backToTitleClick, { 599, 531, 399, 64 }, this);

	settingsReturnButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, NULL, returnNormal, returnHover, returnClick, { 133, 92, 63, 63 }, this);
	settingsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	settingsFullScreenButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, NULL, settingsBoxNormal, settingsBoxHover, settingsTick, { 661, 250, 89, 89 }, this);
	settingsVSyncButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, NULL, settingsBoxNormal, settingsBoxHover, settingsTick, { 661, 350, 89, 89 }, this);
	settingsMusicButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 10, NULL, settingsSlider, settingsSlider, settingsSlider, { 1124, 259, 30, 80 }, this, { 663, 259, 613, 80 });
	settingsFxButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 11, NULL, settingsSlider, settingsSlider, settingsSlider, { 1124, 376, 30, 80 }, this, { 663, 376, 613, 80 });
	settingsControlsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, NULL, settingsControlsButtonNormal, settingsControlsButtonHover, exitClick, { 840, 150, 400, 50 }, this);
	settingsAudioButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, NULL, settingsAudioButtonNormal, settingsAudioButtonHover, exitClick, { 440, 150, 400, 50 }, this);
	settingsOptionsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, NULL, settingsOptionsButtonNormal, settingsOptionsButtonHover, exitClick, { 40, 150, 400, 50 }, this);

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

	settingsControlsButton->state = GuiControlState::HIDDEN;
	settingsAudioButton->state = GuiControlState::HIDDEN;
	settingsOptionsButton->state = GuiControlState::HIDDEN;

	return true;
}

bool Hud::Update(float dt)
{
	//Pause menu
	if (app->sceneVillage->pause || app->sceneShop->pause || app->sceneOasisFaraon->pause || app->sceneTemple->pause || app->sceneFloor1->pause) {
		//If pause menu is activated, show buttons
		if (resumeButton->state == GuiControlState::HIDDEN)
		{
			resumeButton->state = GuiControlState::NORMAL;
			settingsButton->state = GuiControlState::NORMAL;
			backToTitleButton->state = GuiControlState::NORMAL;
			exitButton->state = GuiControlState::NORMAL;

		}
		if(!onSettings){

			settingsFullScreenButton->state = GuiControlState::HIDDEN;
			settingsVSyncButton->state = GuiControlState::HIDDEN;
			settingsMusicButton->state = GuiControlState::HIDDEN;
			settingsFxButton->state = GuiControlState::HIDDEN;
			settingsOptionsButton->state = GuiControlState::HIDDEN;
			settingsAudioButton->state = GuiControlState::HIDDEN;
			settingsControlsButton->state = GuiControlState::HIDDEN;
			//Render pause
			app->render->DrawTexture(pause, 0, 0, NULL, SDL_FLIP_NONE, 0);

			if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && app->sceneMenu->currentId < 3))
			{
				app->sceneMenu->currentId++;

			}
			if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && app->sceneMenu->currentId > 1))
			{
				app->sceneMenu->currentId--;
			}
			//Check if buttons are focused or pressed. If pressed, do the action. With sound effects. 
			if (resumeButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (resumeButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}
				app->sceneVillage->pause = false;
			}
			else if (settingsButton->state == GuiControlState::FOCUSED) {

				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}
				onSettings = true;
				settingsControlsButton->state = GuiControlState::NORMAL;
				settingsAudioButton->state = GuiControlState::NORMAL;
				settingsOptionsButton->state = GuiControlState::NORMAL;
				//settingsReturnButton->state = GuiControlState::NORMAL;
				//settingsExitButton->state = GuiControlState::NORMAL;
				//settingsFullScreenButton->state = GuiControlState::NORMAL;
				//settingsVSyncButton->state = GuiControlState::NORMAL;
				//settingsMusicButton->state = GuiControlState::NORMAL;
				//settingsFxButton->state = GuiControlState::NORMAL;		
			}
			else if (backToTitleButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (backToTitleButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}

				backToTitleButton->state = GuiControlState::HIDDEN;
				resumeButton->state = GuiControlState::HIDDEN;
				settingsButton->state = GuiControlState::HIDDEN;
				exitButton->state = GuiControlState::HIDDEN;

				if(app->sceneVillage->active)
				{
					app->fade->Fade((Module*)app->sceneVillage, (Module*)app->sceneMenu, 60.0f);
					app->sceneVillage->pause = false;
				}
				else  if (app->sceneShop->active)
				{
					app->fade->Fade((Module*)app->sceneShop, (Module*)app->sceneMenu, 60.0f);
					app->sceneShop->pause = false;
				}
				else  if (app->sceneOasisFaraon->active)
				{
					app->fade->Fade((Module*)app->sceneOasisFaraon, (Module*)app->sceneMenu, 60.0f);
					app->sceneOasisFaraon->pause = false;
				}
				else  if (app->sceneTemple->active)
				{
					app->fade->Fade((Module*)app->sceneTemple, (Module*)app->sceneMenu, 60.0f);
					app->sceneTemple->pause = false;
				}
				else  if (app->sceneFloor1->active)
				{
					app->fade->Fade((Module*)app->sceneFloor1, (Module*)app->sceneMenu, 60.0f);
					app->sceneFloor1->pause = false;
				}

				app->map->Disable();
				app->particleManager->Disable();
				app->entityManager->Disable();
				app->hud->Disable();
				
			}
			else if (exitButton->state == GuiControlState::FOCUSED) {
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (exitButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
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
			/*if (settingsReturnButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}*/
			
			//onSettingsControls
			if (onSettingsControls)
			{
				settingsFxButton->state = GuiControlState::HIDDEN;
				settingsMusicButton->state = GuiControlState::HIDDEN;
				settingsFullScreenButton->state = GuiControlState::HIDDEN;
				settingsVSyncButton->state = GuiControlState::HIDDEN;
				app->render->DrawTexture(settingsControls, 145, 200, NULL, SDL_FLIP_NONE, 0);
			}
			else if (onSettingsAudio)
			{
				app->render->DrawTexture(settingsAudioPanel, 340, 200, NULL, SDL_FLIP_NONE, 0);
				settingsFxButton->state = GuiControlState::NORMAL;
				settingsMusicButton->state = GuiControlState::NORMAL;
				settingsFullScreenButton->state = GuiControlState::HIDDEN;
				settingsVSyncButton->state = GuiControlState::HIDDEN;
			}
			else if (onSettingsOptions)
			{
				/*if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && currentId != 8)
				{
					currentId++;
					fxHoverPlayed = false;
					fxClickPlayed = false;
				}
				else if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && currentId != 6)
				{
					currentId--;
					fxHoverPlayed = false;
					fxClickPlayed = false;
				}*/
				app->render->DrawTexture(settingsOptionsPanel, 340, 200, NULL, SDL_FLIP_NONE, 0);
				if (buttonsActivated)
				{
					buttonsActivated = false;
					settingsFullScreenButton->state = GuiControlState::NORMAL;
					settingsVSyncButton->state = GuiControlState::NORMAL;
					settingsFxButton->state = GuiControlState::HIDDEN;
					settingsMusicButton->state = GuiControlState::HIDDEN;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			{
				currentId = 1;
				onSettings = false;
				//Show menu buttons
				resumeButton->state = GuiControlState::NORMAL;
				settingsButton->state = GuiControlState::NORMAL;
				backToTitleButton->state = GuiControlState::NORMAL;
				exitButton->state = GuiControlState::NORMAL;

			}
			else if (settingsControlsButton->state == GuiControlState::FOCUSED)
			{
				/*if (fxHoverPlayed == false)
				{
					app->audio->PlayFx(FxButton1);
					fxHoverPlayed = true;
				}*/
			}
			else if (settingsControlsButton->state == GuiControlState::PRESSED)
			{
				/*if (!fxClickPlayed)
				{
					app->audio->PlayFx(FxButton2);
					fxClickPlayed = true;
				}*/
				onSettingsControls = true;
				onSettingsOptions = false;
				onSettingsAudio = false;
				buttonsActivated = true;
			}
			else if (settingsAudioButton->state == GuiControlState::FOCUSED)
			{
				/*if (fxHoverPlayed == false)
				{
					app->audio->PlayFx(FxButton1);
					fxHoverPlayed = true;
				}*/
			}
			else if (settingsAudioButton->state == GuiControlState::PRESSED)
			{
				/*if (fxClickPlayed == false)
				{
					app->audio->PlayFx(FxButton2);
					fxClickPlayed = true;
				}*/
				onSettingsControls = false;
				onSettingsOptions = false;
				onSettingsAudio = true;
				buttonsActivated = true;
			}
			else if (settingsOptionsButton->state == GuiControlState::FOCUSED)
			{
				/*if (fxHoverPlayed == false)
				{
					app->audio->PlayFx(FxButton1);
					fxHoverPlayed = true;
				}*/
			}
			else if (settingsOptionsButton->state == GuiControlState::PRESSED)
			{
				/*if (fxClickPlayed == false)
				{
					app->audio->PlayFx(FxButton2);
					fxClickPlayed = true;
				}*/
				//fxClickPlayed = false;
				onSettingsControls = false;
				onSettingsOptions = true;
				onSettingsAudio = false;
				buttonsActivated = true;
			}
			//if (onSettingsControls)
			//{
			//	settingsFxButton->state = GuiControlState::HIDDEN;
			//	settingsMusicButton->state = GuiControlState::HIDDEN;
			//	settingsFullScreenButton->state = GuiControlState::HIDDEN;
			//	settingsVSyncButton->state = GuiControlState::HIDDEN;
			//	app->render->DrawTexture(settingsControls, 145, 200, NULL, SDL_FLIP_NONE, 0);
			//}
			//else if (settingsReturnButton->state == GuiControlState::PRESSED)
			//{
			//	if (app->sceneMenu->fxClickPlayed == false)
			//	{
			//		app->audio->PlayFx(app->sceneMenu->FxButton2);
			//		app->sceneMenu->fxClickPlayed = true;
			//		onSettings = false;
			//		resumeButton->state = GuiControlState::NORMAL;
			//		settingsButton->state = GuiControlState::NORMAL;
			//		backToTitleButton->state = GuiControlState::NORMAL;
			//		exitButton->state = GuiControlState::NORMAL;
			//	}
			//}
			//else if (settingsExitButton->state == GuiControlState::FOCUSED)
			//{
			//	if (app->sceneMenu->fxHoverPlayed == false)
			//	{
			//		app->audio->PlayFx(app->sceneMenu->FxButton1);
			//		app->sceneMenu->fxHoverPlayed = true;
			//	}
			//}
			//else if (settingsExitButton->state == GuiControlState::PRESSED)
			//{
			//	
			//	if (app->sceneMenu->fxClickPlayed == false)
			//	{
			//		app->audio->PlayFx(app->sceneMenu->FxButton2);
			//		app->sceneMenu->fxClickPlayed = true;
			//		onSettings = false;
			//		SDL_Quit();
			//	}
			//}
			else if (settingsFullScreenButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsFullScreenButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fullScreen == false) app->sceneMenu->fullScreen = true;
				else app->sceneMenu->fullScreen = false;
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}
			}
			else if (settingsVSyncButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsVSyncButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->vSync == false) app->sceneMenu->vSync = true;
				else app->sceneMenu->vSync = false;
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
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
		settingsOptionsButton->state = GuiControlState::HIDDEN;
		settingsAudioButton->state = GuiControlState::HIDDEN;
		settingsControlsButton->state = GuiControlState::HIDDEN;

		if (app->sceneCombat->active) {
			//Combat Mode Drawing
			////players
			app->render->DrawTexture(character1, 20, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(character1, 80, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(character1, 140, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(character1, 200, 660, NULL, SDL_FLIP_NONE, 0);
			////
			// 
			////barras vida
			app->render->DrawTexture(life1, 70, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(life2, 73, 697, NULL, SDL_FLIP_NONE, 0);

			app->render->DrawTexture(life1, 130, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(life2, 133, 697, NULL, SDL_FLIP_NONE, 0);

			app->render->DrawTexture(life1, 190, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(life2, 193, 697, NULL, SDL_FLIP_NONE, 0);

			app->render->DrawTexture(life1, 250, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(life2, 253, 697, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////botones abajo derecha
			app->render->DrawTexture(sword2, 1150, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(fuego, 1210, 660, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////PP
			app->render->DrawTexture(PP, 20, 20, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////Sword1
			app->render->DrawTexture(sword1, 1210, 20, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////14
			app->render->DrawTexture(number14, 52, 46, NULL, SDL_FLIP_NONE, 0);
			////
			// 
			////L3Move
			app->render->DrawTexture(L3Move, 500, 660, NULL, SDL_FLIP_NONE, 0);
			////
			//
			///execute
			app->render->DrawTexture(execute, 620, 660, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////Xaccept
			app->render->DrawTexture(Xaccept, 740, 660, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////enemyrojo
			app->render->DrawTexture(enemyrojo, 1210, 620, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////playerverd
			app->render->DrawTexture(playerverd, 1180, 620, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////L1
			app->render->DrawTexture(L1, 520, 620, NULL, SDL_FLIP_NONE, 0);
			////
			//
			////R1
			app->render->DrawTexture(R1, 1180, 620, NULL, SDL_FLIP_NONE, 0);
			////
			//
		}
	}
	
	


	return true;
}


bool Hud::CleanUp()
{
	LOG("Freeing Hud");

	return true;
}
