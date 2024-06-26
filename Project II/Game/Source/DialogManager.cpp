#include "DialogManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "SceneVillage.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"
#include "SDL_ttf/include/SDL_ttf.h"

DialogManager::DialogManager() : Module()
{
	name.Create("dialogmanager");
}

// Destructor
DialogManager::~DialogManager()
{}

pugi::xml_node configNodeDialog;

// Called before render is available
bool DialogManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Dialog Manager");
	bool ret = true;

	configNodeDialog = config;


	return ret;

}

bool DialogManager::Start() {

	bool ret = true;

	uint windowW, windowH;
	app->win->GetWindowSize(windowW, windowH);

	//Calculo del tama�o de la caja de texto
	textBoundWidth = windowW - dialogMargin[1] - dialogMargin[3];


	indexText = 1;


	background_tex = app->tex->Load(configNodeDialog.child("background_dialog").attribute("texturepath").as_string());
	background_mission = app->tex->Load(background_mission_path.c_str());

	background_tex_logoMercante = app->tex->Load(configNodeDialog.child("background_dialoglogoMercante").attribute("texturepath").as_string());
	background_tex_logoTabernero = app->tex->Load(configNodeDialog.child("background_dialoglogoTebernero").attribute("texturepath").as_string());
	background_tex_logoPalaya = app->tex->Load(configNodeDialog.child("background_dialoglogoPalaya").attribute("texturepath").as_string());
	background_tex_logoMaat = app->tex->Load(configNodeDialog.child("background_dialoglogoMaat").attribute("texturepath").as_string());
	background_tex_logoThoth = app->tex->Load(configNodeDialog.child("background_dialoglogoThoth").attribute("texturepath").as_string());
	background_tex_logoIsis = app->tex->Load(configNodeDialog.child("background_dialoglogoIsis").attribute("texturepath").as_string());
	background_tex_logoHorus = app->tex->Load(configNodeDialog.child("background_dialoglogoHorus").attribute("texturepath").as_string());

	return ret;
}

// Called before quitting
bool DialogManager::CleanUp()
{
	bool ret = true;
	ListItem<Dialog*>* item;
	item = dialogues.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	dialogues.Clear();

	return ret;
}

Dialog* DialogManager::CreateDialog(pugi::xml_node itemNode, std::string name, const char* faceTexturePath, const char* font)
{
	//Dialogo a crear
	Dialog* dialog = new Dialog(itemNode.attribute("text").as_string());
	dialog->name = name;
	if (faceTexturePath != "") dialog->face_tex = app->tex->Load(faceTexturePath);
	dialog->font = FontSelector(font);


	const char* type = itemNode.attribute("type").as_string("");

	if (strcmp(type, "choose") == 0) {

		dialog->type = DialogType::CHOOSE;

		//Options1
		dialog->option1 = itemNode.child("option1").attribute("text").as_string();
		for (pugi::xml_node optionNode = itemNode.child("option1").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
			Dialog* dialogOp1 = CreateDialog(optionNode, name, faceTexturePath, font);
			dialog->options1.Add(dialogOp1);
		}

		//Options2
		dialog->option2 = itemNode.child("option2").attribute("text").as_string();
		for (pugi::xml_node optionNode = itemNode.child("option2").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
			Dialog* dialogOp2 = CreateDialog(optionNode, name, faceTexturePath, font);
			dialog->options2.Add(dialogOp2);
		}
	}

	return dialog;
}

bool DialogManager::AddDialog(Dialog* dialog)
{
	dialogues.Add(dialog);
	return true;
}

