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
#include "SceneSelection.h"
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

	MC_Idle_1 = app->tex->Load(configNode3.child("MC_Idle_1").attribute("texturepath").as_string());
	MC_Idle_2 = app->tex->Load(configNode3.child("MC_Idle_2").attribute("texturepath").as_string());
	MC_Idle_3 = app->tex->Load(configNode3.child("MC_Idle_3").attribute("texturepath").as_string());
	MC_Idle_4 = app->tex->Load(configNode3.child("MC_Idle_4").attribute("texturepath").as_string());


	attack1 = app->tex->Load(configNode3.child("attack1").attribute("texturepath").as_string());
	attack2 = app->tex->Load(configNode3.child("attack2").attribute("texturepath").as_string());
	attack3 = app->tex->Load(configNode3.child("attack3").attribute("texturepath").as_string());
	attack4 = app->tex->Load(configNode3.child("attack4").attribute("texturepath").as_string());
	attack5 = app->tex->Load(configNode3.child("attack5").attribute("texturepath").as_string());

	ability1 = app->tex->Load(configNode3.child("ability1").attribute("texturepath").as_string());
	ability2 = app->tex->Load(configNode3.child("ability2").attribute("texturepath").as_string());
	ability3 = app->tex->Load(configNode3.child("ability3").attribute("texturepath").as_string());
	ability4 = app->tex->Load(configNode3.child("ability4").attribute("texturepath").as_string());
	ability5 = app->tex->Load(configNode3.child("ability5").attribute("texturepath").as_string());


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

	mission2i0 = app->tex->Load(configNode3.child("mission2i0").attribute("texturepath").as_string());
	mission2i1 = app->tex->Load(configNode3.child("mission2i1").attribute("texturepath").as_string());

	mission3i0 = app->tex->Load(configNode3.child("mission3i0").attribute("texturepath").as_string());
	mission3i1 = app->tex->Load(configNode3.child("mission3i1").attribute("texturepath").as_string());
	mission3i2 = app->tex->Load(configNode3.child("mission3i2").attribute("texturepath").as_string());


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
	ObjectText1 = app->tex->Load(configNode3.child("ObjectText1").attribute("texturepath").as_string());
	ObjectText2 = app->tex->Load(configNode3.child("ObjectText2").attribute("texturepath").as_string());
	ObjectText3 = app->tex->Load(configNode3.child("ObjectText3").attribute("texturepath").as_string());	
	ObjectText1Shop = app->tex->Load(configNode3.child("ObjectText1Shop").attribute("texturepath").as_string());
	ObjectText2Shop = app->tex->Load(configNode3.child("ObjectText2Shop").attribute("texturepath").as_string());
	ObjectText3Shop = app->tex->Load(configNode3.child("ObjectText3Shop").attribute("texturepath").as_string());

	Coin = app->tex->Load(configNode3.child("Coin").attribute("texturepath").as_string());

	items[0] = { 50, 0, 10, false, inventoryItem1 , ObjectText1 , ObjectText1Shop };
	items[1] = {  0, 10, 20, false, inventoryItem2 , ObjectText2 , ObjectText2Shop };
	items[2] = { 10, 0, 30, false, inventoryItem3 , ObjectText3 , ObjectText3Shop };

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

	DescTree = app->tex->Load(configNode3.child("DescTree").attribute("texturepath").as_string());
	DescLife = app->tex->Load(configNode3.child("DescLife").attribute("texturepath").as_string());
	DescSpeed = app->tex->Load(configNode3.child("DescSpeed").attribute("texturepath").as_string());
	DescAtack_1 = app->tex->Load(configNode3.child("DescAtack_1").attribute("texturepath").as_string());
	DescAtack_2 = app->tex->Load(configNode3.child("DescAtack_2").attribute("texturepath").as_string());
	DescAtack_3 = app->tex->Load(configNode3.child("DescAtack_3").attribute("texturepath").as_string());
	DescAtack_4 = app->tex->Load(configNode3.child("DescAtack_4").attribute("texturepath").as_string());
	DescAtack_5 = app->tex->Load(configNode3.child("DescAtack_5").attribute("texturepath").as_string());
	DescAtack_6 = app->tex->Load(configNode3.child("DescAtack_6").attribute("texturepath").as_string());
	DescAtack_7 = app->tex->Load(configNode3.child("DescAtack_7").attribute("texturepath").as_string());
	DescAtack_8 = app->tex->Load(configNode3.child("DescAtack_8").attribute("texturepath").as_string());

	Selection = app->tex->Load(configNode3.child("Selection").attribute("texturepath").as_string());

	classid = app->sceneSelection->currentSelection;

	Description = DescTree;
	Talent1 = SkillTreeTalent;
	Talent2 = SkillTreeLife_1;
	Talent3 = SkillTreeSpeed_1;
	Talent4 = Bloqueado1_1;
	Talent5 = Bloqueado2_1;
	
	skillTreenode = {
	{ Talent1, {333, 162, 77, 77}, false, false, -1, classid}, // Habilidad 1
	{ Talent2, {450, 263, 77, 77}, true, false, -1, classid },  // Habilidad 2
	{ Talent3, {450, 358, 77, 77}, true, false, 1, classid },  // Habilidad 3
	{ Talent4, {213, 263, 77, 77}, true, false, -1, classid },  // Habilidad 4
	{ Talent5, {213, 358, 77, 77}, true, false, 3, classid },  // Habilidad 5
	};

	skillTreenode[0].selected = true;

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

	if (app->sceneSelection->currentSelection == 0)
	{
		MC_Idle = MC_Idle_1;
	}
	if (app->sceneSelection->currentSelection == 1)
	{
		MC_Idle = MC_Idle_2;
	}
	if (app->sceneSelection->currentSelection == 2)
	{
		MC_Idle = MC_Idle_3;
	}
	if (app->sceneSelection->currentSelection == 3)
	{
		MC_Idle = MC_Idle_4;
	}

	return true;
}

