#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "Timer.h"
#include "List.h"
#include <vector>
#include "SDL_ttf/include/SDL_ttf.h"

class DialogManager : public Module
{
public:

	DialogManager();

	// Destructor
	virtual ~DialogManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//Funcion para crear dialogos
	Dialog* CreateDialog(pugi::xml_node itemNode, std::string name = "", const char* faceTexturePath = "", const char* font = "primary");

	//Funcion para a�adir a la lista de dialogos
	bool AddDialog(Dialog* dialog);

private:
	//Funcion que se encarga de mostrar los dialogos
	bool ShowDialog(Dialog* dialog);

	SDL_Texture* CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth);

	TTF_Font* FontSelector(const char* font);


public:

	List<Dialog*> dialogues;

	bool isPlaying;
	bool dialogFinished;

	SDL_Texture* background_tex_logo;
	SDL_Texture* background_tex_logoMercante;
	SDL_Texture* background_tex_logoTabernero;
	SDL_Texture* background_tex_logoPalaya;
	SDL_Texture* background_tex_logoMaat;
	SDL_Texture* background_tex_logoThoth;
	SDL_Texture* background_tex_logoHorus;
	SDL_Texture* background_tex_logoIsis;

private:

	//Colores
	
	SDL_Color textColor = { 0,0,0,0 };
	SDL_Color nameColor= { 255,255,255,255 };
	SDL_Color optionSelectedColor = { 200, 200, 200, 255 };
	SDL_Color optionColor = { 23, 23, 23, 255 };


	//Posiciones
	std::vector<int> dialogMargin = { 130, 50, 0, 50 };
	iPoint dialogPosition = { 360 , 470 }; //{ 0 , 375 }
	iPoint namePosition = { 540 , 430 };
	iPoint optionsPosition = { 900 , 300 };
	int optionsDistanceBetween = 50;

	//Dimensiones
	int textBoundWidth= 100;
	int textNameBoundWidth = 200;
	int optionsBoundWidth = 350;
	iPoint faceTextureSize = { 100, 100 };


	//Config
	int charTimeMS = 10;


	//Para el funcionamiento

	SDL_Texture* background_tex;
	std::string background_tex_path;
	SDL_Texture* background_mission;
	std::string background_mission_path;
	int indexText;
	Timer charTimer;
	int optionSelected = 0;
	bool wasXPressed = false;
	bool wasDownPressed = false;
	bool wasUpPressed = false;
};

#endif // __DIALOGMANAGER_H__