bool DialogManager::ShowDialog(Dialog* dialog)
{
	//Mostrar fondo
	app->render->DrawTexture(background_tex, 0, 0, NULL, SDL_FLIP_NONE, 0);

	app->render->DrawTexture(background_tex_logo, 618, 512, NULL, SDL_FLIP_NONE, 0);

	std::string actualText = dialog->sentence.substr(0, indexText);


	//Inicializando las variables para las texturas
	SDL_Texture* textTexture = nullptr;
	SDL_Texture* textNameTexture = nullptr;
	SDL_Texture* options1NameTexture = nullptr;
	SDL_Texture* options2NameTexture = nullptr;

	//Setting de las variables
	int _textBoundWidth;
	iPoint _dialogPosition;

	if (dialog->face_tex != nullptr) {
		_textBoundWidth = textBoundWidth - faceTextureSize.x;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x + faceTextureSize.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}
	else {
		_textBoundWidth = textBoundWidth;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}

	if (dialog->type == DialogType::CHOOSE) {
		_textBoundWidth = optionsPosition.x - faceTextureSize.x - 10; //CAMBIAR PARA AJUSTAR LAS OPCINES A LA DERECHA
	}

	//Textura dialogo
	textTexture = CreateTextTexture(dialog->font, actualText.c_str(), textColor, 450);
	app->render->DrawTexture(textTexture, _dialogPosition.x, _dialogPosition.y, NULL, SDL_FLIP_NONE, 0);

	//Imagen del personaje
	if (dialog->face_tex != nullptr) {
		app->render->DrawTexture(dialog->face_tex, dialogMargin[3] + dialogPosition.x, dialogMargin[0] + dialogPosition.y, NULL, SDL_FLIP_NONE, 0);
	}

	//Nombre personaje
	textNameTexture = CreateTextTexture(app->render->font, dialog->name.c_str(), nameColor, textNameBoundWidth);
	app->render->DrawTexture(textNameTexture, dialogMargin[3] + namePosition.x, dialogMargin[0] + namePosition.y, NULL, SDL_FLIP_NONE, 0);


	//Opciones
	if (dialog->type == DialogType::CHOOSE) {

		//Textura opcion1
		options1NameTexture = CreateTextTexture(app->render->font, dialog->option1.c_str(), (optionSelected == 1) ? optionSelectedColor : optionColor, optionsBoundWidth);
		app->render->DrawTexture(options1NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween, NULL, SDL_FLIP_NONE, 0);

		//Textura opcion2
		options2NameTexture = CreateTextTexture(app->render->font, dialog->option2.c_str(), (optionSelected == 2) ? optionSelectedColor : optionColor, optionsBoundWidth);
		app->render->DrawTexture(options2NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween * 2, NULL, SDL_FLIP_NONE, 0);
	}




	//Optimizacion de la memoria
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textNameTexture);
	SDL_DestroyTexture(options1NameTexture);
	SDL_DestroyTexture(options2NameTexture);




	//A�adir caracter segun el tiempo
	if (actualText.size() < dialog->sentence.size()) {

		if (charTimer.ReadMSec() >= charTimeMS) {
			indexText++;
			charTimer.Start();
		}

		return false;
	}

	return true;
}

SDL_Texture* DialogManager::CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth)
{
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;



	textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, textBoundWidth);
	textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

	SDL_FreeSurface(textSurface);

	return textTexture;
}

TTF_Font* DialogManager::FontSelector(const char* font)
{

	if (std::strcmp(font, "primary") == 0) {
		return app->render->font;

	}
	else if (std::strcmp(font, "secondary") == 0) {
		return app->render->font;
	}


	//Por defecto
	return app->render->font;
}

bool DialogManager::Update(float dt) {
	GamePad& pad = app->input->pads[0];
	bool ret = true;

	//Para saber si hay algun dialogo en funcionamiento
	isPlaying = (dialogues.Count() > 0);

	if (isPlaying) { //Entonces mostrar dialogos


		Dialog* actualDialog = dialogues.At(0)->data;
		dialogFinished = ShowDialog(actualDialog);



		//Gestionar la opcion seleccionada
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up == KEY_DOWN && !wasDownPressed) {
			optionSelected = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN|| pad.down == KEY_DOWN && !wasDownPressed){
			optionSelected = 2;
			wasDownPressed = true;
		}
		else if (pad.down != KEY_DOWN)
		{
			wasDownPressed = false;
		}
		else if (pad.up != KEY_DOWN)
		{
			wasUpPressed = false;
		}


		//Siguiente dialogo
		if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN ||(pad.x==KEY_DOWN && !wasXPressed) && actualDialog->type != DialogType::CHOOSE) {
			indexText = 1;
			dialogues.Del(dialogues.At(0));
			wasXPressed = true;
		}
		
		//Gestion de las opciones
		else if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || (pad.x == KEY_DOWN && !wasXPressed) && optionSelected != 0 && actualDialog->type == DialogType::CHOOSE) {


			if (optionSelected == 1) {
				dialogues.InsertAfter(0, actualDialog->options1);
			}
			else if (optionSelected == 2) {
				dialogues.InsertAfter(0, actualDialog->options2);
			}


			//Reiniciar varialbes de dialogo y quitar el dialogo actual de su lista
			optionSelected = 0;
			indexText = 1;
			dialogues.Del(dialogues.At(0));

		}
		
		//Terminar el dialogo empezado
		else if (!dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || (pad.x == KEY_DOWN && !wasXPressed) && indexText > 2) {
			indexText = 999;
		}

		else if (pad.x != KEY_DOWN)
		{
			wasXPressed = false;
		}

	}
	else {
		//Reiniciar variables mientras no este algun dialogo en marcha
		indexText = 1;
		optionSelected = 0;
	}

	return ret;

}