bool Hud::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	if (app->sceneVillage->active) RenderStoredTiles();

	//Ability Tree
	if (app->sceneTemple->active && (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN /*|| (pad.b == KEY_DOWN && !wasBPressed*//*)*/))
	{
		abilityTree = !abilityTree;
		wasBPressed = true;
	}
	else if (pad.b != KEY_DOWN)
	{
		wasBPressed = false;
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

			if ((pad.down == KEY_DOWN && !wasDownPressed) && currentId != 8 && currentId != 11 && currentId != 12)
			{
				app->sceneMenu->currentId++;
				wasDownPressed = true;
			}
			else if (pad.down != KEY_DOWN)
			{
				wasDownPressed = false;
			}
			if ((pad.up == KEY_DOWN && !wasUpPressed) && currentId != 6 && currentId != 9 && currentId != 12)
			{
				app->sceneMenu->currentId--;
				wasUpPressed = true;
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
			if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || (pad.r1 == KEY_DOWN && !wasR1Pressed))
			{
				if (app->sceneMenu->currentId >= 9 && app->sceneMenu->currentId < 12)
				{
					app->sceneMenu->currentId = 12;
				}
				else if (app->sceneMenu->currentId >= 6 && app->sceneMenu->currentId < 9)
				{
					app->sceneMenu->currentId = 9;
				}
				wasR1Pressed = true;
			}
			else if (pad.r1 != KEY_DOWN)
			{
				wasR1Pressed = false;
			}
			if ((app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN || (pad.l1 == KEY_DOWN && !wasL1Pressed)))
			{
				if (app->sceneMenu->currentId >= 12)
				{
					app->sceneMenu->currentId = 9;
				}
				else if (app->sceneMenu->currentId >= 9 && app->sceneMenu->currentId < 12)
				{
					app->sceneMenu->currentId = 6;
				}
				wasL1Pressed = true;
			}
			else if (pad.l1 != KEY_DOWN)
			{
				wasL1Pressed = false;
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
				if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || (pad.down == KEY_DOWN && !wasDownPressed) && app->sceneMenu->currentId != 8)
				{
					app->sceneMenu->currentId++;
					app->sceneMenu->fxHoverPlayed = false;
					app->sceneMenu->fxClickPlayed = false;
					wasDownPressed = true;
				}
				else if (pad.down != KEY_DOWN)
				{
					wasDownPressed = false;
				}
				else if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || (pad.up == KEY_DOWN && !wasUpPressed) && app->sceneMenu->currentId != 6)
				{
					app->sceneMenu->currentId--;
					app->sceneMenu->fxHoverPlayed = false;
					app->sceneMenu->fxClickPlayed = false;
					wasUpPressed = true;
				}
				else if (pad.up != KEY_DOWN)
				{
					wasUpPressed = false;
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
			if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || pad.b == KEY_DOWN)
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

			RenderStoredTiles();

			////
			//
			////botones abajo derecha
			app->render->DrawTexture(Cuadrojugador, 1150, 660, NULL, SDL_FLIP_NONE, 0);
			app->render->DrawTexture(Cuadrojugador, 1210, 660, NULL, SDL_FLIP_NONE, 0);

			if (app->sceneCombat->useAbility) app->render->DrawTexture(Selectornaranja, 1210, 660, NULL, SDL_FLIP_NONE, 0);
			else app->render->DrawTexture(Selectornaranja, 1148, 660, NULL, SDL_FLIP_NONE, 0);
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
				if (app->sceneCombat->currentEntity->id == 1)
				{
					if (app->sceneSelection->currentSelection == 0)
					{
						app->render->DrawTexture(attack1, 1150, 660, NULL, SDL_FLIP_NONE, 0);
						app->render->DrawTexture(ability1, 1210, 660, NULL, SDL_FLIP_NONE, 0);
					}
					else if (app->sceneSelection->currentSelection == 1)
					{
						app->render->DrawTexture(attack2, 1150, 660, NULL, SDL_FLIP_NONE, 0);
						app->render->DrawTexture(ability2, 1210, 660, NULL, SDL_FLIP_NONE, 0);
					}
					else if (app->sceneSelection->currentSelection == 2)
					{
						app->render->DrawTexture(attack3, 1150, 660, NULL, SDL_FLIP_NONE, 0);
						app->render->DrawTexture(ability3, 1210, 660, NULL, SDL_FLIP_NONE, 0);
					}
					else if (app->sceneSelection->currentSelection == 3)
					{
						app->render->DrawTexture(attack4, 1150, 660, NULL, SDL_FLIP_NONE, 0);
						app->render->DrawTexture(ability4, 1210, 660, NULL, SDL_FLIP_NONE, 0);
					}
				}
				else if (app->sceneCombat->currentEntity->id == 2)
				{
					app->render->DrawTexture(attack5, 1150, 660, NULL, SDL_FLIP_NONE, 0);
					app->render->DrawTexture(ability5, 1210, 660, NULL, SDL_FLIP_NONE, 0);
				}
			}
		}
	}
	if (mission10Active)
	{
		Missions(0);
	}
	if (mission11Active)
	{
		Missions(1);
	}
	if (mission1Complete)
	{
		mission10Active = false;
		mission11Active = false;
		app->tex->UnLoad(mission1i1);
		app->tex->UnLoad(mission1i0);
	}
	if (mission20Active)
	{
		Missions(2);
	}
	if (mission21Active)
	{
		Missions(3);
	}
	if (mission2Complete)
	{
		mission20Active = false;
		mission21Active = false;
		app->tex->UnLoad(mission2i1);
		app->tex->UnLoad(mission2i0);
	}
	if (mission30Active)
	{
		Missions(4);
	}
	if (mission31Active)
	{
		Missions(5);
	}
	if (mission32Active)
	{
		Missions(6);
	}
	if (mission3Complete)
	{
		mission30Active = false;
		mission31Active = false;
		mission32Active = false;
		app->tex->UnLoad(mission3i1);
		app->tex->UnLoad(mission3i0);
		app->tex->UnLoad(mission3i2);
	}
	return true;
}
//Missions(0)-> mission 1.0
//Missions(1)-> mission 1.1

