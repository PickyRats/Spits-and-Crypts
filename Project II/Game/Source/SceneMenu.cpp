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
#include "CutscenePlayer.h"
#include "SceneSelection.h"

#include "Defs.h"
#include "Log.h"
#include <iostream>
#include <fstream>

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
	background = app->tex->Load(configNode2.child("background").attribute("texturepath").as_string());//Cleaned
	background2 = app->tex->Load(configNode2.child("background2").attribute("texturepath").as_string());//Cleaned
	playNormal = app->tex->Load(configNode2.child("playNormal").attribute("texturepath").as_string());//Cleaned
	playHover = app->tex->Load(configNode2.child("playHover").attribute("texturepath").as_string());//Cleaned
	playClick = app->tex->Load(configNode2.child("playClick").attribute("texturepath").as_string());//Cleaned
	loadGameDisabled = app->tex->Load(configNode2.child("loadGameDisabled").attribute("texturepath").as_string());//Cleaned
	loadGameNormal = app->tex->Load(configNode2.child("loadGameNormal").attribute("texturepath").as_string());//Cleaned
	loadGameHover = app->tex->Load(configNode2.child("loadGameHover").attribute("texturepath").as_string());//Cleaned
	loadGameClick = app->tex->Load(configNode2.child("loadGameClick").attribute("texturepath").as_string());//Cleaned
	settingsNormal = app->tex->Load(configNode2.child("settingsNormal").attribute("texturepath").as_string());//Cleaned
	settingsHover = app->tex->Load(configNode2.child("settingsHover").attribute("texturepath").as_string());//Cleaned
	settingsClick = app->tex->Load(configNode2.child("settingsClick").attribute("texturepath").as_string());//Cleaned
	creditsNormal = app->tex->Load(configNode2.child("creditsNormal").attribute("texturepath").as_string());//Cleaned
	creditsHover = app->tex->Load(configNode2.child("creditsHover").attribute("texturepath").as_string());//Cleaned
	creditsClick = app->tex->Load(configNode2.child("creditsClick").attribute("texturepath").as_string());//Cleaned
	exitNormal = app->tex->Load(configNode2.child("exitNormal").attribute("texturepath").as_string());//Cleaned
	exitHover = app->tex->Load(configNode2.child("exitHover").attribute("texturepath").as_string());//Cleaned
	exitClick = app->tex->Load(configNode2.child("exitClick").attribute("texturepath").as_string());//Cleaned
	settings = app->tex->Load(configNode2.child("settings").attribute("texturepath").as_string());//Cleaned
	credits = app->tex->Load(configNode2.child("credits").attribute("texturepath").as_string());//Cleaned
	settingsBoxNormal = app->tex->Load(configNode2.child("settingsBoxNormal").attribute("texturepath").as_string());//Cleaned
	settingsBoxHover = app->tex->Load(configNode2.child("settingsBoxHover").attribute("texturepath").as_string());//Cleaned
	settingsControls = app->tex->Load(configNode2.child("settingsControls").attribute("texturepath").as_string());//Cleaned
	settingsTick = app->tex->Load(configNode2.child("settingsTick").attribute("texturepath").as_string());//Cleaned
	settingsSlider = app->tex->Load(configNode2.child("settingsSlider").attribute("texturepath").as_string());//Cleaned
	settingsAudioPanel = app->tex->Load(configNode2.child("settingsAudioPanel").attribute("texturepath").as_string());//Cleaned
	settingsOptionsButtonNormal = app->tex->Load(configNode2.child("settingsOptionsButtonNormal").attribute("texturepath").as_string());//Cleaned
	settingsOptionsButtonHover = app->tex->Load(configNode2.child("settingsOptionsButtonHover").attribute("texturepath").as_string());//Cleaned
	settingsAudioButtonNormal = app->tex->Load(configNode2.child("settingsAudioButtonNormal").attribute("texturepath").as_string());//Cleaned
	settingsAudioButtonHover = app->tex->Load(configNode2.child("settingsAudioButtonHover").attribute("texturepath").as_string());//Cleaned
	settingsControlsButtonNormal = app->tex->Load(configNode2.child("settingsControlsButtonNormal").attribute("texturepath").as_string());//Cleaned
	settingsControlsButtonHover = app->tex->Load(configNode2.child("settingsControlsButtonHover").attribute("texturepath").as_string());//Cleaned

	//settings
	settingsOptionsFullScreenHover = app->tex->Load(configNode2.child("settingsOptionsFullScreenHover").attribute("texturepath").as_string());//Cleaned
	settingsOptionsVsyncHover = app->tex->Load(configNode2.child("settingsOptionsVsyncHover").attribute("texturepath").as_string());//Cleaned
	settingsAudioMusicVolumeHover = app->tex->Load(configNode2.child("settingsAudioMusicVolumeHover").attribute("texturepath").as_string());//Cleaned
	settingsSfxButtonHover = app->tex->Load(configNode2.child("settingsSfxButtonHover").attribute("texturepath").as_string());//Cleaned
	settingsOptionsPanel = app->tex->Load(configNode2.child("settingsOptionsPanel").attribute("texturepath").as_string());//Cleaned

	//Load Music
	FxButton1 = app->audio->LoadFx(configNode2.child("buttonFx1").attribute("path").as_string());
	FxButton2 = app->audio->LoadFx(configNode2.child("buttonFx2").attribute("path").as_string());

	//Menu Buttons
	startButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, NULL, playNormal, playHover, playHover, { 54, 198, 240, 55 }, this);//primer numero la posicion en x, 2ndo la pos en y, 3r el largo del boton y 4t el alto del boton.
	loadGameButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2,loadGameDisabled, loadGameNormal, loadGameHover, loadGameHover, { 54, 266, 240, 55 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, NULL, settingsNormal, settingsHover, settingsHover, { 54, 334, 240, 55 }, this);
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, NULL, creditsNormal, creditsHover, creditsHover, { 54, 402, 240, 55 }, this);
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, NULL, exitNormal, exitHover, exitHover, { 54, 470, 240, 55 }, this);

	startButton->state = GuiControlState::NORMAL;
	loadGameButton->state = GuiControlState::DISABLED;
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");
	if(saveFile.child("game_state"))
	{
		loadGameButton->state = GuiControlState::NORMAL;
	}
	settingsButton->state = GuiControlState::NORMAL;
	creditsButton->state = GuiControlState::NORMAL;
	exitButton->state = GuiControlState::NORMAL;

	//Settings Buttons
	settingsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	settingsFullScreenButton = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, NULL, settingsBoxNormal, settingsBoxNormal, settingsTick, { 750, 187, 60, 32 }, this);
	settingsVSyncButton = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, NULL, settingsBoxNormal, settingsBoxNormal, settingsTick, { 750, 257, 89, 89 }, this);
	settingsMusicButton = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 231, NULL, settingsSlider, settingsSlider, settingsSlider, { 570, 197, 30, 30 }, this, { 570, 182, 225, 50 });
	settingsFxButton = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 21, NULL, settingsSlider, settingsSlider, settingsSlider, { 100, 263, 30, 30 }, this, { 570, 248, 225, 50 });

	settingsExitButton->state = GuiControlState::NORMAL;
	settingsFullScreenButton->state = GuiControlState::NORMAL;
	settingsVSyncButton->state = GuiControlState::NORMAL;
	settingsMusicButton->state = GuiControlState::NORMAL;
	settingsFxButton->state = GuiControlState::NORMAL;
	

	if (app->sceneMenu->vSync == true)
	{
		settingsVSyncButton->pressed = true;
	}
	else
	{
		settingsVSyncButton->pressed = false;
	}

	//Credits Buttons
	creditsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	

	//audio
	app->audio->PlayMusic(configNode2.child("musicMenu").attribute("path").as_string());

	return true;
}

