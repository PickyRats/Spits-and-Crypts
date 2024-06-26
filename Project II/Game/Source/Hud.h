#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiControlCheckBox.h"
#include "GuiControlSlider.h"

#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

#include <vector>

struct SDL_Texture;

struct DrawTileCall
{
	SDL_Texture* texture;
	iPoint tilePosition;
};

struct Slot {
	iPoint position = { 0, 0 };
	SDL_Texture* texture = nullptr;
	bool isEmpty = true;
	bool isBought = false;
	int itemId = -1;	
	bool isEquiped = false;

};

struct Item {
	int health = 0;
	int attack = 0;
	int price = 0;
	bool isInInventary = false;
	SDL_Texture* texture = nullptr;
	SDL_Texture* ObjectText = nullptr;
	SDL_Texture* ObjectTextShop = nullptr;
};

struct Skill {
	SDL_Texture* texture;
	SDL_Rect position;
	bool locked;
	bool selected;
	int unlockRequirement; // ID de la habilidad que debe estar desbloqueada para desbloquear esta
	int id; // ID de la clase a la que pertenece
};

class Hud : public Module
{
public:

	Hud(bool enabled = true);

	virtual ~Hud();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	void Inventory();

	void Shop();

	bool CleanUp();

	void DrawTimer();



	void Missions(int mission1);
	void UpdatePlayerStats(Item& item, bool equip);
	void EquipItem(int itemId);

	void SkillTreeclass(int classid);

	void SkillTree();
	void ApplySkillEffects(int skillIndex);
	void HandleSelection(int currentIndex);

	void DrawTile(SDL_Texture* texture, iPoint position);
	void RenderStoredTiles();

	bool playerDeadHud = false;
	bool spacePressed = false;
	bool onSettings = false;

	int currentId = 0;

	int mission1 = 3;
	bool mission10Active = false;
	bool mission11Active = false;
	bool mission1Complete = false;

	bool mission20Active = false;
	bool mission21Active = false;
	bool mission2Complete = false;

	bool mission30Active = false;
	bool mission31Active = false;
	bool mission32Active = false;
	bool mission3Complete = false;

	bool abilityTree = false;

	bool wasSelectPressed = false;
	bool wasDownPressed = false;
	bool wasLeftPressed = false;
	bool wasRightPressed = false;
	bool wasUpPressed = false;
	bool wasR1Pressed = false;
	bool wasL1Pressed = false;
	bool wasYPressed = false;
	bool wasAPressed = false;
	bool wasBPressed = false;
	bool wasXPressed = false;

	Slot inventorySlots[4];

	Item items[3];
	int newItemId;

	Slot shopSlots[3];
	bool shop = false;
	
	int coin= 0;
	int exp = 0;
	char buffer[20];  // Suficientemente grande para almacenar el entero como cadena


	int classid = 2;
	
private:


	//PUNTOS

	SDL_Texture* vidas;
	SDL_Texture* points;
	SDL_Texture* Personaje1;
	SDL_Texture* Personaje2;
	SDL_Texture* Selectorazul;
	SDL_Texture* Selectornaranja;
	SDL_Texture* Opacity;
	SDL_Texture* Cuadrojugador;
	SDL_Texture* numeros;


	//Players
	SDL_Texture* MC_Idle;
	SDL_Texture* MC_Idle_1;
	SDL_Texture* MC_Idle_2;
	SDL_Texture* MC_Idle_3;
	SDL_Texture* MC_Idle_4;

	//SDL_Rect rect = { 0, 0, 9, 51 };
	SDL_Rect lifeRects[11] = { {0, 0, 9, 51}, { 9, 0, 9, 51}, { 18, 0, 9, 51}, { 27, 0, 9, 51}, { 36, 0, 9, 51}, { 45, 0, 9, 51}, { 54, 0, 9, 51}, { 63, 0, 9, 51}, { 72, 0, 9, 51}, { 81, 0, 9, 51}, {90, 0, 9 ,51} };
	SDL_Rect pointsRects[6] = { {0, 0, 82, 121}, {82, 0, 82, 121}, {164, 0, 82, 121}, {246, 0, 82, 121}, {328, 0, 82, 121}, {410, 0, 82, 121} };
	SDL_Rect numerosRects[9] = { {0, 0, 37, 37}, {37, 0, 37, 37}, {74, 0, 37, 37}, {111, 0, 37, 37}, {148, 0, 37, 37}, {185, 0, 37, 37}, {222, 0, 37, 37}, {259, 0, 37, 37}, {296, 0, 37, 37} };