void Hud::Missions(int mission1)
{
	switch (mission1)
	{
	case 0:
		app->render->DrawTexture(mission1i0, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//printf("Mission 1.0\n");
		break;
	case 1:
		app->render->DrawTexture(mission1i1, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//printf("Mission 1.1\n");
		break;
	case 2:
		app->render->DrawTexture(mission2i0, 0, 0, NULL, SDL_FLIP_NONE, 0);
		break;
	case 3:
		app->render->DrawTexture(mission2i1, 0, 0, NULL, SDL_FLIP_NONE, 0);
		break;
	case 4:
		app->render->DrawTexture(mission3i0, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//printf("Mission 3.0\n");
		break;
	case 5:
		app->render->DrawTexture(mission3i1, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//printf("Mission 3.1\n");
		break;
	case 6:
		app->render->DrawTexture(mission3i2, 0, 0, NULL, SDL_FLIP_NONE, 0);
		//printf("Mission 3.2\n");
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

	newItemId = inventorySlots[inventorySlotId].itemId;
	inventorySlots[itemId].isEquiped = true;
	inventorySlots[3].isEmpty = false;
	inventorySlots[3].texture = items[newItemId].texture;
	inventorySlots[3].itemId = newItemId;
	UpdatePlayerStats(items[newItemId], true);
}

void Hud::Inventory() {
	GamePad& pad = app->input->pads[0];

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || (pad.y == KEY_DOWN && !wasYPressed))
	{
		inventory = !inventory;
		wasYPressed = true;

	}
	else if (pad.y != KEY_DOWN)
	{
		wasYPressed = false;
	}

	if (inventory)
	{
		app->render->DrawTexture(inventoryTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);
		app->render->DrawTexture(Habilidad1, 320, 376, NULL, SDL_FLIP_NONE, 0);
		app->render->DrawTexture(Habilidad2, 320, 464, NULL, SDL_FLIP_NONE, 0);
		app->render->DrawTexture(MC_Idle, 60, 236, NULL, SDL_FLIP_NONE, 0);
		

		char Vida[20];
		int vida = app->map->player->health;
		snprintf(Vida, sizeof(Vida), "%02d", vida);
		app->render->DrawText(Vida, 130, 580, 30, 18);

		char Armour[20];
		int dano = app->map->player->attackDamage;
		snprintf(Armour, sizeof(Armour), "%02d", dano);
		app->render->DrawText(Armour, 185, 580, 30, 18);

		app->render->DrawTexture(Coin, 1110, 100, NULL, SDL_FLIP_NONE, 0);
		snprintf(buffer, sizeof(buffer), "%d", coin);
		const char* miVariable = buffer;
		app->render->DrawText(miVariable, 1050, 100, 55, 55);


		for (int i = 0; i < 4; i++) {
			if (!inventorySlots[i].isEmpty) {
				app->render->DrawTexture(inventorySlots[i].texture, inventorySlots[i].position.x, inventorySlots[i].position.y, NULL, SDL_FLIP_NONE, 0);
			}
		}

		app->render->DrawTexture(selectorItemTexture, inventorySlots[itemId].position.x - 20, inventorySlots[itemId].position.y - 15, NULL, SDL_FLIP_NONE, 0);
		if (!inventorySlots[itemId].isEmpty)
		{
			app->render->DrawTexture(items[inventorySlots[itemId].itemId].ObjectText, 900, 300, NULL, SDL_FLIP_NONE, 0);
		}

		if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN||(pad.left==KEY_DOWN && !wasLeftPressed)) && itemId > 0) {
			itemId--;
			wasLeftPressed = true;
		}
		else if (pad.left != KEY_DOWN)
		{
			wasLeftPressed = false;
		}

		if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || (pad.right == KEY_DOWN && !wasRightPressed)) && itemId < 2) {
			itemId++;
			wasRightPressed = true;
		}
		else if (pad.right != KEY_DOWN)
		{
			wasRightPressed = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			if (!inventorySlots[itemId].isEmpty && !inventorySlots[itemId].isEquiped) {
				EquipItem(itemId);

			}
		}
	}
}

