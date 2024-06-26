#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "EntityManager.h"

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class SceneVillage;
class SceneShop;
class SceneOasisFaraon;
class SceneTemple;
class SceneChoza;
class SceneFloor1;
class SceneEnding;
class SceneLight;
class EntityManager;
class Map;
class Physics;
class FadeToBlack;
class ParticleManager;
class GuiManager;
class SceneMenu;
class Hud;
class SceneIntro;
class DialogManager;
class SceneCombat;
class Puzzle;
class Puzzle2;
class CutscenePlayer;
class SceneSelection;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT();

	// Request a save data in an XML file 
	bool LoadRequest();

	// Request to load data from XML file 
	bool SaveRequest();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Reads XML file and loads the data
	bool LoadFromFile();

	// Sace XML file with modules data
	bool SaveFromFile();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	SceneVillage* sceneVillage;
	SceneShop* sceneShop;
	SceneOasisFaraon* sceneOasisFaraon;
	SceneChoza* sceneChoza;
	SceneTemple* sceneTemple;
	SceneEnding* sceneEnding;
	SceneFloor1* sceneFloor1;
	SceneLight* sceneLight;
	EntityManager* entityManager;
	Map* map;
	Physics* physics;
	FadeToBlack* fade;
	ParticleManager* particleManager;
	GuiManager* guiManager;
	SceneMenu* sceneMenu;
	Hud* hud;
	SceneIntro* sceneIntro;
	DialogManager* dialogManager;
	SceneCombat* sceneCombat;
	Puzzle* puzzle;
	Puzzle2* puzzle2;
	CutscenePlayer* cutscenePlayer;
	SceneSelection* sceneSelection;

	uint32 maxFrameDuration = 16;
	uint32 initialMaxFrameDuration = 16;
	uint32 capMaxFrameDuration = 32;

	bool loadRequest = false;
	bool saveRequest = false;

private:

	int argc;
	char** args;
	SString gameTitle;
	SString organization;

	List<Module*> modules;

	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

	// required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float currentFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	char* vsync = nullptr;
};

extern App* app;

#endif	// __APP_H__