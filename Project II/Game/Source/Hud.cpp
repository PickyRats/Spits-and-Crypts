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
#include "SceneTemple.h"
#include "Player.h"


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
	
	
	vidas = app->tex->Load(configNode3.child("vidas").attribute("texturepath").as_string());
	points = app->tex->Load(configNode3.child("points").attribute("texturepath").as_string());
	Personaje1 = app->tex->Load(configNode3.child("Personaje1").attribute("texturepath").as_string());
	Personaje2 = app->tex->Load(configNode3.child("Personaje2").attribute("texturepath").as_string());
	Selectorazul = app->tex->Load(configNode3.child("Selectorazul").attribute("texturepath").as_string());
	Selectornaranja = app->tex->Load(configNode3.child("Selectornaranja").attribute("texturepath").as_string());
	Opacity = app->tex->Load(configNode3.child("Opacity").attribute("texturepath").as_string());
	Cuadrojugador = app->tex->Load(configNode3.child("Cuadrojugador").attribute("texturepath").as_string());
	numeros = app->tex->Load(configNode3.child("numeros").attribute("texturepath").as_string());


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
	saveNormal = app->tex->Load(configNode3.child("saveNormal").attribute("texturepath").as_string());
	saveHover = app->tex->Load(configNode3.child("saveHover").attribute("texturepath").as_string());
	saveClick = app->tex->Load(configNode3.child("saveClick").attribute("texturepath").as_string());
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

	mission1i0 = app->tex->Load(configNode3.child("mission1i0").attribute("texturepath").as_string());
	mission1i1 = app->tex->Load(configNode3.child("mission1i1").attribute("texturepath").as_string());
	mission1i2 = app->tex->Load(configNode3.child("mission1i2").attribute("texturepath").as_string());


	settingsControlsButtonNormal = app->tex->Load(configNode3.child("settingsControlsButtonNormal").attribute("texturepath").as_string());
	settingsControlsButtonHover = app->tex->Load(configNode3.child("settingsControlsButtonHover").attribute("texturepath").as_string());
	settingsAudioButtonNormal = app->tex->Load(configNode3.child("settingsAudioButtonNormal").attribute("texturepath").as_string());
	settingsAudioButtonHover = app->tex->Load(configNode3.child("settingsAudioButtonHover").attribute("texturepath").as_string());
	settingsOptionsButtonNormal = app->tex->Load(configNode3.child("settingsOptionsButtonNormal").attribute("texturepath").as_string());
	settingsOptionsButtonHover = app->tex->Load(configNode3.child("settingsOptionsButtonHover").attribute("texturepath").as_string());


	//Inventory
	inventoryTexture = app->tex->Load(configNode3.child("inventory").attribute("texturepath").as_string());
	inventoryItem1 = app->tex->Load(configNode3.child("inventoryItem1").attribute("texturepath").as_string());
	inventoryItem2 = app->tex->Load(configNode3.child("inventoryItem2").attribute("texturepath").as_string());
	inventoryItem3 = app->tex->Load(configNode3.child("inventoryItem3").attribute("texturepath").as_string());

	items[0] = { 50, 0, 10, false, inventoryItem1 };
	items[1] = {  0, 10, 20, false, inventoryItem2 };
	items[2] = { 10, 0, 30, false, inventoryItem3 };

	inventorySlots[0].position = { 442, 272 };
	inventorySlots[1].position = { 532, 272 };
	inventorySlots[2].position = { 625, 272 };
	inventorySlots[3].position = { 320, 260 };


	//SHOP
	shopTexture = app->tex->Load(configNode3.child("shopTexture").attribute("texturepath").as_string());

	shopSlots[0] = { {343, 120}, items[0].texture, false };
	shopSlots[1] = { {532, 246}, items[1].texture, false };
	shopSlots[2] = { {128, 375}, items[2].texture, false };

	selectorItemTexture = app->tex->Load(configNode3.child("selectorItemTexture").attribute("texturepath").as_string());

	//Create Buttons
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, NULL, exitNormal, exitHover, exitHover, { 1419, 92, 63, 63 }, this);
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, NULL, resumeNormal, resumeHover, resumeHover, { 54, 198, 240, 55 }, this);
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, NULL, settingsNormal, settingsHover, settingsHover, { 54, 266, 240, 55 }, this);
	saveButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, NULL, saveNormal, saveHover, saveHover, { 54, 334, 240, 55 }, this);
	backToTitleButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, NULL, backToTitleNormal, backToTitleHover, backToTitleHover, { 54, 402, 240, 55 }, this);

	settingsReturnButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, NULL, returnNormal, returnHover, returnHover, { 133, 92, 63, 63 }, this);
	settingsExitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, NULL, exitNormal, exitHover, exitClick, { 1419, 92, 63, 63 }, this);
	settingsFullScreenButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, NULL, settingsBoxNormal, settingsBoxNormal, settingsTick, { 750, 187, 60, 32 }, this);
	settingsVSyncButton = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, NULL, settingsBoxNormal, settingsBoxNormal, settingsTick, { 750, 257, 89, 89 }, this);
	settingsMusicButton =(GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 231, NULL, settingsSlider, settingsSlider, settingsSlider, { 795, 197, 30, 30 }, this, { 570, 182, 225, 50 });
	settingsFxButton=(GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 21, NULL, settingsSlider, settingsSlider, settingsSlider, { 795, 263, 30, 30 }, this, { 570, 248, 225, 50 });
	settingsOptionsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, NULL, settingsOptionsButtonNormal, settingsOptionsButtonHover, settingsOptionsButtonHover, { 378, 116, 170, 30 }, this);
	settingsAudioButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, NULL, settingsAudioButtonNormal, settingsAudioButtonHover, settingsAudioButtonHover, { 560, 116, 170, 30 }, this);
	settingsControlsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, NULL, settingsControlsButtonNormal, settingsControlsButtonHover, settingsControlsButtonHover, { 742, 116, 170, 30 }, this);

	app->sceneMenu->loadGameButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->startButton->state = GuiControlState::HIDDEN;
	//app->sceneMenu->continueButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->settingsButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->creditsButton->state = GuiControlState::HIDDEN;
	app->sceneMenu->exitButton->state = GuiControlState::HIDDEN;

	//SkillTree
	skillTree = app->tex->Load(configNode3.child("SkillTree").attribute("texturepath").as_string());
	skillTreerama_1 = app->tex->Load(configNode3.child("SkillTreerama_1").attribute("texturepath").as_string());
	skillTreerama_2_1 = app->tex->Load(configNode3.child("SkillTreerama_2_1").attribute("texturepath").as_string());
	skillTreerama_2_2 = app->tex->Load(configNode3.child("SkillTreerama_2_2").attribute("texturepath").as_string());
	skillTreerama_3_1 = app->tex->Load(configNode3.child("SkillTreerama_3_1").attribute("texturepath").as_string());
	skillTreerama_3_2 = app->tex->Load(configNode3.child("SkillTreerama_3_2").attribute("texturepath").as_string());

	//SkillTree Buttons
	SkillTreeTalent = app->tex->Load(configNode3.child("SkillTreeTalent").attribute("texturepath").as_string());

	SkillTreeLife_1 = app->tex->Load(configNode3.child("SkillTreeLife_1").attribute("texturepath").as_string());
	SkillTreeLife_2 = app->tex->Load(configNode3.child("SkillTreeLife_2").attribute("texturepath").as_string());
	SkillTreeSpeed_1 = app->tex->Load(configNode3.child("SkillTreeSpeed_1").attribute("texturepath").as_string());
	SkillTreeSpeed_2 = app->tex->Load(configNode3.child("SkillTreeSpeed_2").attribute("texturepath").as_string());

	SkillTreeAtack_1_1 = app->tex->Load(configNode3.child("SkillTreeAtack_1_1").attribute("texturepath").as_string());
	SkillTreeAtack_1_2 = app->tex->Load(configNode3.child("SkillTreeAtack_1_2").attribute("texturepath").as_string());
	SkillTreeAtack_2_1 = app->tex->Load(configNode3.child("SkillTreeAtack_2_1").attribute("texturepath").as_string());
	SkillTreeAtack_2_2 = app->tex->Load(configNode3.child("SkillTreeAtack_2_2").attribute("texturepath").as_string());
	SkillTreeAtack_3_1 = app->tex->Load(configNode3.child("SkillTreeAtack_3_1").attribute("texturepath").as_string());
	SkillTreeAtack_3_2 = app->tex->Load(configNode3.child("SkillTreeAtack_3_2").attribute("texturepath").as_string());
	SkillTreeAtack_4_1 = app->tex->Load(configNode3.child("SkillTreeAtack_4_1").attribute("texturepath").as_string());
	SkillTreeAtack_4_2 = app->tex->Load(configNode3.child("SkillTreeAtack_4_2").attribute("texturepath").as_string());
	SkillTreeAtack_5_1 = app->tex->Load(configNode3.child("SkillTreeAtack_5_1").attribute("texturepath").as_string());
	SkillTreeAtack_5_2 = app->tex->Load(configNode3.child("SkillTreeAtack_5_2").attribute("texturepath").as_string());
	SkillTreeAtack_6_1 = app->tex->Load(configNode3.child("SkillTreeAtack_6_1").attribute("texturepath").as_string());
	SkillTreeAtack_6_2 = app->tex->Load(configNode3.child("SkillTreeAtack_6_2").attribute("texturepath").as_string());
	SkillTreeAtack_7_1 = app->tex->Load(configNode3.child("SkillTreeAtack_7_1").attribute("texturepath").as_string());
	SkillTreeAtack_7_2 = app->tex->Load(configNode3.child("SkillTreeAtack_7_2").attribute("texturepath").as_string());
	SkillTreeAtack_8_1 = app->tex->Load(configNode3.child("SkillTreeAtack_8_1").attribute("texturepath").as_string());
	SkillTreeAtack_8_2 = app->tex->Load(configNode3.child("SkillTreeAtack_8_2").attribute("texturepath").as_string());

	Selection = app->tex->Load(configNode3.child("Selection").attribute("texturepath").as_string());

	Talent1 = SkillTreeTalent;
	Talent2 = SkillTreeLife_1;
	Talent3 = SkillTreeSpeed_1;
	Talent4 = SkillTreeAtack_1_1;
	Talent5 = SkillTreeAtack_2_1;


	exitButton->state = GuiControlState::HIDDEN;
	resumeButton->state = GuiControlState::HIDDEN;
	settingsButton->state = GuiControlState::HIDDEN;
	saveButton->state = GuiControlState::HIDDEN;
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

	if (app->sceneMenu->vSync == true)
	{
		settingsVSyncButton->pressed = true;
	}
	else
	{
		settingsVSyncButton->pressed = false;
	}

	return true;
}