void Hud::Shop() {	
  GamePad& pad = app->input->pads[0];
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN||(pad.b==KEY_DOWN&&!wasBPressed)) {
		shop = false;
		wasBPressed = true;
	}
	else if (pad.b != KEY_DOWN)
	{
		wasBPressed = false;
	}
	if (shop) {
		app->render->DrawTexture(shopTexture, 0, 0, NULL, SDL_FLIP_NONE, 0);

		app->render->DrawTexture(Coin, 1100, 100, NULL, SDL_FLIP_NONE, 0);
		snprintf(buffer, sizeof(buffer), "%d", coin);
		const char* miVariable = buffer;
		app->render->DrawText(miVariable, 1050, 100, 55, 55);

		for (int i = 0; i < 3; i++) 
		{
			if (!shopSlots[i].isEmpty) 
			{
				app->render->DrawTexture(shopSlots[i].texture, shopSlots[i].position.x, shopSlots[i].position.y, NULL, SDL_FLIP_NONE, 0);
			}
		}

		app->render->DrawTexture(selectorItemTexture, shopSlots[itemId].position.x-20, shopSlots[itemId].position.y-15, NULL, SDL_FLIP_NONE, 0);
		app->render->DrawTexture(items[itemId].ObjectTextShop, 775, 200, NULL, SDL_FLIP_NONE, 0);

		if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || (pad.left == KEY_DOWN && !wasLeftPressed)) && itemId > 0)
		{
			itemId--;
			wasLeftPressed = true;

		}
		else if (pad.left != KEY_DOWN && wasLeftPressed)
		{
			wasLeftPressed = false;
		}

		else if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || (pad.right == KEY_DOWN && !wasRightPressed)) && itemId < 2)
		{
			itemId++;
			wasRightPressed = true;
		}
		else if (pad.right != KEY_DOWN && wasRightPressed)
		{
			wasRightPressed = false;
		}

		if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || (pad.a == KEY_DOWN && !wasAPressed)) && !shopSlots[itemId].isBought ) {
			for (int i = 0; i < 3; i++) {
				if (inventorySlots[i].isEmpty && (coin >= items[itemId].price))	{
					inventorySlots[i].isEmpty = false;
					inventorySlots[i].texture = items[itemId].texture;
					inventorySlots[i].itemId = itemId;
					shopSlots[itemId].isEmpty = true;
					shopSlots[itemId].isBought = true;
					items[itemId].isInInventary = true;
					coin = coin - items[itemId].price;
					break;
				}
			}
			wasAPressed = true;
		}
		else if (pad.a != KEY_DOWN )
		{
			wasAPressed = false;
		}
	}
}
    