bool SceneMenu::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	//Render background 
	app->render->DrawTexture(background, 0, 0);
	if (x > 366) {
		a = a * exp(-0.055);
		x -= a;
	}
	app->render->DrawTexture(background2, x, 33);

	int rangoMusic = settingsMusicButton->sliderBounds.w;
	percentageMusic = (((float)settingsMusicButton->sliderPosition - settingsMusicButton->sliderBounds.x) / rangoMusic) * 100;
	Mix_VolumeMusic(percentageMusic);
	
	int rangoFX = settingsFxButton->sliderBounds.w - 20;
	int percentageFX = (((float)settingsFxButton->sliderPosition - settingsFxButton->sliderBounds.x) / rangoFX) * 100;
	Mix_Volume(-1,percentageFX);


	//On menu screen

	if(onMenu && !onSettings && !onCredits){

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && currentId < 5)
			currentId++;

		if (pad.down == KEY_DOWN && !wasDownPressed && currentId < 5)
		{
			currentId++;
			wasDownPressed = true;
		}
		else if (pad.down != KEY_DOWN)
		{
			wasDownPressed = false;
		}

		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && currentId > 1))
		{
			currentId--;
		}

		if (pad.up == KEY_DOWN && !wasUpPressed && currentId > 1)
		{
			currentId--;
			wasUpPressed = true;
		}
		else if (pad.up != KEY_DOWN)
		{
			wasUpPressed = false;
		}

		//Hide settings buttons
		settingsExitButton->state = GuiControlState::HIDDEN;
		settingsFullScreenButton->state = GuiControlState::HIDDEN;
		settingsVSyncButton->state = GuiControlState::HIDDEN;
		settingsMusicButton->state = GuiControlState::HIDDEN;
		settingsFxButton->state = GuiControlState::HIDDEN;
		creditsExitButton->state = GuiControlState::HIDDEN;

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

			app->fade->Fade(this, (Module*)app->sceneShop, 60.0f);

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
				app->LoadRequest();
				app->fade->Fade(this, (Module*)app->sceneFloor1, 60.0f);
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
			currentId = 6;
			if (fxClickPlayed == false)
			{
				app->audio->PlayFx(FxButton2);
				fxClickPlayed = true;
			}
			//Show settings buttons
			onSettings = true;
			
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
				app->cutscenePlayer->Enable();
				app->cutscenePlayer->ConvertPixels(0, 1);
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
			}
			return false;
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
		OnSettings(pad);
	}

	//On credits screen
	else if (onCredits)
	{
		OnCredits(pad);
	}

	return true;
}