	SDL_Texture* attack1;
	SDL_Texture* attack2;
	SDL_Texture* attack3;
	SDL_Texture* attack4;
	SDL_Texture* attack5;
	SDL_Texture* ability1;
	SDL_Texture* ability2;
	SDL_Texture* ability3;
	SDL_Texture* ability4;
	SDL_Texture* ability5;

	SDL_Texture* exitNormal;
	SDL_Texture* exitHover;
	SDL_Texture* exitClick;

	SDL_Texture* returnNormal;
	SDL_Texture* returnHover;
	SDL_Texture* returnClick;

	SDL_Texture* saveNormal;
	SDL_Texture* saveHover;
	SDL_Texture* saveClick;

	SDL_Texture* pause;

	SDL_Texture* resumeNormal;
	SDL_Texture* resumeHover;
	SDL_Texture* resumeClick;

	SDL_Texture* settingsNormal;
	SDL_Texture* settingsHover;
	SDL_Texture* settingsClick;

	SDL_Texture* backToTitleNormal;
	SDL_Texture* backToTitleHover;
	SDL_Texture* backToTitleClick;

	//Settings
	SDL_Texture* settingsControlsButtonNormal;
	SDL_Texture* settingsControlsButtonHover;

	SDL_Texture* settingsAudioButtonNormal;
	SDL_Texture* settingsAudioButtonHover;

	SDL_Texture* settingsOptionsButtonNormal;
	SDL_Texture* settingsOptionsButtonHover;

	SDL_Texture* settingsAudioPanel;
	SDL_Texture* settingsControls;
	SDL_Texture* settingsOptionsPanel;

	SDL_Texture* settingsExitNormal;
	SDL_Texture* settingsExitHover;
	SDL_Texture* settingsExitClick;

	SDL_Texture* settingsReturnNormal;
	SDL_Texture* settingsReturnHover;
	SDL_Texture* settingsReturnClick;

	SDL_Texture* settingsMusicNormal;
	SDL_Texture* settingsMusicHover;
	SDL_Texture* settingsMusicClick;

	SDL_Texture* settingsFxNormal;
	SDL_Texture* settingsFxHover;
	SDL_Texture* settingsFxClick;

	SDL_Texture* settingsFullScreenNormal;
	SDL_Texture* settingsFullScreenHover;
	SDL_Texture* settingsFullScreenClick;

	SDL_Texture* settingsVSyncNormal;
	SDL_Texture* settingsVSyncHover;
	SDL_Texture* settingsVSyncClick;

	SDL_Texture* settingsTick;

	SDL_Texture* settingsSlider;

	SDL_Texture* settingsBoxNormal;
	SDL_Texture* settingsBoxHover;

	SDL_Texture* settings;

	//Inventory

	SDL_Texture* inventoryTexture;
	SDL_Texture* inventoryItem1;
	SDL_Texture* inventoryItem2;
	SDL_Texture* inventoryItem3;
	SDL_Texture* ObjectText1;
	SDL_Texture* ObjectText2;
	SDL_Texture* ObjectText3;
	
	
	SDL_Texture* ObjectText1Shop;
	SDL_Texture* ObjectText2Shop;
	SDL_Texture* ObjectText3Shop;

	SDL_Texture* Habilidad1;
	SDL_Texture* Habilidad2;


	
	bool inventory = false;

	//Shop
	int itemId = 0;
	SDL_Texture* shopTexture;
	SDL_Texture* selectorItemTexture;

	SDL_Texture* Coin;
	SDL_Texture* Exp;



	GuiControlButton* exitButton;
	GuiControlButton* returnButton;
	GuiControlButton* resumeButton;
	GuiControlButton* saveButton;
	GuiControlButton* settingsButton;
	GuiControlButton* backToTitleButton;

	GuiControlButton* settingsExitButton;
	GuiControlButton* settingsReturnButton;
	GuiControlSlider* settingsMusicButton;
	GuiControlSlider* settingsFxButton;
	GuiControlButton* settingsOptionsButton;