void Hud::SkillTree() {

	GamePad& pad = app->input->pads[0];


	if (app->sceneTemple->active && app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		abilityTree = false;
	}

	// Dibuja el fondo del �rbol de habilidades

	app->render->DrawTexture(skillTree, 0, 0, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama1_1, 405, 199, NULL, SDL_FLIP_HORIZONTAL, 0);
	app->render->DrawTexture(Rama1_2, 252, 199, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama2_1, 486, 336, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama2_2, 252, 336, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Rama3_1, 388, 436, NULL, SDL_FLIP_HORIZONTAL, 0);
	app->render->DrawTexture(Rama3_2, 251, 435, NULL, SDL_FLIP_NONE, 0);
	app->render->DrawTexture(Description, 877, 275, NULL, SDL_FLIP_NONE, 0);


	// Inicializa las texturas de los nodos bloqueados
	SkillTreeclass(classid);

	// Itera sobre cada nodo de habilidad y dibuja su textura
	for (const auto& node : skillTreenode) {
		app->render->DrawTexture(node.texture, node.position.x, node.position.y, NULL, SDL_FLIP_NONE, 0);

		if (node.selected) {
			app->render->DrawTexture(Selection, node.position.x+64, node.position.y+45);
		}
	}

	// Maneja la selecci�n y el desbloqueo de habilidades
	for (int i = 0; i < skillTreenode.size(); ++i) {
		auto& node = skillTreenode[i];

		if (node.selected) {
			if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN ||pad.a==KEY_DOWN && !wasAPressed && (node.unlockRequirement == -1 || !skillTreenode[node.unlockRequirement].locked)) {
				app->audio->PlayFx(app->sceneMenu->FxButton1);
				node.locked = false;
				wasAPressed = true;
				// Aplica efectos seg�n el tipo de habilidad desbloqueada
				ApplySkillEffects(i);
			}
			else if (pad.a != KEY_DOWN)
			{
				wasAPressed = false;
			}
			// Maneja el movimiento de la selecci�n
			HandleSelection(i);
		}
	}
}

void Hud::ApplySkillEffects(int skillIndex) {
	switch (skillIndex) {
	case 1:
		skillTreenode[1].texture = SkillTreeLife_2;
		Rama1_1 = skillTreerama_1;
		Rama2_1 = skillTreerama_2_1;
		Rama3_1 = skillTreerama_3_1;
		app->map->player->health += 5;
		break;
	case 2:
		skillTreenode[2].texture = SkillTreeSpeed_2;
		Rama2_1 = skillTreerama_2_2;
		Rama3_1 = skillTreerama_3_2;
		break;
	case 3:
		skillTreenode[3].texture = Bloqueado1_2;
		Rama1_2 = skillTreerama_1;
		Rama2_2 = skillTreerama_2_1;
		Rama3_2 = skillTreerama_3_1;
		app->map->player->attackDamage += 5;
		Habilidad1 = Bloqueado1_2;
		break;
	case 4:
		skillTreenode[4].texture = Bloqueado2_2;
		Rama2_2 = skillTreerama_2_2;
		Rama3_2 = skillTreerama_3_2;
		Habilidad2 = Bloqueado2_2;
		break;
	}
}