bool Hud::Update(float dt)
{
	//Ability Tree
	if (app->sceneTemple->active && app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		abilityTree = !abilityTree;
	}
	if (abilityTree)
	{
		SkillTree();
	}

	//Inventory
	Inventory();

	//Shop
	Shop();


	//Pause menu
	if (app->sceneVillage->pause || app->sceneShop->pause || app->sceneOasisFaraon->pause || app->sceneTemple->pause || app->sceneFloor1->pause) {
		//If pause menu is activated, show buttons
		if (app->sceneMenu->percentageMusic>5)
		{

			Mix_VolumeMusic(5);
		}
		if (resumeButton->state == GuiControlState::HIDDEN)
		{
			resumeButton->state = GuiControlState::NORMAL;
			settingsButton->state = GuiControlState::NORMAL;
			backToTitleButton->state = GuiControlState::NORMAL;
			saveButton->state = GuiControlState::NORMAL;
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

			if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && app->sceneMenu->currentId < 4))
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
				app->sceneMenu->currentId = 9;
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
				saveButton->state = GuiControlState::HIDDEN;
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
				//app->dialogManager->Disable();
				//app->physics->Disable();
				
			}
			else if (saveButton->state==GuiControlState::PRESSED)
			{
				app->audio->PlayFx(app->sceneMenu->FxButton1);
				app->sceneMenu->fxHoverPlayed = true;
				app->SaveRequest();
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
		else if (onSettings)
		{
			if ((app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN))
			{
				if (app->sceneMenu->currentId >= 9 && app->sceneMenu->currentId < 12)
				{
					app->sceneMenu->currentId = 12;
				}
				else if (app->sceneMenu->currentId >= 6 && app->sceneMenu->currentId < 9)
				{
					app->sceneMenu->currentId = 9;
				}
			}
			else if ((app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN))
			{
				if (app->sceneMenu->currentId >= 12)
				{
					app->sceneMenu->currentId = 9;
				}
				else if (app->sceneMenu->currentId >= 9 && app->sceneMenu->currentId < 12)
				{
					app->sceneMenu->currentId = 6;
				}
			}
			//Hide previous buttons
			resumeButton->state = GuiControlState::HIDDEN;
			settingsButton->state = GuiControlState::HIDDEN;
			backToTitleButton->state = GuiControlState::HIDDEN;
			saveButton->state = GuiControlState::HIDDEN;
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
				app->render->DrawTexture(settingsControls, 0, 0, NULL, SDL_FLIP_NONE, 0);
			}
			else if (onSettingsAudio)
			{
				app->render->DrawTexture(settingsAudioPanel, 0, 0, NULL, SDL_FLIP_NONE, 0);
				settingsFxButton->state = GuiControlState::NORMAL;
				settingsMusicButton->state = GuiControlState::NORMAL;
				settingsFullScreenButton->state = GuiControlState::HIDDEN;
				settingsVSyncButton->state = GuiControlState::HIDDEN;
			}
			else if (onSettingsOptions)
			{
				if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && app->sceneMenu->currentId != 8)
				{
					app->sceneMenu->currentId++;
					app->sceneMenu->fxHoverPlayed = false;
					app->sceneMenu->fxClickPlayed = false;
				}
				else if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && app->sceneMenu->currentId != 6)
				{
					app->sceneMenu->currentId--;
					app->sceneMenu->fxHoverPlayed = false;
					app->sceneMenu->fxClickPlayed = false;
				}
				app->render->DrawTexture(settingsOptionsPanel, 0, 0, NULL, SDL_FLIP_NONE, 0);

				if (buttonsActivated)
				{
					buttonsActivated = false;
					settingsVSyncButton->state = GuiControlState::NORMAL;
					settingsFullScreenButton->state = GuiControlState::NORMAL;
					settingsFxButton->state = GuiControlState::HIDDEN;
					settingsMusicButton->state = GuiControlState::HIDDEN;
				}
			}
		}
			if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			{
				app->sceneMenu->currentId = 1;
				onSettings = false;
				//Show menu buttons
				resumeButton->state = GuiControlState::NORMAL;
				settingsButton->state = GuiControlState::NORMAL;
				backToTitleButton->state = GuiControlState::NORMAL;
				saveButton->state = GuiControlState::NORMAL;
				exitButton->state = GuiControlState::NORMAL;

			}
			else if (settingsOptionsButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsOptionsButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}
				app->sceneMenu->fxClickPlayed = false;
				onSettingsControls = false;
				onSettingsOptions = true;
				onSettingsAudio = false;
				buttonsActivated = true;
			}
			else if (settingsAudioButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsAudioButton->state == GuiControlState::PRESSED)
			{
				if (app->sceneMenu->fxClickPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}
				onSettingsControls = false;
				onSettingsOptions = false;
				onSettingsAudio = true;
				buttonsActivated = true;
			}
			else if (settingsControlsButton->state == GuiControlState::FOCUSED)
			{
				if (app->sceneMenu->fxHoverPlayed == false)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton1);
					app->sceneMenu->fxHoverPlayed = true;
				}
			}
			else if (settingsControlsButton->state == GuiControlState::PRESSED)
			{
				if (!app->sceneMenu->fxClickPlayed)
				{
					app->audio->PlayFx(app->sceneMenu->FxButton2);
					app->sceneMenu->fxClickPlayed = true;
				}
				onSettingsControls = true;
				onSettingsOptions = false;
				onSettingsAudio = false;
				buttonsActivated = true;
			}
			else if (settingsFullScreenButton->state == GuiControlState::FOCUSED)
			{
				if (!app->sceneMenu->fxHoverPlayed)
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
	else
	{
		//If pause menu is deactivated, hide buttons

		resumeButton->state = GuiControlState::HIDDEN;
		settingsButton->state = GuiControlState::HIDDEN;
		backToTitleButton->state = GuiControlState::HIDDEN;
		saveButton->state = GuiControlState::HIDDEN;
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
			
			app->render->DrawTexture(Cuadrojugador, 10, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(Cuadrojugador, 70, 660, NULL, SDL_FLIP_NONE, 0);
			if (app->sceneCombat->currentPlayerIndex == 0) app->render->DrawTexture(Selectorazul, 10, 660, NULL, SDL_FLIP_NONE, 0);
			else if (app->sceneCombat->currentPlayerIndex == 1) app->render->DrawTexture(Selectorazul, 70, 660, NULL, SDL_FLIP_NONE, 0);

			app->render->DrawTexture(Personaje1, 20, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(Personaje2, 80, 660, NULL, SDL_FLIP_NONE, 0);
			////
			// 
			////barras vida
			
			app->render->DrawTexture(vidas, 63, 660, &lifeRects[app->map->player->health / 10]);

			app->render->DrawTexture(vidas, 123, 660, &lifeRects[app->map->player2->health / 10]);

			for (int i = 0; i < 2; i++)
			{
				app->render->DrawTexture(
					vidas, 
					app->sceneCombat->enemies[i]->position.x + 24, 
					app->sceneCombat->enemies[i]->position.y - 100, 
					&lifeRects[app->sceneCombat->enemies[i]->health / 10],
					SDL_FLIP_NONE,
					0,
					90
				);
			}

			////
			//
			////botones abajo derecha
			app->render->DrawTexture(Cuadrojugador, 1150, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(Cuadrojugador, 1210, 660, NULL, SDL_FLIP_NONE, 0);

			app->render->DrawTexture(Selectornaranja, 1210, 660, NULL, SDL_FLIP_NONE, 0);
			////
			//
			int currentPoints = app->sceneCombat->currentEntity->totalPoints - (app->sceneCombat->tilesCount - 1);
			float percentage = (float)currentPoints / (float)app->sceneCombat->currentEntity->totalPoints;
			int index = std::round((percentage + 0.1) * 6);
			if (index > 6) index = 6;
			if (app->sceneCombat->isPlayerTurn)
			{
				if (index > 0 && index <= 6) app->render->DrawTexture(points, 20, 20, &pointsRects[index-1]);
				else app->render->DrawTexture(points, 20, 20, &pointsRects[0]);
				app->render->DrawTexture(numeros, 38, 80, &numerosRects[currentPoints]);
			}
		}
	}
	if (mission10Active)
	{
		Missions(0);
	}
	else if (mission11Active)
	{
		Missions(1);
	}
	else if (mission1Complete)
	{
		mission10Active = false;
		mission11Active = false;
		app->tex->UnLoad(mission1i1);
		app->tex->UnLoad(mission1i0);
	}
	return true;
}


void Hud::Missions(int mission1)
{
	switch (mission1)
	{
	case 0:
		app->render->DrawTexture(mission1i0, 0, 0, NULL, SDL_FLIP_NONE, 0);
		break;
	case 1:
		app->render->DrawTexture(mission1i1, 0, 0, NULL, SDL_FLIP_NONE, 0);
		break;
	default:
		break;
	}
}

void Hud::UpdatePlayerStats(Item& item, bool equip) {
	int multiplier = equip ? 1 : -1;
	app->map->player->health += item.health * multiplier;
	app->map->player->attackDamage += item.attack * multiplier;
}

void Hud::EquipItem(int inventorySlotId) {
	if (!inventorySlots[3].isEmpty) {
		for (int i = 0; i < 4; i++)
		{
			inventorySlots[i].isEquiped = false;
		}
		int equippedItemId = inventorySlots[3].itemId;
		UpdatePlayerStats(items[equippedItemId], false);
	}

	int newItemId = inventorySlots[inventorySlotId].itemId;
	inventorySlots[itemId].isEquiped = true;
	inventorySlots[3].isEmpty = false;
	inventorySlots[3].texture = items[newItemId].texture;
	inventorySlots[3].itemId = newItemId;
	UpdatePlayerStats(items[newItemId], true);
}

void Hud::Inventory() {
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
		inventory = !inventory;
	}
	if (inventory) {
		app->render->DrawTexture(inventoryTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);

		char Vida[20];
		int vida = app->map->player->health;
		snprintf(Vida, sizeof(Vida), "%02d", vida);
		app->render->DrawText(Vida, 60, 580, 30, 18);

		char Armour[20];
		int daño = app->map->player->attackDamage;
		snprintf(Armour, sizeof(Armour), "%02d", daño);
		app->render->DrawText(Armour, 123, 580, 30, 18);

		for (int i = 0; i < 4; i++) {
			if (!inventorySlots[i].isEmpty) {
				app->render->DrawTexture(inventorySlots[i].texture, inventorySlots[i].position.x, inventorySlots[i].position.y, NULL, SDL_FLIP_NONE, 0);
			}
		}

		app->render->DrawTexture(selectorItemTexture, inventorySlots[itemId].position.x - 20, inventorySlots[itemId].position.y - 15, NULL, SDL_FLIP_NONE, 0);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && itemId > 0) {
			itemId--;
		}
		else if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && itemId < 2) {
			itemId++;
		}

		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			if (items[itemId].isInInventary && !inventorySlots[itemId].isEquiped) {
				EquipItem(itemId);
			}
		}
	}
}


