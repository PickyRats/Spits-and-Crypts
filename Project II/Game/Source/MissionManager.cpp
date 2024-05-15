#include "MissionManager.h"
#include "DialogManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "SceneVillage.h"
#include "Window.h"
#include "Dialog.h"

#include "Defs.h"
#include "Log.h"
#include "SDL_ttf/include/SDL_ttf.h"

MissionManager::MissionManager() : Module()
{
	name.Create("missionmanager");
}

// Destructor
MissionManager::~MissionManager()
{}

// Called before render is available

bool MissionManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Mission Manager");
	bool ret = true;

	for (pugi::xml_node missionNode = config.child("missions").child("mission"); missionNode; missionNode = missionNode.next_sibling("mission"))
	{
		std::string missionName = missionNode.attribute("name").as_string();
		std::string missionText = missionNode.child("text").attribute("value").as_string();
		missions[missionName] = missionText;
	}

	background_mission_path = config.child("textures").child("background_mission").attribute("texturepath").as_string();
	return ret;
}

bool MissionManager::Start() {

	bool ret = true;

	uint windowW, windowH;
	app->win->GetWindowSize(windowW, windowH);

	//Calculo del tamaño de la caja de texto
	textBoundWidth = windowW - dialogMargin[1] - dialogMargin[3];


	indexText = 1;


	
	background_mission = app->tex->Load(background_mission_path.c_str());

	return ret;
}

bool MissionManager::Update(float dt)
{
	return true;
}

bool MissionManager::CleanUp()
{
	bool ret = true;
	ListItem<Dialog*>* item;
	item = dialogues.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	dialogues.Clear();

	return ret;
}

bool MissionManager::ShowDialog(Dialog* dialog)
{
	// Mostrar fondo
	app->render->DrawTexture(background_mission, 0, 0, 0);


	std::string actualText = dialog->sentence.substr(0, indexText);

	
	SDL_Texture* textTexture = nullptr;
	SDL_Texture* textNameTexture = nullptr;

	
	int _dialogPositionX = dialogMargin[3] + dialogPosition.x;
	int _dialogPositionY = dialogMargin[0] + dialogPosition.y;

	// Textura del diálogo
	textTexture = CreateTextTexture(dialog->font, actualText.c_str(), textColor, textBoundWidth);
	app->render->DrawTexture(textTexture, _dialogPositionX, _dialogPositionY, 0);

	
	textNameTexture = CreateTextTexture(app->render->font, dialog->name.c_str(), nameColor, textNameBoundWidth);
	app->render->DrawTexture(textNameTexture, dialogMargin[3] + namePosition.x, dialogMargin[0] + namePosition.y);

	// Optimización de la memoria
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textNameTexture);

	// Añadir caracter según el tiempo
	if (actualText.size() < dialog->sentence.size()) {
		if (charTimer.ReadMSec() >= charTimeMS) {
			indexText++;
			charTimer.Start();
		}
		return false;
	}

	return true;
}



Dialog* MissionManager::ExecuteMission(pugi::xml_node itemNode, std::string name, const char* font)
{
	/*Dialog* missionDialog = new Dialog(missions[missionName]);
	app->dialogManager->AddDialog(missionDialog);*/

	Dialog* dialog = new Dialog(itemNode.attribute("value").as_string());
	dialog->name = name;
	dialog->font = FontSelector(font);

	return dialog;
}

SDL_Texture* MissionManager::CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth)
{
	// Crear la superficie de texto
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, color, textBoundWidth);
	if (surface == nullptr) {
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return nullptr;
	}

	// Crear la textura desde la superficie
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
	if (texture == nullptr) {
		LOG("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	}

	// Liberar la superficie
	SDL_FreeSurface(surface);

	return texture;
}

TTF_Font* MissionManager::FontSelector(const char* font)
{
	TTF_Font* selectedFont = nullptr;

	if (strcmp(font, "primary") == 0) {
		selectedFont = app->render->font;
	}
	else {
		LOG("Font %s not recognized.", font);
	}

	return selectedFont;
}