void Hud::HandleSelection(int currentIndex) {
	GamePad& pad = app->input->pads[0];
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN|| pad.right == KEY_DOWN && !wasRightPressed) {
		if (currentIndex == 0) {
			skillTreenode[1].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescLife;
			wasRightPressed = true;
		}
	}
	else if (pad.right != KEY_DOWN)
	{
		wasRightPressed = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || pad.left == KEY_DOWN && !wasLeftPressed) {
		if (currentIndex == 0) {
			skillTreenode[3].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescAtackid;
			wasLeftPressed = true;
		}
	}
	else if (pad.left != KEY_DOWN)
	{
		wasLeftPressed = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.up == KEY_DOWN && !wasUpPressed) {
		if (currentIndex == 2) {
			skillTreenode[1].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescLife;
			
		}
		else if (currentIndex == 4) {
			skillTreenode[3].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescAtackid;
		}
		else if (currentIndex == 1 || currentIndex == 3) {
			skillTreenode[0].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescTree;
		}
		wasUpPressed = true;
	}
	else if (pad.up != KEY_DOWN)
	{
		wasUpPressed = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.down == KEY_DOWN && !wasDownPressed) {
		if (currentIndex == 1) {
			skillTreenode[2].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescSpeed;
		}
		else if (currentIndex == 3) {
			skillTreenode[4].selected = true;
			skillTreenode[currentIndex].selected = false;
			Description = DescAtackid2;
		}
		wasDownPressed = true;
	}

	else if (pad.down != KEY_DOWN)
	{
		wasDownPressed = false;
	}
}

void Hud::SkillTreeclass(int classid) {
	switch (classid)
	{
	case 1:
		Bloqueado1_1 = SkillTreeAtack_1_1;
		Bloqueado1_2 = SkillTreeAtack_1_2;
		Bloqueado2_1 = SkillTreeAtack_2_1;
		Bloqueado2_2 = SkillTreeAtack_2_2;
		DescAtackid = DescAtack_1;
		DescAtackid2 = DescAtack_2;
		break;
	case 2:
		Bloqueado1_1 = SkillTreeAtack_3_1;
		Bloqueado1_2 = SkillTreeAtack_3_2;
		Bloqueado2_1 = SkillTreeAtack_4_1;
		Bloqueado2_2 = SkillTreeAtack_4_2;
		DescAtackid = DescAtack_3;
		DescAtackid2 = DescAtack_4;
		break;
	case 3:
		DescAtackid = DescAtack_5;
		DescAtackid2 = DescAtack_6;
		Bloqueado1_1 = SkillTreeAtack_5_1;
		Bloqueado1_2 = SkillTreeAtack_5_2;
		Bloqueado2_1 = SkillTreeAtack_6_1;
		Bloqueado2_2 = SkillTreeAtack_6_2;
		break;
	case 4:
		DescAtackid = DescAtack_7;
		DescAtackid2 = DescAtack_8;
		Bloqueado1_1 = SkillTreeAtack_7_1;
		Bloqueado1_2 = SkillTreeAtack_7_2;
		Bloqueado2_1 = SkillTreeAtack_8_1;
		Bloqueado2_2 = SkillTreeAtack_8_2;
		break;
	}

	if (skillTreenode[3].locked)
	{
		skillTreenode[3].texture = Bloqueado1_1;
	}
	if (skillTreenode[4].locked)
	{
		skillTreenode[4].texture = Bloqueado2_1;
	}
}

void Hud::DrawTile(SDL_Texture* texture, iPoint tilePosition)
{
	DrawTileCall call;
	call.texture = texture;
	call.tilePosition = tilePosition;
	drawTileCalls.push_back(call);
}

void Hud::RenderStoredTiles()
{
	for (const auto& call : drawTileCalls)
	{
		app->render->DrawTexture(call.texture, call.tilePosition.x, call.tilePosition.y + 16);
	}
	drawTileCalls.clear();
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
	app->tex->UnLoad(DescTree);
	app->tex->UnLoad(DescLife);
	app->tex->UnLoad(DescSpeed);
	app->tex->UnLoad(DescAtack_1);
	app->tex->UnLoad(DescAtack_2);
	app->tex->UnLoad(DescAtack_3);
	app->tex->UnLoad(DescAtack_4);
	app->tex->UnLoad(DescAtack_5);
	app->tex->UnLoad(DescAtack_6);
	app->tex->UnLoad(DescAtack_7);
	app->tex->UnLoad(DescAtack_8);

	return true;
}