void SceneMenu::OnSettings(GamePad& pad)
{
	InputSettings(pad);

	//Hide menu buttons
	startButton->state = GuiControlState::HIDDEN;
	loadGameButton->state = GuiControlState::HIDDEN;
	settingsButton->state = GuiControlState::HIDDEN;
	creditsButton->state = GuiControlState::HIDDEN;
	exitButton->state = GuiControlState::HIDDEN;

	if (onMenu) {

		app->render->DrawTexture(settings, 0, 0, NULL, SDL_FLIP_NONE, 0);

		//Controls tab
		if (currentId == 12)
		{
			/*settingsFxButton->state = GuiControlState::HIDDEN;
			settingsMusicButton->state = GuiControlState::HIDDEN;
			settingsVSyncButton->state = GuiControlState::HIDDEN;*/
			settingsFullScreenButton->state = GuiControlState::HIDDEN;
			settingsVSyncButton->state = GuiControlState::HIDDEN;

			app->render->DrawTexture(settingsControlsButtonHover, 742, 110, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(settingsAudioButtonNormal, 560, 110, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(settingsOptionsButtonNormal, 378, 110, NULL, SDL_FLIP_NONE, 0);

			app->render->DrawTexture(settingsControls, 0, 0, NULL, SDL_FLIP_NONE, 0);
		}
		else if (currentId >= 9 && currentId <= 11)//Audio tab
		{

			LOG("%d", settingsMusicButton->sliderPosition);
			settingsFullScreenButton->state = GuiControlState::HIDDEN;
			settingsVSyncButton->state = GuiControlState::HIDDEN;
			settingsMusicButton->state = GuiControlState::NORMAL;

			app->render->DrawTexture(settingsControlsButtonNormal, 742, 110);
			app->render->DrawTexture(settingsAudioButtonHover, 560, 110);
			app->render->DrawTexture(settingsOptionsButtonNormal, 378, 110);

			app->render->DrawTexture(settingsAudioPanel, 0, 0);
			//app->render->DrawTexture(settingsAudioPanel, 0, 0, NULL, SDL_FLIP_NONE, 0);

			/*settingsFxButton->state = GuiControlState::NORMAL;
			settingsMusicButton->state = GuiControlState::NORMAL;
			settingsFullScreenButton->state = GuiControlState::HIDDEN;
			settingsVSyncButton->state = GuiControlState::HIDDEN;*/
			if (currentId == 10)//Fullscreen selected
			{
				app->render->DrawTexture(settingsAudioMusicVolumeHover, 420, 193);
			}
			if (currentId == 11)//SFX selected
			{
				app->render->DrawTexture(settingsSfxButtonHover,420,260);
			}
		}
		else if (currentId >= 6 && currentId <= 8)//Options tab
		{
			if (settingsFullScreenButton->state == GuiControlState::PRESSED)
			{
				if (fullScreen == false) fullScreen = true;
				else fullScreen = false;
				if (fxClickPlayed == false)
				{
					app->audio->PlayFx(FxButton2);
					fxClickPlayed = true;
				}
			}

			if (settingsVSyncButton->state == GuiControlState::PRESSED)
			{
				if (vSync == false) vSync = true;
				else vSync = false;
				if (fxClickPlayed == false)
				{
					app->audio->PlayFx(FxButton2);
					fxClickPlayed = true;
				}
			}

			settingsFxButton->state = GuiControlState::HIDDEN;
			settingsMusicButton->state = GuiControlState::HIDDEN;

			settingsFullScreenButton->state = GuiControlState::NORMAL;
			settingsVSyncButton->state = GuiControlState::NORMAL;

			app->render->DrawTexture(settingsControlsButtonNormal, 742, 110);
			app->render->DrawTexture(settingsAudioButtonNormal, 560, 110);
			app->render->DrawTexture(settingsOptionsButtonHover, 378, 110);

			app->render->DrawTexture(settingsOptionsPanel, 0, 0);

			if (currentId == 7)//Fullscreen selected
			{
				app->render->DrawTexture(settingsOptionsFullScreenHover, 465, 196);
			}
			else if (currentId == 8)//Vsync selected
			{
				app->render->DrawTexture(settingsOptionsVsyncHover, 465, 263);
			}

		}
	}

	//FullScreen and VSync
	if (fullScreen)	SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	else SDL_SetWindowFullscreen(app->win->window, 0);

	if (vSync)	app->render->vsync = true;
	else	app->render->vsync = false;

	if (!onSettings)
	{
		startButton->state = GuiControlState::NORMAL;
		loadGameButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::NORMAL;
		creditsButton->state = GuiControlState::NORMAL;
		exitButton->state = GuiControlState::NORMAL;
	}
}

void SceneMenu::InputSettings(GamePad& pad)
{

	if (pad.down == KEY_DOWN && !wasDownPressed && currentId != 8 && currentId != 11 && currentId != 12)
	{
		currentId++;
		wasDownPressed = true;
	}
	else if (pad.down != KEY_DOWN)
	{
		wasDownPressed = false;
	}

	if (pad.up == KEY_DOWN && !wasUpPressed &&  currentId != 6 && currentId != 9 && currentId != 12)
	{
		currentId--;
		wasUpPressed = true;
	}
	else if (pad.up != KEY_DOWN)
	{
		wasUpPressed = false;
	}

	if (currentId == 10 || currentId == 11)
	{
		//right arrow for volume
		if (currentId == 10 && pad.right == KEY_DOWN && !wasRightPressed)
		{
			LOG("Music volume +");
			LOG("%d", settingsMusicButton->sliderPosition);
			settingsMusicButton->sliderPosition++;
			LOG("%d", settingsMusicButton->sliderPosition);
			wasRightPressed = true;
		}
		else if (currentId == 11 && pad.right == KEY_DOWN && !wasRightPressed)
		{
			LOG("SFX Volume +");
			wasRightPressed = true;
		}
		else if (pad.right != KEY_DOWN)
		{
			wasRightPressed = false;
		}

		//left arrow for volume
		if (currentId == 10 && pad.left == KEY_DOWN && !wasLeftPressed)
		{
			LOG("%d", );
			wasLeftPressed = true;
		}
		else if (currentId == 11 && pad.left == KEY_DOWN && !wasLeftPressed)
		{
			LOG("SFX Volume -");
			wasLeftPressed = true;
		}
		else if (pad.left != KEY_DOWN)
		{
			wasLeftPressed = false;
		}

	}


	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || pad.r1 == KEY_DOWN && !wasR1Pressed)
	{
		if (currentId >= 9 && currentId < 12)
		{
			currentId = 12;
		}
		else if (currentId >= 6 && currentId < 9)
		{
			currentId = 9;
		}
		wasR1Pressed = true;
	}
	else if (pad.r1 != KEY_DOWN)
	{
		wasR1Pressed = false;
	}

	if ((app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN || pad.l1 == KEY_DOWN && !wasL1Pressed))
	{
		if (currentId >= 12)
		{
			currentId = 9;
		}
		else if (currentId >= 9 && currentId < 12)
		{
			currentId = 6;
		}
		wasL1Pressed = true;
	}
	else if (pad.l1 != KEY_DOWN)
	{
		wasL1Pressed = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || pad.b == KEY_DOWN)
	{
		currentId = 3;
		onSettings = false;
	}
}

void SceneMenu::OnCredits(GamePad& pad)
{
	currentId = 1;
	//Hide menu buttons
	startButton->state = GuiControlState::HIDDEN;
	loadGameButton->state = GuiControlState::HIDDEN;
	settingsButton->state = GuiControlState::HIDDEN;
	creditsButton->state = GuiControlState::HIDDEN;
	exitButton->state = GuiControlState::HIDDEN;

	//Render background and credits
	if (onMenu) {

		app->render->DrawTexture(credits, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//app->render->DrawTexture(controlsHint, 30, 670, NULL, SDL_FLIP_NONE, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || pad.b == KEY_DOWN)
	{
		if (onCredits)
		{
			onCredits = false;
			app->cutscenePlayer->Disable();
		}

		//Show menu buttons
		startButton->state = GuiControlState::NORMAL;
		loadGameButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::NORMAL;
		creditsButton->state = GuiControlState::NORMAL;
		exitButton->state = GuiControlState::NORMAL;
	}
	else
	{
		//Reset sound effects
		fxHoverPlayed = false;
		fxClickPlayed = false;
	}
}

bool SceneMenu::SaveState(pugi::xml_node node)
{
	pugi::xml_node status = node.append_child("saved");
	status.append_attribute("saved").set_value(1);

	return true;
}

bool SceneMenu::CleanUp()
{
	LOG("Freeing SceneMenu");

	//Clean Textures
	app->tex->UnLoad(background);
	app->tex->UnLoad(background2);
	app->tex->UnLoad(playNormal);
	app->tex->UnLoad(playHover);
	app->tex->UnLoad(playClick);
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
	app->tex->UnLoad(credits);
	app->tex->UnLoad(settingsBoxNormal);
	app->tex->UnLoad(settingsBoxHover);
	app->tex->UnLoad(settingsControls);
	app->tex->UnLoad(settingsTick);
	app->tex->UnLoad(settingsSlider);
	app->tex->UnLoad(settingsAudioPanel);
	app->tex->UnLoad(settingsOptionsFullScreenHover);
	app->tex->UnLoad(settingsOptionsVsyncHover);
	app->tex->UnLoad(settingsOptionsPanel);
	app->tex->UnLoad(settingsOptionsButtonNormal);
	app->tex->UnLoad(settingsOptionsButtonHover);
	app->tex->UnLoad(settingsAudioButtonNormal);
	app->tex->UnLoad(settingsAudioButtonHover);
	app->tex->UnLoad(settingsControlsButtonNormal);
	app->tex->UnLoad(settingsControlsButtonHover);
	app->tex->UnLoad(controlsHint);
	app->tex->UnLoad(settingsSfxButtonHover);
	//app->audio->UnloadFx(FxButton1);
	//app->audio->UnloadFx(FxButton2);

	//Clean Music
	app->audio->CleanMusic(configNode2.child("musicMenu").attribute("path").as_string(), configNode2.child("musicMenu").attribute("fadeOutTime").as_float());
	
	
	return true;
}