void Hud::Shop() {
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
		shop = !shop;
	}
	if (shop) {
		app->render->DrawTexture(shopTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);

		for (int i = 0; i < 3; i++) 
		{
			if (!shopSlots[i].isEmpty) 
			{
				app->render->DrawTexture(shopSlots[i].texture, shopSlots[i].position.x, shopSlots[i].position.y, NULL, SDL_FLIP_NONE, 0);
			}
		}

		app->render->DrawTexture(selectorItemTexture, shopSlots[itemId].position.x, shopSlots[itemId].position.y, NULL, SDL_FLIP_NONE, 0);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && itemId > 0) {
			itemId--;
		}
		else if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && itemId < 2) {
			itemId++;
		}

		if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN && !shopSlots[itemId].isBought) {
			for (int i = 0; i < 3; i++) {
				if (inventorySlots[i].isEmpty) {
					inventorySlots[i].isEmpty = false;
					inventorySlots[i].texture = items[itemId].texture;
					inventorySlots[i].itemId = itemId;
					shopSlots[itemId].isEmpty = true;
					shopSlots[itemId].isBought = true;
					items[itemId].isInInventary = true;
					break;
				}
			}
		}
	}
}

bool Hud::CleanUp()
{
	LOG("Freeing Hud");

	app->tex->UnLoad(vidas);
	app->tex->UnLoad(points);
	app->tex->UnLoad(Personaje1);
	app->tex->UnLoad(Personaje2);
	app->tex->UnLoad(Selectorazul);
	app->tex->UnLoad(Selectornaranja);
	app->tex->UnLoad(Opacity);
	app->tex->UnLoad(Cuadrojugador);
	app->tex->UnLoad(numeros);
	app->tex->UnLoad(pause);
	app->tex->UnLoad(exitNormal);
	app->tex->UnLoad(exitHover);
	app->tex->UnLoad(exitClick);
	app->tex->UnLoad(resumeNormal);
	app->tex->UnLoad(resumeHover);
	app->tex->UnLoad(resumeClick);
	app->tex->UnLoad(settingsNormal);
	app->tex->UnLoad(settingsHover);
	app->tex->UnLoad(settingsClick);
	app->tex->UnLoad(backToTitleNormal);
	app->tex->UnLoad(backToTitleHover);
	app->tex->UnLoad(backToTitleClick);
	app->tex->UnLoad(settingsExitNormal);
	app->tex->UnLoad(settingsExitHover);
	app->tex->UnLoad(settingsExitClick);
	app->tex->UnLoad(settingsReturnNormal);
	app->tex->UnLoad(settingsReturnHover);
	app->tex->UnLoad(settingsReturnClick);
	app->tex->UnLoad(settingsTick);
	app->tex->UnLoad(settingsSlider);
	app->tex->UnLoad(settingsBoxNormal);
	app->tex->UnLoad(settingsBoxHover);
	app->tex->UnLoad(settings);
	app->tex->UnLoad(settingsControls);
	app->tex->UnLoad(settingsAudioPanel);
	app->tex->UnLoad(settingsOptionsPanel);
	app->tex->UnLoad(settingsControlsButtonNormal);
	app->tex->UnLoad(settingsControlsButtonHover);
	app->tex->UnLoad(settingsAudioButtonNormal);
	app->tex->UnLoad(settingsAudioButtonHover);
	app->tex->UnLoad(settingsOptionsButtonNormal);
	app->tex->UnLoad(settingsOptionsButtonHover);
	app->tex->UnLoad(inventoryTexture);
	app->tex->UnLoad(inventoryItem1);
	app->tex->UnLoad(inventoryItem2);
	app->tex->UnLoad(inventoryItem3);
	app->tex->UnLoad(shopTexture);
	app->tex->UnLoad(skillTree);
	app->tex->UnLoad(skillTreerama_1);
	app->tex->UnLoad(skillTreerama_2_1);
	app->tex->UnLoad(skillTreerama_3_1);
	app->tex->UnLoad(skillTreerama_2_2);
	app->tex->UnLoad(skillTreerama_3_2);
	app->tex->UnLoad(SkillTreeTalent);
	app->tex->UnLoad(SkillTreeLife_1);
	app->tex->UnLoad(SkillTreeLife_2);
	app->tex->UnLoad(SkillTreeSpeed_1);
	app->tex->UnLoad(SkillTreeSpeed_2);
	app->tex->UnLoad(SkillTreeAtack_1_1);
	app->tex->UnLoad(SkillTreeAtack_1_2);
	app->tex->UnLoad(SkillTreeAtack_2_1);
	app->tex->UnLoad(SkillTreeAtack_2_2);
	app->tex->UnLoad(SkillTreeAtack_3_1);
	app->tex->UnLoad(SkillTreeAtack_3_2);
	app->tex->UnLoad(SkillTreeAtack_4_1);
	app->tex->UnLoad(SkillTreeAtack_4_2);
	app->tex->UnLoad(SkillTreeAtack_5_1);
	app->tex->UnLoad(SkillTreeAtack_5_2);
	app->tex->UnLoad(SkillTreeAtack_6_1);
	app->tex->UnLoad(SkillTreeAtack_6_2);
	app->tex->UnLoad(SkillTreeAtack_7_1);
	app->tex->UnLoad(SkillTreeAtack_7_2);
	app->tex->UnLoad(SkillTreeAtack_8_1);
	app->tex->UnLoad(SkillTreeAtack_8_2);
	app->tex->UnLoad(Selection);

	return true;
}

