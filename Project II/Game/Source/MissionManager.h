#ifndef __MISSIONMANAGER_H__
#define __MISSIONMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "Timer.h"
#include "List.h"
#include <vector>
#include "SDL_ttf/include/SDL_ttf.h"
#include <map>
class MissionManager: public Module
{
	public:

	MissionManager();

	// Destructor
	virtual ~MissionManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	
	//Funcion para añadir a la lista de dialogos
	Dialog* ExecuteMission(pugi::xml_node itemNode, std::string name, const char* font);

	private:
		//Funcion que se encarga de mostrar los dialogos
		bool ShowDialog(Dialog* dialog);

		SDL_Texture* CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth);

		TTF_Font* FontSelector(const char* font);

	public:

		List<Dialog*> dialogues;

		bool isPlaying;

	private:

		//Colores

		SDL_Color textColor = { 0,0,0,0 };
		SDL_Color nameColor = { 255,255,255,255 };
		SDL_Color optionSelectedColor = { 200, 200, 200, 255 };

		//Posiciones
		std::vector<int> dialogMargin = { 130, 50, 0, 50 };
		iPoint dialogPosition = { 360 , 470 }; //{ 0 , 375 }
		iPoint namePosition = { 520 , 430 };
		iPoint optionsPosition = { 900 , 300 };
		int optionsDistanceBetween = 50;

		//Dimensiones
		int textBoundWidth = 100;
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
		std::map<std::string, std::string> missions;
		int indexText;
		Timer charTimer;
		int optionSelected = 0;



};

#endif // __MISSIONMANAGER_H__