	GuiControlButton* settingsFullScreenButton;
	GuiControlCheckBox* settingsVSyncButton;
	GuiControlButton* settingsControlsButton;
	GuiControlButton* settingsAudioButton;


	SDL_Texture* mission1i0;
	SDL_Texture* mission1i1;
	SDL_Texture* mission1i2;

	SDL_Texture* mission2i0;
	SDL_Texture* mission2i1;

	SDL_Texture* mission3i0;
	SDL_Texture* mission3i1;
	SDL_Texture* mission3i2;

	//HabilityTree

	SDL_Texture* skillTree;
	SDL_Texture* skillTreerama_1;
	SDL_Texture* skillTreerama_2_1;
	SDL_Texture* skillTreerama_2_2;
	SDL_Texture* skillTreerama_3_1;
	SDL_Texture* skillTreerama_3_2;

	SDL_Texture* SkillTreeTalent;

	SDL_Texture* SkillTreeLife_1;
	SDL_Texture* SkillTreeLife_2;
	SDL_Texture* SkillTreeSpeed_1;
	SDL_Texture* SkillTreeSpeed_2;

	SDL_Texture* SkillTreeAtack_1_1;
	SDL_Texture* SkillTreeAtack_1_2;
	SDL_Texture* SkillTreeAtack_2_1;
	SDL_Texture* SkillTreeAtack_2_2;
	SDL_Texture* SkillTreeAtack_3_1;
	SDL_Texture* SkillTreeAtack_3_2;
	SDL_Texture* SkillTreeAtack_4_1;
	SDL_Texture* SkillTreeAtack_4_2;
	SDL_Texture* SkillTreeAtack_5_1;
	SDL_Texture* SkillTreeAtack_5_2;
	SDL_Texture* SkillTreeAtack_6_1;
	SDL_Texture* SkillTreeAtack_6_2;
	SDL_Texture* SkillTreeAtack_7_1;
	SDL_Texture* SkillTreeAtack_7_2;
	SDL_Texture* SkillTreeAtack_8_1;
	SDL_Texture* SkillTreeAtack_8_2;	
	
	SDL_Texture* Bloqueado1_1 = NULL;
	SDL_Texture* Bloqueado1_2 = NULL;
	SDL_Texture* Bloqueado2_1 = NULL;
	SDL_Texture* Bloqueado2_2 = NULL;

	SDL_Texture* Rama1_1 = NULL;
	SDL_Texture* Rama1_2 = NULL;
	SDL_Texture* Rama2_1 = NULL;
	SDL_Texture* Rama2_2 = NULL;
	SDL_Texture* Rama3_1 = NULL;
	SDL_Texture* Rama3_2 = NULL;

	SDL_Texture* Description;
	SDL_Texture* DescAtackid = NULL;
	SDL_Texture* DescAtackid2 = NULL;
	SDL_Texture* DescTree;
	SDL_Texture* DescLife;
	SDL_Texture* DescSpeed;
	SDL_Texture* DescAtack_1;
	SDL_Texture* DescAtack_2;
	SDL_Texture* DescAtack_3;
	SDL_Texture* DescAtack_4;
	SDL_Texture* DescAtack_5;
	SDL_Texture* DescAtack_6;
	SDL_Texture* DescAtack_7;
	SDL_Texture* DescAtack_8;

	SDL_Texture* Selection;

	std::vector<Skill> skillTreenode;

	//Talents
	SDL_Texture* Talent1;
	SDL_Texture* Talent2;
	SDL_Texture* Talent3;
	SDL_Texture* Talent4;
	SDL_Texture* Talent5;



	//Talent 1
	bool talent1selected = true;

	//Talent 2
	bool talent2selected = false;
	bool talent2locked = false;

	//Talent 3
	bool talent3selected = false;
	bool talent3locked = false;

	//Talent 4
	bool talent4selected = false;
	bool talent4locked = false;

	//Talent 5
	bool talent5selected = false;
	bool talent5locked = false;

	bool onSettingsControls = false;
	bool onSettingsAudio = true;
	bool onSettingsOptions = false;
	bool buttonsActivated = false;

	std::vector<DrawTileCall> drawTileCalls;
};

#endif // __HUD_H__