void Hud::SkillTree()
{
	app->render->DrawTexture(skillTree, 0, 0, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama1_1, 405, 199, NULL, SDL_FLIP_HORIZONTAL, 0);
	app->render->DrawTexture(Rama1_2, 252, 199, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama2_1, 486, 336, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama2_2, 252, 336, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama3_1, 388, 436, NULL, SDL_FLIP_HORIZONTAL, 0);
	app->render->DrawTexture(Rama3_2, 251, 435, NULL, SDL_FLIP_NONE, 0);

	app->render->DrawTexture(Talent1, 333, 162, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Talent2, 450, 263, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Talent3, 450, 358, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Talent4, 213, 263, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Talent5, 213, 358, NULL, SDL_FLIP_NONE, 0);

	//Change sprites if talents are locked
	if (talent2locked)
	{
		Rama1_1 = skillTreerama_1;
		Rama2_1 = skillTreerama_2_1;
		Rama3_1 = skillTreerama_3_1;
		Talent2 = SkillTreeLife_2;
		app->map->player->health += 5;
	}
	if (talent3locked)
	{
		Rama2_1 = skillTreerama_2_2;
		Rama3_1 = skillTreerama_3_2;
		Talent3 = SkillTreeSpeed_2;
	}
	if (talent4locked)
	{
		Rama1_2 = skillTreerama_1;
		Rama2_2 = skillTreerama_2_1;
		Rama3_2 = skillTreerama_3_1;
		Talent4 = SkillTreeAtack_1_2;
		app->map->player->attackDamage += 5;
	}
	if (talent5locked)
	{
		Rama2_2 = skillTreerama_2_2;
		Rama3_2 = skillTreerama_3_2;
		Talent5 = SkillTreeAtack_2_2;
	}

	//Move the selection
	if (talent1selected)
	{
		app->render->DrawTexture(Selection, 333, 162);
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			talent1selected = false;
			talent2selected = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			talent1selected = false;
			talent4selected = true;
		}
	}
	if (talent2selected)
	{
		app->render->DrawTexture(Selection, 450, 263);
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			app->audio->PlayFx(app->sceneMenu->FxButton1);
			talent2locked = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			talent2selected = false;
			talent1selected = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			talent2selected = false;
			talent3selected = true;
		}
	}
	if (talent3selected)
	{
		app->render->DrawTexture(Selection, 450, 358);
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && talent2locked)
		{
			app->audio->PlayFx(app->sceneMenu->FxButton1);
			talent3locked = true;
		}
		if (talent3locked)
		{
			Talent3 = SkillTreeSpeed_2;
			app->render->DrawTexture(skillTreerama_2_2, 486, 336);
			app->render->DrawTexture(skillTreerama_3_2, 388, 436, NULL, SDL_FLIP_HORIZONTAL);
		}
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			talent3selected = false;
			talent2selected = true;
		}
	}
	if (talent4selected)
	{
		app->render->DrawTexture(Selection, 213, 263);
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			app->audio->PlayFx(app->sceneMenu->FxButton1);
			talent4locked = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			talent4selected = false;
			talent1selected = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			talent4selected = false;
			talent5selected = true;
		}
	}
	if (talent5selected)
	{
		app->render->DrawTexture(Selection, 213, 358);
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && talent4locked)
		{
			app->audio->PlayFx(app->sceneMenu->FxButton1);
			talent5locked = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			talent5selected = false;
			talent4selected = true;
		}
	}
}