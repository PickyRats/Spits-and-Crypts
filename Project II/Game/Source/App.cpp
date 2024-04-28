#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneVillage.h"
#include "SceneShop.h"
#include "SceneOasisFaraon.h"
#include "SceneTemple.h"
#include "SceneFloor1.h"
#include "SceneLight.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "ParticleManager.h"
#include "GuiManager.h"
#include "SceneMenu.h"
#include "Hud.h"
#include "SceneIntro.h"
#include "DialogManager.h"
#include "SceneCombat.h"
#include "Puzzle.h"
#include "Puzzle2.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	physics = new Physics();
	sceneVillage = new SceneVillage(false);
	sceneShop = new SceneShop(false);
	sceneOasisFaraon = new SceneOasisFaraon(false);
	sceneTemple = new SceneTemple(false);
	sceneFloor1 = new SceneFloor1(false);
	sceneLight = new SceneLight(false);
	map = new Map(false);
	entityManager = new EntityManager(false);
	fade = new FadeToBlack();
	particleManager = new ParticleManager(false);
	guiManager = new GuiManager();
	sceneMenu = new SceneMenu(false);
	hud = new Hud(false);
	sceneIntro = new SceneIntro();
	dialogManager = new DialogManager();
	sceneCombat = new SceneCombat(false);
	puzzle = new Puzzle(false);
	puzzle2 = new Puzzle2(false);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);

	//AddModule(scene);
	AddModule(sceneCombat);
	AddModule(sceneVillage);
	AddModule(sceneShop);
	AddModule(sceneOasisFaraon);
	AddModule(sceneTemple);
	AddModule(sceneFloor1);
	AddModule(sceneLight);
	AddModule(map);
	AddModule(entityManager);
	AddModule(puzzle);
	AddModule(puzzle2);
	AddModule(particleManager);
	AddModule(hud);
	AddModule(sceneMenu);
	AddModule(sceneIntro);
	AddModule(guiManager);
	AddModule(fade);
	AddModule(dialogManager);
	// Render last to swap buffer
	AddModule(render);


	LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	Timer timer = Timer();

	bool ret = LoadConfig();

	if (ret == true)
	{
		gameTitle = configNode.child("app").child("title").child_value();
		win->SetTitle(gameTitle.GetString());
		initialMaxFrameDuration = configFile.child("config").child("app").child("initialMaxFrameDuration").attribute("value").as_int();
		capMaxFrameDuration = configFile.child("config").child("app").child("capMaxFrameDuration").attribute("value").as_int();
		maxFrameDuration = initialMaxFrameDuration;

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (!item->data->active) {
			item = item->next;
			continue;
		}
		ret = item->data->Start();
		item = item->next;
	}

	LOG("Timer App Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		maxFrameDuration = (maxFrameDuration == initialMaxFrameDuration) ? capMaxFrameDuration : initialMaxFrameDuration;
	}

	// This is a good place to call Load / Save functions
	double currentDt = frameTime.ReadMs();
	if (app->render->vsync)
	{
		if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
			uint32 delay = (uint32)(maxFrameDuration - currentDt);

			PerfTimer delayTimer = PerfTimer();
			SDL_Delay(delay);
			//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
		}
	}

	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();

	// Amount of ms took the last update (dt)
	dt = (float)frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	vsync = app->render->vsync ? "on" : "off";

	// Shows the time measurements in the window title
	static char title[256];
	/*sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ",
		gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);*/

	sprintf_s(title, 256, "%s: FPS: %i / Avg.FPS: %.2f / Last-frame MS: %.0f / Vsync: %s ",
		gameTitle.GetString(), framesPerSecond, averageFps, dt, vsync);

	app->win->SetTitle(title);

	if (loadRequest) {
		loadRequest = false;
		LoadFromFile();
	}

	if (saveRequest) {
		saveRequest = false;
		SaveFromFile();
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		if (!item->data->active) {
			item = item->prev;
			continue;
		}
		ret = item->data->CleanUp();
		item = item->prev;
	}

	LOG("Timer App CleanUp(): %f", timer.ReadMSec());

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return gameTitle.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

float App::GetDT()
{
	return dt;
}

// Request a save data in an XML file 
bool App::LoadRequest() {

	bool ret = true;
	loadRequest = true;
	return ret;
}

// Request to load data from XML file 
bool App::SaveRequest() {
	bool ret = true;
	saveRequest = true;
	return true;
}

bool App::LoadFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	if (result)
	{
		LOG("save_game.xml parsed without errors");

		// Iterates all modules and call the load of each with the part of the XML node that corresponds to the module
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(saveFile.child("game_state").child(item->data->name.GetString()));
			item = item->next;
		}

	}
	else
	{
		LOG("Error loading save_game.xml: %s", result.description());
		ret = false;
	}



	return ret;

}

bool App::SaveFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_node gameState = saveFile.append_child("game_state");

	// Iterates all modules and call the save of each with the part of the XML node that corresponds to the module
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		pugi::xml_node module = gameState.append_child(item->data->name.GetString());
		ret = item->data->SaveState(module);
		item = item->next;
	}

	ret = saveFile.save_file("save_game.xml");

	if (ret) LOG("Saved");
	else LOG("Error saving game state");

	return ret;

}

