#include "SceneMenu.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneVillage.h"
#include "SceneShop.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ParticleManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu(bool enabled) : Module(enabled)
{
	name.Create("scenemenu");
}

SceneMenu::~SceneMenu()
{}

pugi::xml_node configNode2;

bool SceneMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMenu");
	bool ret = true;

	configNode2 = config;

	return ret;
}

bool SceneMenu::Start()
{
	//Load textures
	background = app->tex->Load(configNode2.child("background").attribute("texturepath").as_string());
	playNormal = app->tex->Load(configNode2.child("playNormal").attribute("texturepath").as_string());
	playHover = app->tex->Load(configNode2.child("playHover").attribute("texturepath").as_string());
	playClick = app->tex->Load(configNode2.child("playClick").attribute("texturepath").as_string());
	loadGameDisabled = app->tex->Load(configNode2.child("loadGameDisabled").attribute("texturepath").as_string());
	loadGameNormal = app->tex->Load(configNode2.child("loadGameNormal").attribute("texturepath").as_string());
	loadGameHover = app->tex->Load(configNode2.child("loadGameHover").attribute("texturepath").as_string());
	loadGameClick = app->tex->Load(configNode2.child("loadGameClick").attribute("texturepath").as_string());
	settingsNormal = app->tex->Load(configNode2.child("settingsNormal").attribute("texturepath").as_string());
	settingsHover = app->tex->Load(configNode2.child("settingsHover").attribute("texturepath").as_string());
	settingsClick = app->tex->Load(configNode2.child("settingsClick").attribute("texturepath").as_string());
	creditsNormal = app->tex->Load(configNode2.child("creditsNormal").attribute("texturepath").as_string());
	creditsHover = app->tex->Load(configNode2.child("creditsHover").attribute("texturepath").as_string());
	creditsClick = app->tex->Load(configNode2.child("creditsClick").attribute("texturepath").as_string());
	exitNormal = app->tex->Load(configNode2.child("exitNormal").attribute("texturepath").as_string());
	exitHover = app->tex->Load(configNode2.child("exitHover").attribute("texturepath").as_string());
	exitClick = app->tex->Load(configNode2.child("exitClick").attribute("texturepath").as_string());
	settings = app->tex->Load(configNode2.child("settings").attribute("texturepath").as_string());
	credits = app->tex->Load(configNode2.child("credits").attribute("texturepath").as_string());
	settingsBoxNormal = app->tex->Load(configNode2.child("settingsBoxNormal").attribute("texturepath").as_string());
	settingsBoxHover = app->tex->Load(configNode2.child("settingsBoxHover").attribute("texturepath").as_string());
	settingsControls = app->tex->Load(configNode2.child("settingsControls").attribute("texturepath").as_string());
	settingsTick = app->tex->Load(configNode2.child("settingsTick").attribute("texturepath").as_string());
	settingsSlider = app->tex->Load(configNode2.child("settingsSlider").attribute("texturepath").as_string());
	settingsAudioPanel = app->tex->Load(configNode2.child("settingsAudioPanel").attribute("texturepath").as_string());
	settingsOptionsPanel = app->tex->Load(configNode2.child("settingsOptionsPanel").attribute("texturepath").as_string());
	settingsOptionsButtonNormal = app->tex->Load(configNode2.child("settingsOptionsButtonNormal").attribute("texturepath").as_string());
	settingsOptionsButtonHover = app->tex->Load(configNode2.child("settingsOptionsButtonHover").attribute("texturepath").as_string());
	settingsAudioButtonNormal = app->tex->Load(configNode2.child("settingsAudioButtonNormal").attribute("texturepath").as_string());
	settingsAudioButtonHover = app->tex->Load(configNode2.child("settingsAudioButtonHover").attribute("texturepath").as_string());
	settingsControlsButtonNormal = app->tex->Load(configNode2.child("settingsControlsButtonNormal").attribute("texturepath").as_string());
	settingsControlsButtonHover = app->tex->Load(configNode2.child("settingsControlsButtonHover").attribute("texturepath").as_string());
	FxButton1 = app->audio->LoadFx(configNode2.child("buttonFx1").attribute("path").as_string());
	FxButton2 = app->audio->LoadFx(configNode2.child("buttonFx2").attribute("path").as_string());

	//Menu Buttons
	startButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, NULL, playNormal, playHover, playClick, { 100, 250, 400, 50 }, this);//primer numero la posicion en x, 2ndo la pos en y, 3r el largo del boton y 4t el alto del boton.
	loadGameButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, loadGameDisabled, loadGameNormal, loadGameHover, loadGameClick, { 100, 330, 400, 50 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, NULL, settingsNormal, settingsHover, settingsClick, { 100, 410, 400, 50 }, this);
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, NULL, creditsNormal, creditsHover, creditsClick, { 100, 490, 400, 50 }, this);
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, NULL, exitNormal, exitHover, exitClick, { 100, 570, 400, 50 }, this);

	startButton->state = GuiControlState::NORMAL;
	loadGameButton->state = GuiControlState::DISABLED;
	settingsButton->state = GuiControlState::NORMAL;
	creditsButton->state = GuiControlState::NORMAL;
	exitButton->state = GuiControlState::NORMAL;

	//Settings Buttons
	settingsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	settingsFullScreenButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, NULL, settingsBoxNormal, settingsBoxHover, settingsTick, { 661, 250, 89, 89 }, this);
	settingsVSyncButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, NULL, settingsBoxNormal, settingsBoxHover, settingsTick, { 661, 350, 89, 89 }, this);
	settingsMusicButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 10, NULL, settingsSlider, settingsSlider, settingsSlider, { 1124, 259, 30, 80 }, this, { 663, 259, 613, 80 });
	settingsFxButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 11, NULL, settingsSlider, settingsSlider, settingsSlider, { 1124, 376, 30, 80 }, this, { 663, 376, 613, 80 });
	settingsOptionsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, NULL, settingsOptionsButtonNormal, settingsOptionsButtonHover, exitClick, { 40, 150, 400, 50 }, this);
	settingsAudioButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, NULL, settingsAudioButtonNormal, settingsAudioButtonHover, exitClick, { 440, 150, 400, 50 }, this);
	settingsControlsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, NULL, settingsControlsButtonNormal, settingsControlsButtonHover, exitClick, { 840, 150, 400, 50 }, this);

	settingsExitButton->state = GuiControlState::NORMAL;
	settingsFullScreenButton->state = GuiControlState::NORMAL;
	settingsVSyncButton->state = GuiControlState::NORMAL;
	settingsMusicButton->state = GuiControlState::NORMAL;
	settingsFxButton->state = GuiControlState::NORMAL;
	settingsOptionsButton->state = GuiControlState::NORMAL;
	
	//Credits Buttons
	creditsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	

	//audio
	app->audio->PlayMusic(configNode2.child("musicMenu").attribute("path").as_string());

	return true;
}

bool SceneMenu::Update(float dt)
{
	//On menu screen
	if(onMenu && !onSettings && !onCredits){

		//Hide settings buttons
		settingsExitButton->state = GuiControlState::HIDDEN;
		settingsFullScreenButton->state = GuiControlState::HIDDEN;
		settingsVSyncButton->state = GuiControlState::HIDDEN;
		settingsMusicButton->state = GuiControlState::HIDDEN;
		settingsFxButton->state = GuiControlState::HIDDEN;
		settingsOptionsButton->state = GuiControlState::HIDDEN;
		settingsAudioButton->state = GuiControlState::HIDDEN;
		settingsControlsButton->state = GuiControlState::HIDDEN;
		creditsExitButton->state = GuiControlState::HIDDEN;

		//Render background 
		app->render->DrawTexture(background, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//Check if buttons are focused or pressed. If pressed, do the action. With sound effects.
		if (startButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (startButton->state == GuiControlState::PRESSED)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}

			app->fade->Fade(this, (Module*)app->sceneOasisFaraon, 60.0f);

		}
		else if (loadGameButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (loadGameButton->state == GuiControlState::PRESSED)
		{
			
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}
		}
		else if (settingsButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (settingsButton->state == GuiControlState::PRESSED)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}
			//Show settings buttons
			onSettings = true;
			settingsOptionsButton->state = GuiControlState::NORMAL;
			settingsAudioButton->state = GuiControlState::NORMAL;
			settingsControlsButton->state = GuiControlState::NORMAL;
			//settingsExitButton->state = GuiControlState::NORMAL;
		}
		else if (creditsButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true; 
			}
		}
		else if (creditsButton->state == GuiControlState::PRESSED)
		{

			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
				onCredits = true;
			}
		}
		else if (exitButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (exitButton->state == GuiControlState::PRESSED)
		{

			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
				return false;
			}
		}
		else
		{
			fxHoverPlayed = false;
			fxClickPlayed = false;
		}
	}
	//On settings screen
	else if (onSettings)
	{
		//Hide menu buttons
		startButton->state = GuiControlState::HIDDEN;
		loadGameButton->state = GuiControlState::HIDDEN;
		settingsButton->state = GuiControlState::HIDDEN;
		creditsButton->state = GuiControlState::HIDDEN;
		exitButton->state = GuiControlState::HIDDEN;

		if (onMenu) {
			app->render->DrawTexture(background, 0, 0, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(settings, 0, 0, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(controlsHint, 30, 670, NULL, SDL_FLIP_NONE, 0);
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
				app->render->DrawTexture(settingsAudioPanel,340, 200, NULL, SDL_FLIP_NONE, 0);
				settingsFxButton->state = GuiControlState::NORMAL;
				settingsMusicButton->state = GuiControlState::NORMAL;
				settingsFullScreenButton->state = GuiControlState::HIDDEN;
				settingsVSyncButton->state = GuiControlState::HIDDEN;
			}
			else if (onSettingsOptions)
			{	
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
		}
		//return control
		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			onSettings = false;
			//Show menu buttons
			startButton->state = GuiControlState::NORMAL;
			loadGameButton->state = GuiControlState::DISABLED;
			settingsButton->state = GuiControlState::NORMAL;
			creditsButton->state = GuiControlState::NORMAL;
			exitButton->state = GuiControlState::NORMAL;

		}
		else if (settingsOptionsButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (settingsOptionsButton->state == GuiControlState::PRESSED)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}
			onSettingsControls = false;
			onSettingsOptions = true;
			onSettingsAudio = false;
			buttonsActivated = true;
		}
		else if (settingsAudioButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (settingsAudioButton->state == GuiControlState::PRESSED)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}
			onSettingsControls = false;
			onSettingsOptions = false;
			onSettingsAudio = true;
			buttonsActivated = true;
		}
		else if (settingsControlsButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (settingsControlsButton->state == GuiControlState::PRESSED)
		{
			if (!fxClickPlayed)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}
			onSettingsControls = true;
			onSettingsOptions = false;
			onSettingsAudio = false;
			buttonsActivated = true;
		}
		else if (settingsFullScreenButton->state == GuiControlState::FOCUSED)
		{
			if (!fxHoverPlayed)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (settingsFullScreenButton->state == GuiControlState::PRESSED)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
				if (fullScreen == false) fullScreen = true;
				else fullScreen = false;
			}
		}
		else if (settingsVSyncButton->state == GuiControlState::FOCUSED)
		{
			if (fxHoverPlayed == false)
			{
				app->audio->PlayFx(FxButton1);
				fxHoverPlayed = true;
			}
		}
		else if (settingsVSyncButton->state == GuiControlState::PRESSED)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
				if (vSync == false) vSync = true;
				else vSync = false;
			}
		}
		else
		{
			fxHoverPlayed = false;
			fxClickPlayed = false;
		}
		//FullScreen and VSync
		if (fullScreen)	SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		else SDL_SetWindowFullscreen(app->win->window, 0);

		if (vSync)	app->render->vsync = true;
		else	app->render->vsync = false;
	}
	//On credits screen
	else if (onCredits)
	{
		//Hide menu buttons
		startButton->state = GuiControlState::HIDDEN;
		loadGameButton->state = GuiControlState::HIDDEN;
		settingsButton->state = GuiControlState::HIDDEN;
		creditsButton->state = GuiControlState::HIDDEN;
		exitButton->state = GuiControlState::HIDDEN;

		//Render background and credits
		if (onMenu) {
			app->render->DrawTexture(background, 0, 0, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(credits, 0, 0, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(controlsHint, 30, 670, NULL, SDL_FLIP_NONE, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
				onCredits = false;
				//Show menu buttons
				startButton->state = GuiControlState::NORMAL;
				loadGameButton->state = GuiControlState::NORMAL;
				settingsButton->state = GuiControlState::NORMAL;
				creditsButton->state = GuiControlState::NORMAL;
				exitButton->state = GuiControlState::NORMAL;
			}
		}
		else
		{
			//Reset sound effects
			fxHoverPlayed = false;
			fxClickPlayed = false;
		}
	}

	return true;
}

bool SceneMenu::CleanUp()
{
	LOG("Freeing SceneMenu");

	app->tex->UnLoad(background);
	app->tex->UnLoad(playNormal);
	app->tex->UnLoad(playHover);
	app->tex->UnLoad(playClick);
	app->tex->UnLoad(loadGameDisabled);
	app->tex->UnLoad(loadGameNormal);
	app->tex->UnLoad(loadGameHover);
	app->tex->UnLoad(loadGameClick);
	app->tex->UnLoad(settingsNormal);
	app->tex->UnLoad(settingsHover);
	app->tex->UnLoad(settingsClick);
	app->tex->UnLoad(creditsNormal);
	app->tex->UnLoad(creditsHover);
	app->tex->UnLoad(creditsClick);
	app->tex->UnLoad(exitNormal);
	app->tex->UnLoad(exitHover);
	app->tex->UnLoad(exitClick);
	app->tex->UnLoad(settings);
	app->tex->UnLoad(returnNormal);
	app->tex->UnLoad(returnHover);
	app->tex->UnLoad(returnClick);
	app->tex->UnLoad(settingsBoxNormal);
	app->tex->UnLoad(settingsBoxHover);
	app->tex->UnLoad(settingsTick);
	app->tex->UnLoad(settingsSlider);
	app->tex->UnLoad(credits);
	//Clean Music
	app->audio->CleanMusic(configNode2.child("musicMenu").attribute("path").as_string(), configNode2.child("musicMenu").attribute("fadeOutTime").as_float());
	
	
	return true;
}
